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

// ���� ���� ����.
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    // Transloation�� ���� �ʰ� Rotation�� �����մϴ�. (w���� 0���� �����ϴ� ��)
    float4 viewPos = mul(float4(input.localPos, 0.0f), g_matView);
    float4 clipSpacePos = mul(viewPos, g_matProjection);

    // z���� w�� ���������ν�, w/w = 1�̱� ������ �׻� ���̰� 1�� �����˴ϴ�.
    output.pos = clipSpacePos.xyww;
    output.uv = input.uv;

    return output;
}

// �ȼ� ���� ����
float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = g_tex_0.Sample(g_sam_0, input.uv);
    return color;
}

#endif