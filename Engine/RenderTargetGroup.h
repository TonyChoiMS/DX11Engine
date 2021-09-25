#pragma once
#include "Texture.h"
// 텍스쳐인데, 픽셀 쉐이더에서 해당 데이터를 통해 렌더링을 하길 위한 클래스

enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN,		// Back buffer, front buffer
	G_BUFFER,		//Geometry Buffer Position, Normal, Color
	LIGHTING,		// DIFFUSE LIGHT, SPECULAR LIGHT
	END
};

enum 
{
	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3,
	RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT = 2,
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END)
};

struct RenderTarget
{
	shared_ptr<Texture> target;
	float clearColor[4];
};

class RenderTargetGroup
{
public:
	// dsTexture: 깊이 버퍼. 경우에 따라 이전에 정의한 내용을 계속 사용해야할 경우가 생기기 때문에, 해당 클래스에서 생성하지 않고 외부에서 받아옴.
	void Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture);
	
	void OMSetRenderTargets(uint32 count, uint32 offset);
	void OMSetRenderTargets();

	void ClearRenderTargetView(uint32 index);	
	void ClearRenderTargetView();

	shared_ptr<Texture> GetRTTexture(uint32 index) { return _rtVec[index].target; }
	shared_ptr<Texture> GetDSTexture() { return _dsTexture; }

	void WaitTargetToResource();
	void WaitResourceToTarget();

private:
	RENDER_TARGET_GROUP_TYPE _groupType;
	vector<RenderTarget> _rtVec;
	uint32 _rtCount;
	shared_ptr<Texture> _dsTexture;
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;

private:
	uint32 _rtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE _rtvHeapBegin;
	D3D12_CPU_DESCRIPTOR_HANDLE _dsvHeapBegin;

private:
	D3D12_RESOURCE_BARRIER _targetToResource[8];
	D3D12_RESOURCE_BARRIER _resourceToTarget[8];
};

