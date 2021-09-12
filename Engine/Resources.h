#pragma once

#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

// ���ӿ�����Ʈ, �޽� ������, ���̴�, �ؽ��� �� ��� ���ҽ��� �����ϴ� �Ŵ���.
class Resources
{
	DECLARE_SINGLE(Resources);

public:
	void Init();

	// ���ø����� ����ϴ� ���� : Resources���� �پ��� Ŭ������ ����ֱ� ������,
	// �װ͵��� �������� ��ɵ��� ��� �ƿ﷯�� ó���ϱ� ����.
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
	// ���ҽ� ���� �����ϴ� Map�� ���� ����� ���� array�� ����.
	array<KeyObjMap, OBJECT_TYPE_COUNT> _resources;
};


// ���ø� ���� ������ �Ϲ������� ������� ���ο��� ��� �ۼ��ϸ�,
// ��������� �ʹ� ����� ��� inl���Ϸ� �и��ؼ� ������ ��Ŭ��� �ϴ� �������
// ����ϱ⵵ �մϴ�.
template<typename T>
inline shared_ptr<T> Resources::Load(const wstring& key, const wstring& path)
{
	OBJECT_TYPE objectType = GetObjectType<T>();
	// ���۷���(&)�̱� ������ ���� �����͸� �����ϴ� ���Դϴ�.
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];


	// �̹� key�� ������, �������� ���� �����Ƿ�, �ش� �����͸� �Ѱ��ݴϴ�.
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	// �������� ���� ������, ������ �о�ͼ� ���ο� ������Ʈ�� �����մϴ�.
	shared_ptr<T> object = make_shared<T>();
	object->Load(path);
	keyObjMap[key] = object;

	return object;
}

// �ӽ÷� ���� ���� ����� �� �ִ� �Լ��� �߰��س��� ���Դϴ�.
// ���߿��� Load�� ���ؼ� ������ ����/�ҷ����� ������ ����ؾ��մϴ�.
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

// ��ü�� ����� ������ �ƴ�, Ŭ���� Ÿ�Ը� ���� � Ÿ������ �� �� �ִ� �Լ�.
// Object.h �� �ִ� GetType()���� �ٸ��ϴ�. << �� �Լ��� ���� ��ü�� �����ؼ� ��������� 
// ���� �������� � Ÿ������ �� �� �ֽ��ϴ�.
template<typename T>
inline OBJECT_TYPE Resources::GetObjectType()
{
	// TODO :: std::is_same_v�� ���� �˾ƺ��ô�.
	// ������ Ÿ�ӿ� ������ �˴ϴ�.
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
