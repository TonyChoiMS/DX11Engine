#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{

}

Camera::~Camera()
{

}

void Camera::FinalUpdate()
{
	// 카메라의 트랜스폼을 월드행렬을 가져와서 역행렬을 구해줬습니다.
	// 뷰 행렬은 월드행렬의 역행렬이기 때문입니다.
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	// DirectX SimpleMath에서는 오른손좌표계 기반으로 행렬이 만들어져있기 때문에
	// LH(왼손좌표계)로 만들어주는 함수로 호출한 것입니다.
	// width / height = ratio
	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	S_MatView = _matView;
	S_MatProjection = _matProjection;
}

void Camera::Render()
{
	// 53:24부터 다시 시작
}

