#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

	// Parent 기준
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	Vec3 GetWorldPosition() { return _matWorld.Translation(); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

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

