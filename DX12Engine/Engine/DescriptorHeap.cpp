#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h"

void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	// Descriptor (DX12) = View (-DX11)
	// DX11은 리소스마다 View가 따로 있었으나, DX12는 Descriptor 하나로 묘사하고 관리한다.
	// [서술자 힙] 으로 RTV 생성
	// DX11의 RTV(RenderTargetView), DSV(DepthStencilView),
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부!

	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	// 같은 종류의 데이터끼리 배열로 관리
	// RTV 목록 : [ ] [ ] 
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));
	
	// 정수 형태 주소값.(포인터 아님. size_t)
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (INT i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// CD3DX12 는 d3dx12.h 헬퍼 클래스에 있는 함수.
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		// _rtvHandle[i].ptr = rtvHeapBegin.ptr + (i *_rtvHeapSize);
		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
	}
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView()
{
	return GetRTV(_swapChain->GetCurrentBackBufferIndex());
}
