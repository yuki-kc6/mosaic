//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
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

cbuffer bulletHit
{
    float4 g_hitPos; // 弾の当たったワールド座標
    bool g_isHit; // 弾が当たったかどうか
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float4 normal : TEXCOORD2; //法線
    float2 uv : TEXCOORD0; //UV座標
    float4 eye : TEXCOORD1; //視線
    float3 worldPos : TEXCOORD3; //ワールド座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
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

    outData.worldPos = worldPos.xyz; //ワールド座標をピクセルシェーダーへ
    
	//まとめて出力
    return outData;
}
//───────────────────────────────────────
// ノイズ生成
//───────────────────────────────────────
static float hash(float3 p)
{
    p = frac(p * 0.3183099 + 0.1);
    p *= 17.0;
    return frac(p.x * p.y * p.z * (p.x + p.y + p.z));


}

//───────────────────────────────────────
// モザイク化
//───────────────────────────────────────
float Block3D(float3 p)
{
    return hash(floor(p));
}


//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
 
    float NoiseScale = 10.0;//ノイズの粗さ
    float radius = 0.5;//ノイズの半径
    //float3 hitPos = g_hitPos;//弾の当たったワールド座標
    //ワールド座標の定義
    float3 worldPos = inData.worldPos.xyz;
    float3 p = worldPos * NoiseScale;
    
    //ベースになるノイズ
    float NoiseColor = Block3D(p);
    
    
	//ライトの向き
    float4 lightDir = g_vecLightDir; //グルーバル変数は変更できないので、いったんローカル変数へ
    lightDir = normalize(lightDir); //向きだけが必要なので正規化

	//法線はピクセルシェーダーに持ってきた時点で補完され長さが変わっている
	//正規化しておかないと面の明るさがおかしくなる
    inData.normal = normalize(inData.normal);

	//拡散反射光（ディフューズ）
	//法線と光のベクトルの内積が、そこの明るさになる
    float4 shade = saturate(dot(inData.normal, -lightDir));
    shade.a = 1; //暗いところが透明になるので、強制的にアルファは1

    float4 diffuse;
	//テクスチャ有無
    if (g_isTexture == true)
    {
		//テクスチャの色
        diffuse = g_texture.Sample(g_sampler, inData.uv);
    }
    else
    {
		//マテリアルの色
        diffuse = g_vecDiffuse;
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

    
    float4 ResultColor = float4(NoiseColor, NoiseColor, NoiseColor, 1.0);
	//最終的な色
    //return diffuse * shade + diffuse * ambient + speculer;
    return ResultColor; 
}