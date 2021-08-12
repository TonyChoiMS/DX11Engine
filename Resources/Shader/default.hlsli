#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_Position;           // SV : SystemValue
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
};

// ���� ���� ����.
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    // Projection���� ���� ������ǥ��� ��.
    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    // �������� ���ֱ� ���ؼ� View��ǥ������� ���
    output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;      // x,y,z,w << w�� 1�� �Ǿ���.
    output.viewNOrmal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);        // ���⺤�ʹ� ������ w�� 0���� �Է������ translation�� ������� �ʱ� ������ ������ ���� 0���� �Է�

    return output;
}

// �ȼ� ���� ����
float4 PS_Main(VS_OUT input) : SV_Target
{
    // �ȼ� ���̴����� ����� �÷�.
    //float4 color = tex_0.Sample(g_sam_0, input.uv);
    float4 color = float4(1.f, 1.f, 1.f, 1.f);

    LightColor = totalColor = (LightColor)0.f;

    for (int i = 0; i < g_lightCount; i++)
    {
        LightColor color = CalculateLightColor(i, input.viewNormal, input.viewPos);
        totalColor.diffuse += color.diffuse;
        totalColor.ambient += color.ambient;
        totalColor.specular += color.specular;
    }

    color.xyz = (totalColor.diffuse.xyz * color.xyz)
        + totalColor.ambient.xyz * color.xyz
        + totalColor.specular.xyz;

    return color;
}

#endif