//---------------------------------------
// テクスチャ＆サンプラーデータのグローバル変数定義
//---------------------------------------
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー
Texture2D decalTex : register(t1); //デカールテクスチャ

//---------------------------------------
//コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//---------------------------------------
cbuffer global
{
    float4x4 g_matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 g_matNormalTrans; // 法線の変換行列（回転行列と拡大の逆行列）
    float4x4 g_matWorld; // ワールド変換行列
    float4 g_vecLightDir; // ライトの方向ベクトル
    float4 g_vecDiffuse; // ディフューズカラー（マテリアルの色）
    float4 g_vecAmbient; // アンビエントカラー（影の色）
    float4 g_vecSpeculer; // スペキュラーカラー（ハイライトの色）
    float4 g_vecCameraPosition; // 視点（カメラの位置）
    float g_shuniness; // ハイライトの強さ（テカリ具合）
    bool g_isTexture; // テクスチャ貼ってあるかどうか

};
//---------------------------------------
// デカール直方体
//---------------------------------------

struct DecalBox
{
    float3 pos; //中心点
    int enable; //有効フラグ
    float4 right; //右（および幅）
    float4 up; //上（および高さ）
    float4 front; //前（および奥行）
};

#define mosaic_num 16
cbuffer ConstatBuffer : register(b4)
{
    DecalBox g_decalBox[mosaic_num];
}


//---------------------------------------
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//---------------------------------------
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float4 normal : TEXCOORD2; //法線
    float2 uv : TEXCOORD0; //UV座標
    float4 eye : TEXCOORD1; //視線
    float3 worldPos : TEXCOORD3; //ワールド座標	
};

//---------------------------------------
// 頂点シェーダ
//---------------------------------------
VS_OUT VS(float4 pos : POSITION, float4 Normal : NORMAL, float2 Uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, g_matWVP);
    
	//法線の変形
    Normal.w = 0; //4次元目は使わないので0
    Normal = mul(Normal, g_matNormalTrans); //オブジェクトが変形すれば法線も変形
    outData.normal = Normal; //これをピクセルシェーダーへ

	//視線ベクトル（ハイライトの計算に必要
    float4 worldPos = mul(pos, g_matWorld); //ローカル座標にワールド行列をかけてワールド座標へ
    outData.eye = normalize(g_vecCameraPosition - worldPos); //視点から頂点位置を引き算し視線を求めてピクセルシェーダーへ

	//UV「座標
    outData.uv = Uv; //そのままピクセルシェーダーへ


	//まとめて出力
    return outData;
}

//---------------------------------------
// ピクセルシェーダ
//---------------------------------------
float4 PS(VS_OUT inData) : SV_Target
{
	//ライトの向き
    float4 lightDir = g_vecLightDir; //グルーバル変数は変更できないので、いったんローカル変数へ
    float3 uvw = 0; //デカール直方体のサイズ
    float4 decal = 0;
	
    lightDir = normalize(lightDir); //向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
    inData.normal = normalize(inData.normal);

	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
    float4 shade = saturate(dot(inData.normal, -lightDir));
    shade.a = 1; //暗いところが透明になるので、強制的にアルファは1

    float4 diffuse;
	
    for (int i = 0; i < mosaic_num; ++i)
    {
        if (g_decalBox[i].enable)
        {
            float3 vec = inData.worldPos - g_decalBox[i].pos;

            //3軸への射影
            uvw = float3(
                dot(vec, g_decalBox[i].right.xyz),
                dot(vec, g_decalBox[i].up.xyz),
                dot(vec, g_decalBox[i].front.xyz)
            );

            // サイズで割って -1~1 に変換
            uvw /= float3(
                g_decalBox[i].right.w,
                g_decalBox[i].up.w,
                g_decalBox[i].front.w
            );

            // デカール範囲判定
            if (abs(uvw.x) < 1 && abs(uvw.y) < 1 && abs(uvw.z) < 1)
            {
                // UV変換（XY だけ）
                float2 uv = (float2(uvw.x, -uvw.y) + 1.0) * 0.5;

                decal = decalTex.Sample(g_sampler, uv);
                break;
            }
        }
	
    }
	
	//-------------------------------------
    //デカールとテクスチャを流す
    //-------------------------------------
    if (decal.a == 0)
    {
		//テクスチャ有無
        if (g_isTexture)
        {
			//テクスチャの色
            diffuse = g_texture.Sample(g_sampler, inData.uv);
        }
        else
        {
			//マテリアルの色
            diffuse = g_vecDiffuse;
        }
    }
    else
    {
		//デカール
        diffuse = decal;
    }
	
	//環境光（アンビエント）
	//これはMaya側で指定し、グローバル変数で受け取ったものをそのまま
    float4 ambient = g_vecAmbient;

	//鏡面反射光（スペキュラー）
    float4 speculer = float4(0, 0, 0, 0); //とりあえずハイライトは無しにしておいて…
    if (g_vecSpeculer.a != 0)	//スペキュラーの情報があれば
    {
        float4 R = reflect(lightDir, inData.normal); //正反射ベクトル
        speculer = pow(saturate(dot(R, inData.eye)), g_shuniness) * g_vecSpeculer; //ハイライトを求める
    }

	//最終的な色
    return diffuse * shade + diffuse * ambient + speculer;
}