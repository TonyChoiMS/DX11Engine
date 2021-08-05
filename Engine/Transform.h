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

	// TODO :: �°� Parent/Child ����
private:
	// TODO :: World ��ġ ����
	// ���̾��Ű �� �θ� Ʈ�������� ������ ���, 
	// �ڽ� Ʈ�������� ��ǥ�� ���� ���� ��ǥ�踦 ������ �ִ� ���� �ƴ�,
	// �θ� ��ǥ�� �������� ��ǥ�迡���� ���� ��ǥ�踦 ����ϰ� �˴ϴ�.
	Vec3 _localPosition = {};
	Vec3 _localRotation = {};
	Vec3 _localScale = { 1.f, 1.f, 1.f };

	Matrix _matLocal = {};		// �θ� Ʈ�������� �������� �� ���� ��ǥ��
	Matrix _matWorld = {};

	weak_ptr<Transform> _parent;
};

