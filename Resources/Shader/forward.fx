#ifndef _FORWARD_FX_
#define _FORWARD_FX_

#include "params.fx"
#include "utils.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;           // SV : SystemValue
    float2 uv : TEXCOORD;
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTangent : TANGENT;
    float3 viewBinormal : BINORMAL;
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
    output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz);        // 방향벡터는 마지막 w를 0으로 입력해줘야 translation이 적용되지 않기 때문에 마지막 값을 0으로 입력
    output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);        // g_matWV를 곱해줌으로써 view space 기준으로 한 탄젠트가 구해집니다.
    output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

    return output;
}

// 픽셀 단위 연산
float4 PS_Main(VS_OUT input) : SV_Target
{
    // 픽셀 쉐이더에서 계산한 컬러.
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);

    float3 viewNormal = input.viewNormal;
    // Bump mapping
    if (g_tex_on_1)
    {
        // 컬러는 샘플러를 통한 결과가 0~1값으로 변환된 값으로 받아옵니다.
        // [0. 255] 범위에서 [0, 1]로 변환
        float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
        // [0, 1] 범위에서 [-1, 1]로 변환
        tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
        float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
        viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
    }
    
    //float4 color = float4(1.f, 1.f, 1.f, 1.f);

    LightColor totalColor = (LightColor)0.f;

    for (int i = 0; i < g_lightCount; ++i)
    {
         LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
         totalColor.diffuse += color.diffuse;
         totalColor.ambient += color.ambient;
         totalColor.specular += color.specular;
    }

    color.xyz = (totalColor.diffuse.xyz * color.xyz)
        + totalColor.ambient.xyz * color.xyz
        + totalColor.specular.xyz;

     return color;
}


// [Texture Shader]
// 텍스쳐가 라이팅의 영향을 받지 않고 그대로 그리기 위한 쉐이더
// g_tex_0 : Output Texture
// AlphaBlend : true
struct VS_TEX_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEX_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_TEX_OUT VS_Tex(VS_TEX_IN input)
{
    VS_TEX_OUT output = (VS_TEX_OUT)0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    return output;
}

float4 PS_Tex(VS_TEX_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);

    return color;
}

#endif