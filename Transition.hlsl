/*
Texture2D SceneTexture : register(t0);
SamplerState Sampler : register(s0);

cbuffer MosaicBuffer : register(b0)
{
    int MosaicBlockNum;
    float3 padding;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 PS(VS_OUT input) : SV_TARGET
{
    float2 uv = input.uv;

    // ブロックごとに区切る
    uv.x = floor(uv.x * MosaicBlockNum) / MosaicBlockNum;
    uv.y = floor(uv.y * MosaicBlockNum) / MosaicBlockNum;

    return SceneTexture.Sample(Sampler, uv);
}
*/