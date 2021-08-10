#include "pch.h"
#include "Light.h"
#include "Transform.h"

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
}

Light::~Light()
{
}

void Light::FinalUPdate()
{
	_lightInfo.position = GetTransform()->GetWorldPosition();
}
