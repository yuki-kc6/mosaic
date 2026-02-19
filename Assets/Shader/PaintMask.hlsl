
cbuffer BrushCB
{
    float2 g_hitUV;
    float g_radius;
    float padding; //16バイト境界に合わせるため
};


struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// 頂点シェーダー
VS_OUT VS(uint vid : SV_VertexID)
{
    VS_OUT output;
    
    float2 uv = float2((vid << 1) & 2, vid & 2);
    output.uv = uv;

    
    output.pos = float4(uv * float2(2, -2) + float2(-1, 1), 0, 1);
    return output;
}

// ピクセルシェーダー
float4 PS(VS_OUT input) : SV_Target
{
    
    float2 g_hitUV = float2(0.3, 0.5);
    float g_radius = 0.2;
    
   // 現在のピクセル(input.uv)とマウス位置(g_hitUV)の距離を計算
    float dist = distance(input.uv, g_hitUV);

    // 距離が半径以内なら「白」、外なら「何もしない」
    if (dist < g_radius)
    {
        return float4(1, 1, 1, 1);
    }
    
    // 半径の外側は描画自体をキャンセルする（元のテクスチャの色を維持する）
    discard;
    return float4(0, 0, 0, 0);
}