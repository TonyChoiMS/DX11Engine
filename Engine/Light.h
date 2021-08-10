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
	int32 padding;			// LightInfo �������� ����� 16����� ���߱� ���ؼ� ���.
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

	virtual void FinalUPdate() override;

public:
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }
	
	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type) { _lightInfo.lightType = static_cast<int32>(type); }
	void SetLightRange(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

private:
	LightInfo _lightInfo = {};
};

