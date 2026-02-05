float4 PS(float2 uv : TEXCOORD) : SV_Target
{
    // uv は 0〜1
    float2 center = float2(0.5, 0.5);
    float dist = distance(uv, center);

    float radius = 0.5;
    float alpha = step(dist, radius); // 円の内側だけ1

    return float4(alpha, alpha, alpha, 1.0);
}