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

// 정점 단위 연산.
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    // Projection까지 가서 투영좌표계로 감.
    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    // 빛연산을 해주기 위해서 View좌표계까지만 계산
    output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz;      // x,y,z,w << w는 1이 되야함.
    output.viewNOrmal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);        // 방향벡터는 마지막 w를 0으로 입력해줘야 translation이 적용되지 않기 때문에 마지막 값을 0으로 입력

    return output;
}

// 픽셀 단위 연산
float4 PS_Main(VS_OUT input) : SV_Target
{
    // 픽셀 쉐이더에서 계산한 컬러.
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