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

// 컴포넌트의 라이프사이클이 모두 완료 된 후에 호출될 함수.
// 사용자에게 제공하는 것이 아닌 엔진에서만 사용하는 함수.
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

	// 부모 오브젝트가 존재할 경우, 부모의 로컬 0,0,0을 기준으로 좌표를 추가 계산하기 위해
	// 부모의 좌표를 가져와서 계산합니다.
	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
}

// GPU에 데이터를 밀어넣어주기 위한 일련의 단계
void Transform::PushData()
{
	// [W]VP
	// View 행렬은 카메라가 들고 있음.
	// TODO:: 
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_matWorld, sizeof(_matWorld));
}
