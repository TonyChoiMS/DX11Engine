#ifndef _LIGHTING_FX_
#define _LIGHTING_FX_

#include "params.fx"
#include "utils.fx"

// 라이팅 관련 내용을 모두 가지고 있을 쉐이더

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

struct PS_OUT
{
	float4 diffuse : SV_Target0;
	float4 specular : SV_Target1; 
};

// [Directional Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_tex_2 : Shadow RT
// g_mat_0 : ShadowCamera VP
// Mesh : Rectangle

VS_OUT VS_DirLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// 2를 곱해주는 이유는 Resources에서 Rectangle을 만들 때 
	// width, height를 0.5, 0.5로 하기 때문에 투영 좌표계에서 
	// 화면 전체를 표현하기 위해(-1 ~ 1까지) *2를 실시.
	output.pos = float4(input.pos * 2.f, 1.f);		
	output.uv = input.uv;

	return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	// viewPos는 카메라를 기준 영역 좌표계이므로, 카메라보다 z좌표가 뒤에 있으면(음수이면)
	// 화면 내에 존재하지 않는 오브젝트라고 판단.
	float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
	if (viewPos.z <= 0.f)
		clip(-1);			// return 

	float3 viewNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;

	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);
	output.diffuse = color.diffuse + color.ambient;

	output.specular = color.specular;

	return output;
}

// [Point Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_vec2_0 : RenderTarget Resolution
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), g_matWVP);
	output.uv = input.uv;

	return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	// input.pos = SV_Position = Screen 좌표 (픽셀 좌표계)
	float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
	float3 viewPos = g_tex_0.Sample(g_sam_0, uv).xyz;
	if (viewPos.z <= 0.f)
		clip(-1);

	int lightIndex = g_int_0;
	float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
	float distance = length(viewPos - viewLightPos);
	if (distance > g_light[lightIndex].range)
		clip(-1);

	float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;

	LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);

	// diffuse가 0이면 아무것도 안보이는 곳이니 그림자 계산 필요 없음.
	if (length(color.diffuse) != 0)
	{
		// Shadow 적용 여부 확인 추가 코드
		matrix shadowCameraVP = g_mat_0;

		float4 worldPos = mul(float4(viewPos.xyz, 1.f), g_matViewInv);
		float4 shadowClipPos = mul(worldPos, shadowCameraVP);
		float depth = shadowClipPos.z / shadowClipPos.w;

		// x [-1 ~ 1] -> u [0 ~ 1]
		// y [1 ~ -1] -> v [0 ~ 1]
		float2 uv = shadowClipPos.xy / shadowClipPos.w;
		uv.y = -uv.y;
		uv = uv * 0.5 + 0.5;

		if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
		{
			float shadowDepth = g_tex_2.Sample(g_sam_0, uv).x;
			if (shadowDepth > 0 && depth > shadowDepth + 0.00001f)		// float 오차 판별을 위함. (앱실론)
			{
				color.diffuse *= 0.5f;
				color.specular = (float4)0.f;
			}
		}
	}

	output.diffuse = color.diffuse + color.ambient;
	output.specular = color.specular;

	return output;
}

// [Final]
// 인게임 세상에서 보여줄 쉐이더
// g_tex_0 : Diffuse Color Target;
// g_tex_1 : Diffuse Light Target;
// g_tex_2 : Specular Light Target;
// Mesh : Rectangel

VS_OUT VS_Final(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos * 2.f, 1.f);
	output.uv = input.uv;

	return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
	float4 output = (float4)0;

	float4 lightPower = g_tex_1.Sample(g_sam_0, input.uv);
	if (lightPower.x == 0.f && lightPower.y == 0.f && lightPower.z == 0.f)
		clip(-1);

	float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	float4 specular = g_tex_2.Sample(g_sam_0, input.uv);

	output = (color * lightPower) + specular;
	return output;
}

#endif