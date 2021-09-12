#pragma once

#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

// 게임오브젝트, 메쉬 렌더러, 쉐이더, 텍스쳐 등 모든 리소스를 관리하는 매니저.
class Resources
{
	DECLARE_SINGLE(Resources);

public:
	void Init();

	// 템플릿으로 사용하는 이유 : Resources에는 다양한 클래스가 들어있기 때문에,
	// 그것들의 공통적인 기능들을 모두 아울러서 처리하기 위함.
	template<typename T> 
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> object);

	template<typename T>
	shared_ptr<T> Get(const wstring& Key);

	template<typename T>
	OBJECT_TYPE GetObjectType();

	shared_ptr<Mesh> LoadRectangleMesh();
	shared_ptr<Mesh> LoadCubeMesh();
	shared_ptr<Mesh> LoadSphereMesh();

private:
	void CreateDefaultShader();

private:
	using KeyObjMap = std::map<wstring/*key*/, shared_ptr<Object>>;
	// 리소스 별로 관리하는 Map을 따로 만들기 위해 array로 정의.
	array<KeyObjMap, OBJECT_TYPE_COUNT> _resources;
};


// 템플릿 관련 구현은 일반적으로 헤더파일 내부에서 모두 작성하며,
// 헤더파일이 너무 길어질 경우 inl파일로 분리해서 파일을 인클루드 하는 방식으로
// 사용하기도 합니다.
template<typename T>
inline shared_ptr<T> Resources::Load(const wstring& key, const wstring& path)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	// 레퍼런스(&)이기 때문에 실제 데이터를 수정하는 것입니다.
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];


	// 이미 key가 있으면, 만들어놓은 것이 있으므로, 해당 포인터를 넘겨줍니다.
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	// 만들어놓은 것이 없으면, 파일을 읽어와서 새로운 오브젝트를 생성합니다.
	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

// 임시로 직접 만들어서 등록할 수 있는 함수를 추가해놓은 것입니다.
// 나중에는 Load를 통해서 파일을 저장/불러오는 로직을 사용해야합니다.
template<typename T>
inline bool Resources::Add(const wstring& key, shared_ptr<T> object)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return false;

	keyObjMap[key] = object;

	return true;
}

template<typename T>
inline shared_ptr<T> Resources::Get(const wstring& Key)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(Key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

// 객체를 만드는 개념이 아닌, 클래스 타입만 보고 어떤 타입인지 알 수 있는 함수.
// Object.h 에 있는 GetType()과는 다릅니다. << 이 함수는 실제 객체를 생성해서 멤버변수에 
// 값을 등록해줘야 어떤 타입인지 알 수 있습니다.
template<typename T>
inline OBJECT_TYPE Resources::GetObjectType()
{
	// TODO :: std::is_same_v가 뭔지 알아봅시다.
	// 컴파일 타임에 연산이 됩니다.
	if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::GAMEOBJECT;
	else if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::MATERIAL;
	else if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::MESH;
	else if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::SHADER;
	else if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::TEXTURE;
	else if (std::is_same_v<T, GameObject>)
		return OBJECT_TYPE::COMPONENT;
	else
		return OBJECT_TYPE::NONE;
	return OBJECT_TYPE();
}
