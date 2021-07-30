#pragma once

// GameObject를가지고 있으면서 
// 시점에 맞춰 GameObject의 라이프사이클에 맞는 함수를 호출해주는 역할

class GameObject;

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

private:
	// TODO :: GameObject를 layer단위로 분류해서 관리할 수 있는 기능 추가 필요.
	vector<shared_ptr<GameObject>> _gameObjects;
};

