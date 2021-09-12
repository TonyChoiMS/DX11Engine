#pragma once
#include "Component.h"
#include "Object.h"

class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;
class Light;

class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();

	void AddComponent(shared_ptr<Component> component);

	void SetCheckFrustum(bool checkFrustum) { _checkFrustum = checkFrustum; }
	bool GetCheckFrustum() { return _checkFrustum; }

	void SetLayerIndex(uint8 layer) { _layerIndex = layer; }
	uint8 GetLayerIndex() { return _layerIndex; }
	
private:
	// 컴포넌트는 하나씩만 붙일 수 있기 때문에 최대 개수가 예측이 되기에 array 자료구조 사용합니다.
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// MonoBehaviour는 사용자가 최대 몇개까지 붙일지 알 수 없기 때문에 vector를 사용합니다.
	vector<shared_ptr<MonoBehaviour>> _scripts;


	bool _checkFrustum = true;
	uint8 _layerIndex = 0;
};

