#pragma once
#include "Component.h"

// 원근 투영을 해주는 카메라와 직교 투영을 해주는 카메라 2가지로 분류.
enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영
};
class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;

	// 카메라가 여러대가 될 수 있으므로, View와 Projection 행렬은 카메라가 가지고 있는게 맞습니다.
	Matrix _matView = {};
	Matrix _matProjection = {};

public:
	// TEMP
	static Matrix S_MatView;
	static Matrix S_MatProjection;

};