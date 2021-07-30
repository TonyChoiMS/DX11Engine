#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::Awake()
{
	// shared_ptr 특성상 어떤 객체를 복사하게 되면 ref count가 1이 증가합니다.
	// 그래서 const 없이 레퍼런스를 가져오게 되면 ref count가 증가하는데,
	// 여기서는 객체의 데이터를 변환할 목적이 아니기 때문에 const를 사용해서 복사를 방지하는 것입니다.
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
