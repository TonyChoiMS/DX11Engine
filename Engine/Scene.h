#pragma once

// GameObject�������� �����鼭 
// ������ ���� GameObject�� ����������Ŭ�� �´� �Լ��� ȣ�����ִ� ����

class GameObject;

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	const vector<shared_ptr<GameObject>> GetGameObjects() { return _gameObjects; }

private:
	// TODO :: GameObject�� layer������ �з��ؼ� ������ �� �ִ� ��� �߰� �ʿ�.
	vector<shared_ptr<GameObject>> _gameObjects;
};
