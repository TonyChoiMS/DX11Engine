#include "pch.h"
#include "Transform.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

// ������Ʈ�� ����������Ŭ�� ��� �Ϸ� �� �Ŀ� ȣ��� �Լ�.
// ����ڿ��� �����ϴ� ���� �ƴ� ���������� ����ϴ� �Լ�.
void Transform::FinalUpdate()
{
}

void Transform::PushData()
{
}


// ù��° CBV�� transform���� �̿�.
//CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));