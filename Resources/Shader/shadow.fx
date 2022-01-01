#ifndef _SHADOW_FX_
#define _SHADOW_FX_

#include "params.fx"

// 그림자가 적용되야 하는 모든 물체에 해당 쉐이더가 적용됩니다.
struct VS_IN
{
	float3 pos : POSITION;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 clipPos : POSITION;
};

VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0.f;

	// pos에 clipPos가 저장되는데 따로 clipPos를 저장하는 이유는
	// pos는 SV_Position(SystemValue Position)이기 때문에, 
	// 픽셀 쉐이더로 넘어가는 순간에 우리가 생각하는 clipPos로 남아있는 것이 아니라,
	// 실제 픽셀 좌표로 변환되기 때문입니다.
	output.pos = mul(float4(input.pos, 1.f), g_matWVP);
	output.clipPos = output.pos;

	return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
	return float4(input.clipPos.z / input.clipPos.w, 0.f, 0.f, 0.f);
}

#endif