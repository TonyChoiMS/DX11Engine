#pragma once

#include "Component.h"

struct TransformMatrix
{
	Vec4 offset;
};

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

	// TODO :: 온갖 Parent/Child 관계
private:
	// TODO :: World 위치 관련
	// 하이어라키 상 부모 트랜스폼이 존재할 경우, 
	// 자식 트랜스폼의 좌표는 순수 월드 좌표계를 가지고 있는 것이 아닌,
	// 부모 좌표를 기준으로 좌표계에서의 로컬 좌표계를 얘기하게 됩니다.
	Vec3 _localPosition = {};
	Vec3 _localRotation = {};
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = {};		// 부모 트랜스폼을 기준으로 한 로컬 좌표계
	Matrix _matWorld = {};

	weak_ptr<Transform> _parent;
};

