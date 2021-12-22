#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Material.h"
#include "Shader.h"
#include "ParticleSystem.h"

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

	_frustum.FinalUpdate();
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecForward.clear();
	_vecDeffered.clear();
	_vecParticle.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr && gameObject->GetParticleSystem() == nullptr)
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;
		
		// Frustum을 체크하지 않을 게임오브젝트는 패스.
		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		if (gameObject->GetMeshRenderer())
		{
			SHADER_TYPE shaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (shaderType)
			{
			case SHADER_TYPE::DEFERRED:
				_vecDeffered.push_back(gameObject);
				break;
			case SHADER_TYPE::FORWARD:
				_vecForward.push_back(gameObject);
				break;
			}
		}
		else
		{
			_vecParticle.push_back(gameObject);
		}
	}
}

void Camera::Render_Deferred()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& gameObject : _vecDeffered)
	{
		gameObject->GetMeshRenderer()->Render();
	}
}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (auto& gameObject : _vecForward)
	{
		gameObject->GetMeshRenderer()->Render();
	}

	for (auto& gameObject : _vecParticle)
	{
		gameObject->GetParticleSystem()->Render();
	}
}
