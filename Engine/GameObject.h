#pragma once
#include "Component.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	shared_ptr<Transform> GetTransform();

	void AddComponent(shared_ptr<Component> component);


private:
	// 컴포넌트는 하나씩만 붙일 수 있기 때문에 최대 개수가 예측이 되기에 array 자료구조 사용합니다.
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// MonoBehaviour는 사용자가 최대 몇개까지 붙일지 알 수 없기 때문에 vector를 사용합니다.
	vector<shared_ptr<MonoBehaviour>> _scripts;

};

