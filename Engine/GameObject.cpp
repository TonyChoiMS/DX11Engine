#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "MonoBehaviour.h"


GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	AddComponent(make_shared<Transform>());
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

shared_ptr<Transform> GameObject::GetTransform()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(_components[index]);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	// this�� ������ shared_ptr�� �ƴ� GameObject�� ptr�̱� ������, this�� �Ѱ��ָ� ������ ������ �߻��մϴ�.
	// enable_shared_from_this<GameObject>�� ��ӹ��� ������ shared_from_this�Լ��� ����ϱ� �����Դϴ�.
	// �� ����� �ش� ��ü�� weak_ptr�� �ϳ� ��� �ְ� �Ǵµ�, shared_from_this �Լ��� ȣ�������ν� weak_ptr�� 
	// ȣ���� �� �ֽ��ϴ�. ��������� SetGameObject�� ���ڷ� �ѱ�� �Ǵ� �����ʹ� GameObject ��ü�� weak_ptr�� �ѱ��
	// �Ǵ� ���Դϴ�. 
	// �ڱ� �ڽſ� ���� ����Ʈ �����͸� �ѱ�� ���� �� ����մϴ�.
	component->SetGameObject(shared_from_this());
	// this ��� make(shared<GameObject>(this) �� ���ڷ� �ѱ�� ū�ϳ��ϴ�.
	// ���ο� ����Ʈ �����͸� ������ָ鼭 �� ��ü�� �����ϸ鼭 ref count�� 1�� �����͸� ���� ������ֱ� ������
	// ���� ��ü�� �����ϴ� ���� �ٸ� �� ����Ʈ�����Ͱ� ����� �˴ϴ�.
	// �׷��� �� �� �ϳ��� ref count�� 0�� �Ǽ� �Ҹ�Ǵ� ���� ���װ� �߻��մϴ�.

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	{
		// dynamic_pointer_cast == dynamic_cast�� ����� �Լ�.
		// �ٸ����� ����Ʈ�����͸� ���� ����ȯ �۾��� �ϰ���� �� dynamic_pointer_cast�� ����մϴ�.
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}


