#include "pch.h"
#include "Transform.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

// 컴포넌트의 라이프사이클이 모두 완료 된 후에 호출될 함수.
// 사용자에게 제공하는 것이 아닌 엔진에서만 사용하는 함수.
void Transform::FinalUpdate()
{
}

void Transform::PushData()
{
}


// 첫번째 CBV는 transform으로 이용.
//CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));