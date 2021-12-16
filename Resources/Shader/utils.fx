#ifndef _UTILS_FX_
#define _UTILS_FX_

#include "params.fx"

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightColor color = (LightColor)0.f;

    float3 viewLightDir = (float3)0.f;

    float diffuseRatio = 0.f;
    float specularRatio = 0.f;
    float distanceRatio = 1.f;          // Point Light와 Spot Light에서 내가 빛을 받아야되는지 아닌지 체크하기 위한 거리를 계산하는 변수.

    if (g_light[lightIndex].lightType == 0)
    {
        // Directional Light
        viewLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);
        // dot(-viewLightDir, viewNormal)는 0~1사이
        // saturate는 0 미만의 값이 나오면 0으로 보정.
        // 빛이 정방향이 아니라 역방향으로 오는 것에 대한 처리를 위해 saturate 실행.
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));        
        
    }
    else if (g_light[lightIndex].lightType == 1)
    {
        // Point Light
        float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        float dist = distance(viewPos, viewLightPos);
        if (g_light[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else // 거리에 따라 적절히 세기를 조절 (멀어지면 멀어질수록 빛의 세기가 약해지게)
            distanceRatio = saturate(1.f - pow(dist / g_light[lightIndex].range, 2));
    }
    else
    {
        // Spot Light
        float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        if (g_light[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else
        {
            // 절반 각도
            float halfAngle = g_light[lightIndex].angle / 2;

            float3 viewLightVec = viewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);

            float centerDist = dot(viewLightVec, viewCenterLightDir);
            distanceRatio = saturate(1.f - centerDist / g_light[lightIndex].range);

            // 시야각을 구하는 공식. 여기서 viewLightVec 대신 viewLightDir를 사용해도 됬을 것 같음.
            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));      

            if (centerDist < 0.f || centerDist > g_light[lightIndex].range) // 최대 거리를 벗어났는지
                distanceRatio = 0.f;
            else if (lightAngle > halfAngle) // 최대 시야각을 벗어났는지 코사인은 양수,음수 모두 같기 때문에 음수 체크 안했습니다.
                distanceRatio = 0.f;
            else // 거리에 따라 적절히 세기를 조절 (멀어지면 멀어질수록 빛의 세기가 약해지게)
                distanceRatio = saturate(1.f - pow(centerDist / g_light[lightIndex].range, 2));
        }
    }

    // 반사각 단위 벡터
    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    // 시야 단위 벡터, 현재 연산을 진행하는 공간은 ViewSpace이므로, 카메라 위치는 (0, 0, 0)으로 볼 수 있습니다.
    // 그러므로 시야 벡터는 viewPos의 역방향을 사용하면 된다는 것을 알 수 있습니다. <<- eyeDir을 -eyeDir로 사용하는 이유
    float3 eyeDir = normalize(viewPos);
    specularRatio = saturate(dot(-eyeDir, reflectionDir));
    // 제곱을 하는 이유 : 코사인의 파동을 좀 더 가파르게 해주기 위해서.
    // ratio가 워낙 넓다보니, 영역을 좁히기 위해 pow를 해주고 있습니다.
    specularRatio = pow(specularRatio, 2);      
    

    color.diffuse = g_light[lightIndex].color.diffuse * diffuseRatio * distanceRatio;
    color.ambient = g_light[lightIndex].color.ambient * distanceRatio;
    color.specular = g_light[lightIndex].color.specular * specularRatio * distanceRatio;

    return color;
}

float Rand(float2 co)
{
    // frac -> 소수점을 추출하는 함수 ex. 1.234 -> 0.234 [0~1]
    return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}
#endif