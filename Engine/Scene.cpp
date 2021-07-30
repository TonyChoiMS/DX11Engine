#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::Awake()
{
	// shared_ptr Ư���� � ��ü�� �����ϰ� �Ǹ� ref count�� 1�� �����մϴ�.
	// �׷��� const ���� ���۷����� �������� �Ǹ� ref count�� �����ϴµ�,
	// ���⼭�� ��ü�� �����͸� ��ȯ�� ������ �ƴϱ� ������ const�� ����ؼ� ���縦 �����ϴ� ���Դϴ�.
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObject : _gameObjects)
	{
		gameObject->LateUpdate();
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	_gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);
	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
	}
}
