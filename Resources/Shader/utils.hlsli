struct LightColor
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
};

struct LightInfo
{
    LightColor color;
    float4 position;
    float4 direction;
    int lightType;
    float range;
    float angle;
    int padding;
};

cbuffer GLOBAL_PARAMS : register(b0)
{
    int g_lightCount;
    float3 g_lightPadding;
    LightInfo g_light[50];
}


cbuffer TRANSFORM_PARAMS : register(b1)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProjection;
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
};

cbuffer MATERIAL_PARAMS : register(b2)
{
    // 다 사용하지 않아도 최대 크기로 사용하는 이유는
    // 당장 사용할 예정은 없지만 그때그때 빌드를 해야하는 수고를 덜기 위해서.
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    int g_int_4;
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    float g_float_4;
};

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);

SamplerState g_sam_0 : register(s0);

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = mul(float4(input.pos, 1.f), matWVP);
    output.uv = input.uv;

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    // 픽셀 쉐이더에서 계산한 컬러.
    float4 color = tex_0.Sample(sam_0, input.uv);

    return color;
}