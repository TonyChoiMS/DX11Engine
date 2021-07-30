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

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

private:
	// TODO :: GameObject�� layer������ �з��ؼ� ������ �� �ִ� ��� �߰� �ʿ�.
	vector<shared_ptr<GameObject>> _gameObjects;
};

