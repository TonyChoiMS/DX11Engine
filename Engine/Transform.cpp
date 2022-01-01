#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

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
	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _matWorld * Camera::S_MatView;
	transformParams.matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	transformParams.matViewInv = Camera::S_MatView.Invert();

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushGraphicsData(&transformParams, sizeof(transformParams));
}

void Transform::LookAt(const Vec3& dir)
{
	// ���� �ٶ󺸰� ���� ���͸� front
	Vec3 front = dir;
	front.Normalize();

	Vec3 right = Vec3::Up.Cross(dir);
	if (right == Vec3::Zero)
		right = Vec3::Forward.Cross(dir);

	right.Normalize();

	Vec3 up = front.Cross(right);
	up.Normalize();

	Matrix matrix = XMMatrixIdentity();
	matrix.Right(right);
	matrix.Up(up);
	matrix.Backward(front);

	_localRotation = DecomposeRotationMatrix(matrix);
}

bool Transform::CloseEnough(const float& a, const float& b, const float& epsilon)
{
	return (epsilon > std::abs(a - b));
}

Vec3 Transform::DecomposeRotationMatrix(const Matrix& rotation)
{
	return Vec3();
}
