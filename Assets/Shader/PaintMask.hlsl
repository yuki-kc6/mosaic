// 頂点シェーダー
// 入力なし（SV_VertexIDだけで作る）
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUT VS(uint vid : SV_VertexID)
{
    VS_OUT output;
    // 0,1,2,3 のIDから四角形を作る魔法の計算
    float2 uv = float2((vid << 1) & 2, vid & 2);
    output.uv = uv;
    // 画面全体(-1～1)を覆う四角形を出力
    // ビューポートが小さくなっているので、実際にはブラシのサイズになる
    output.pos = float4(uv * float2(2, -2) + float2(-1, 1), 0, 1);
    return output;
}

// ピクセルシェーダー
float4 PS(VS_OUT input) : SV_Target
{
    // 円形に切り抜く
    float2 dist = input.uv - 0.5f; // 中心からの距離
    if (length(dist) > 0.5f)
        discard; // 半径0.5より外は描かない

    return float4(1, 1, 1, 1); // 白を塗る
}