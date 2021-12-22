#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "MonoBehaviour.h"
#include "ParticleSystem.h"

GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{
}

GameObject::~GameObject()
{
}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		if (script)
			script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		if (script)
			script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		if (script)
			script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		if (script)
			script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}
}

// 혹시 잘못된 인덱스 접근을 하는지 확인하기 위한 함수.
shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER);
	return static_pointer_cast<MeshRenderer>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
}

shared_ptr<Light> GameObject::GetLight()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::LIGHT);
	return static_pointer_cast<Light>(component);
}

shared_ptr<ParticleSystem> GameObject::GetParticleSystem()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
	return static_pointer_cast<ParticleSystem>(component);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	// this는 엄밀히 shared_ptr가 아닌 GameObject의 ptr이기 때문에, this를 넘겨주면 컴파일 에러가 발생합니다.
	// enable_shared_from_this<GameObject>를 상속받은 이유는 shared_from_this함수를 사용하기 위함입니다.
	// 이 상속은 해당 객체의 weak_ptr를 하나 들고 있게 되는데, shared_from_this 함수를 호출함으로써 weak_ptr를 
	// 호출할 수 있습니다. 결과적으로 SetGameObject에 인자로 넘기게 되는 포인터는 GameObject 객체의 weak_ptr를 넘기게
	// 되는 것입니다. 
	// 자기 자신에 대한 스마트 포인터를 넘기고 싶을 때 사용합니다.
	component->SetGameObject(shared_from_this());
	// this 대신 make(shared<GameObject>(this) 를 인자로 넘기면 큰일납니다.
	// 새로운 스마트 포인터를 만들어주면서 이 객체를 참조하면서 ref count가 1인 포인터를 새로 만들어주기 때문에
	// 같은 객체를 참조하는 서로 다른 두 스마트포인터가 생기게 됩니다.
	// 그래서 둘 중 하나가 ref count가 0이 되서 소멸되는 순간 버그가 발생합니다.

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		// dynamic_pointer_cast == dynamic_cast와 흡사한 함수.
		// 다른점은 스마트포인터를 통한 형변환 작업을 하고싶을 때 dynamic_pointer_cast를 사용합니다.
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}


