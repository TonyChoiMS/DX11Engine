#pragma once
#include "Component.h"

class Mesh;
class Material;

// [32][32]
union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 marterialID;
	};
	uint64 id;
};

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer();

	shared_ptr<Material> GetMaterial() { return _material; }

	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	void Render();
	void Render(shared_ptr<class InstancingBuffer>& buffer);
	void RenderShadow();

	uint64 GetInstanceId();

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;
};

