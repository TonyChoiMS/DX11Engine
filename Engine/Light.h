#pragma once
#include "Component.h"

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightColor
{
	Vec4 diffuse;
	Vec4 ambient;
	Vec4 specular;
};

struct LightInfo
{
	LightColor color;
	Vec4 position;			// Directional Light�� position�� �ǹ� �����ϴ�.
	Vec4 direction;			// Point LIght�� ū �ǹ� �����ϴ�.
	int32 lightType;		// shader�� padding�� ���߱� ���� enum�� int8�̾ ����� int32�� ����.
	float range;			// ���� �ִ� ����
	float angle;			// ���� ���ߴ� ����
	int32 padding;			// PackingRule!!! � ����ü�� ���鶧 ����ü�� ���������� �޸𸮿� �ö󰡴� �޸� ���� ����. LightInfo �������� ����� 16����� ���߱� ���ؼ� ���.
};

// ���̴��� �Ѱ��� �� ������
// ����Ʈ�� ��ü���� �Ź� ��ü�ؼ� �۾��ϴ°� �ƴ϶�, �����Ӵ� �� �ѹ��� ��� ���� ���� ������ 
// �����ؼ� ����մϴ�.
struct LightParams
{
	uint32 lightCount;
	Vec3 padding;
	LightInfo lights[50];			// Material�� ���� �ϳ��� ���̴��� �Ѱ�����, ����Ʈ�� �ѹ��� ��� �ѱ�ϴ�.
};
// 8:00
class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;
	void Render();
	void RenderShadow();

public:
	LIGHT_TYPE GetLightType() { return static_cast<LIGHT_TYPE>(_lightInfo.lightType); }
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(Vec3 direction);
	
	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type);
	void SetLightRange(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

	void SetLightIndex(int8 index) { _lightIndex = index; }

private:
	LightInfo _lightInfo = {};

	int8 _lightIndex = -1;
	shared_ptr<class Mesh> _volumeMesh;
	shared_ptr<class Material> _lightMaterial;

	shared_ptr<GameObject> _shadowCamera;
};

