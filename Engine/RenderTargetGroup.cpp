#include "pch.h"
#include "RenderTargetGroup.h"
#include "Engine.h"
#include "Device.h"

void RenderTargetGroup::Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture)
{
	_groupType = groupType;
	_rtVec = rtVec;
	_rtCount = static_cast<uint32>(rtVec.size());
	_dsTexture = dsTexture;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = _rtCount;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NodeMask = 0;

	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_rtvHeap));

	_rtvHeapSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	_dsvHeapBegin = _dsTexture->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	for (uint32 i = 0; i < _rtCount; i++)
	{
		uint32 desctSize = 1;
		D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, i * _rtvHeapSize);

		uint32 srcSize = 1;
		ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = _rtVec[i].target->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

		DEVICE->CopyDescriptors(1, &destHandle, &desctSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	for (int i = 0; i < _rtCount; ++i)
	{
		_targetToResource[i] = CD3DX12_RESOURCE_BARRIER::Transition(_rtVec[i].target->GetTex2D().Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COMMON);

		_resourceToTarget[i] = CD3DX12_RESOURCE_BARRIER::Transition(_rtVec[i].target->GetTex2D().Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void RenderTargetGroup::OMSetRenderTargets(uint32 count, uint32 offset)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, offset * _rtvHeapSize);
	GRAPHICS_CMD_LIST->OMSetRenderTargets(count, &rtvHandle, FALSE/*1��*/, &_dsvHeapBegin);
}

void RenderTargetGroup::OMSetRenderTargets()
{
	GRAPHICS_CMD_LIST->OMSetRenderTargets(_rtCount, &_rtvHeapBegin, TRUE/*����*/, &_dsvHeapBegin);
}

void RenderTargetGroup::ClearRenderTargetView(uint32 index)
{
	// Specify the buffers we are going to render to.
	// ����۸� ������ ������, �ű⿡ ������� GPU���� ���� �ϸ� �ȴٰ� �˷���.
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, index * _rtvHeapSize);
	assert(this != nullptr);
	assert(_rtvHeap != nullptr);
	assert(rtvHandle.ptr != NULL);
	GRAPHICS_CMD_LIST->ClearRenderTargetView(rtvHandle, _rtVec[index].clearColor, 0, nullptr);

	GRAPHICS_CMD_LIST->ClearDepthStencilView(_dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void RenderTargetGroup::ClearRenderTargetView()
{
	WaitResourceToTarget();

	// depth buffer�� 1.0���� �ʱ�ȭ.
	// Deferred Group �ʱ�ȭ
	for (uint32 i = 0; i < _rtCount; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvHeapBegin, i * _rtvHeapSize);
		assert(this != nullptr);
		GRAPHICS_CMD_LIST->ClearRenderTargetView(rtvHandle, _rtVec[i].clearColor, 0, nullptr);
	}

	GRAPHICS_CMD_LIST->ClearDepthStencilView(_dsvHeapBegin, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
}

void RenderTargetGroup::WaitTargetToResource()
{
	GRAPHICS_CMD_LIST->ResourceBarrier(_rtCount, _targetToResource);
}

void RenderTargetGroup::WaitResourceToTarget()
{
	GRAPHICS_CMD_LIST->ResourceBarrier(_rtCount, _resourceToTarget);
}
