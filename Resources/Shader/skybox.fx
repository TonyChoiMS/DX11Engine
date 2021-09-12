#ifndef _SKYBOX_FX_
#define _SKYBOX_FX_

#include "params.fx"

struct VS_IN
{
    float3 localPos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;           // SV : SystemValue
    float2 uv : TEXCOORD;
};

// 정점 단위 연산.
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    // Transloation은 하지 않고 Rotation만 적용합니다. (w값을 0으로 지정하는 것)
    float4 viewPos = mul(float4(input.localPos, 0.0f), g_matView);
    float4 clipSpacePos = mul(viewPos, g_matProjection);

    // z값에 w를 대입함으로써, w/w = 1이기 때문에 항상 깊이가 1로 유지됩니다.
    output.pos = clipSpacePos.xyww;
    output.uv = input.uv;

    return output;
}

// 픽셀 단위 연산
float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = g_tex_0.Sample(g_sam_0, input.uv);
    return color;
}

#endif