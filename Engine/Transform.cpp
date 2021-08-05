#include "pch.h"
#include "Transform.h"
#include "Engine.h"
//#include "Camera.h"

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
	// SRT
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;
	_matWorld = _matLocal;

	// �θ� ������Ʈ�� ������ ���, �θ��� ���� 0,0,0�� �������� ��ǥ�� �߰� ����ϱ� ����
	// �θ��� ��ǥ�� �����ͼ� ����մϴ�.
	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
}

// GPU�� �����͸� �о�־��ֱ� ���� �Ϸ��� �ܰ�
void Transform::PushData()
{
	// [W]VP
	// View ����� ī�޶� ��� ����.
	// TODO:: 
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_matWorld, sizeof(_matWorld));
}
