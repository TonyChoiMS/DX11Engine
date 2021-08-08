#pragma once
#include "Object.h"

// �ٸ� ������Ʈ���� �ϳ����ۿ� ���� �� ������,
// MonoBehaviour�� �������� ���� �� �ִ�.
enum class COMPONENT_TYPE : uint8 
{
	TRANSFORM,
	MESH_RENDERER,
	// ...
	MONO_BEHAVIOUR,
	CAMERA,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END)
};

class GameObject;
class Transform;

class Component : public Object
{
public:
	Component(COMPONENT_TYPE type);
	// ���̽� Ŭ������ Ȱ��� �����̱� ������ �� �Ҹ��ڸ� virtual�� Ȱ��
	virtual ~Component();

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}

public:
	COMPONENT_TYPE GetType() { return _type; }
	bool IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	// SetGameObject �Լ��� �ƹ� ��ü�� ȣ���Ҽ� ������ private���� ����� �� ��,
	// GameObject������ ȣ�� �����ϵ��� GameObject Ŭ������ friend Ŭ������ �����մϴ�.
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	// GameObject������ Component�������� �ֱ� ������ shared_ptr�� ����� �Ǹ� 
	// ��ȯ������ �Ǿ� ref count�� 0�� ���� �ʾ� ���� �������� ���� �� �ֱ� ������ weak_ptr����մϴ�.
	weak_ptr<GameObject> _gameObject;
};

