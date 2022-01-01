#include "pch.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "InstancingBuffer.h"
#include "Resources.h"

MeshRenderer::MeshRenderer() : Component(COMPONENT_TYPE::MESH_RENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	GetTransform()->PushData();
	_material->PushGraphicsData();
	_mesh->Render();
}

void MeshRenderer::Render(shared_ptr<class InstancingBuffer>& buffer)
{
	buffer->PushData();
	_material->PushGraphicsData();
	_mesh->Render(buffer);
}

void MeshRenderer::RenderShadow()
{
	// TODO :: Instancing 방법을 활용하는 방식으로 수정 필요.
	// 따로 작성되는 이유는 자신의 머테리얼을 이용하는 것이 아닌, 
	// Shadow를 그릴 때는 미리 만들어놓은 ShadowMap을 이용하기 때문.
	GetTransform()->PushData();
	GET_SINGLE(Resources)->Get<Material>(L"Shadow")->PushGraphicsData();
	_mesh->Render();
}

uint64 MeshRenderer::GetInstanceId()
{
	if (_mesh == nullptr || _material == nullptr)
		return 0;

	// uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}


