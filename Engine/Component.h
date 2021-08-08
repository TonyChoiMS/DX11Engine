#pragma once
#include "Object.h"

// 다른 컴포넌트들은 하나씩밖에 가질 수 없지만,
// MonoBehaviour는 여러개를 가질 수 있다.
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
	// 베이스 클래스로 활용될 예정이기 때문에 꼭 소멸자를 virtual로 활용
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
	// SetGameObject 함수를 아무 객체나 호출할수 없도록 private으로 만들어 준 뒤,
	// GameObject에서만 호출 가능하도록 GameObject 클래스를 friend 클래스로 선언합니다.
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	COMPONENT_TYPE _type;
	// GameObject에서도 Component를가지고 있기 때문에 shared_ptr로 만들게 되면 
	// 순환구조가 되어 ref count가 0이 되지 않아 영영 없어지지 않을 수 있기 때문에 weak_ptr사용합니다.
	weak_ptr<GameObject> _gameObject;
};

