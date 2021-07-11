#pragma once

// [기안서]
// 일을 맡길 때 이런 저런 정보들을 같이 넘겨줘야 하는데,
// 아무 형태로나 요청하면 알아듣지 못한다.
// - 표준 규격, 양식
// - DX11의 View라는 용어로 사용되던 것. DX12에서 DescriptionHeap이 됨.
// - 각종 리소스를 어떤 용도로 사용하는지 꼼꼼하게 적어서 넘겨주는 용도.
// - 일종의 배열. 같은 종류의 데이터끼리 저장하고 관리한다.
// - Wrapping Class
class DescriptorHeap
{
public:
	void Init(ComPtr <ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(int32 idx) { return _rtvHandle[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferView();

private:
	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	uint32 _rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE _rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	shared_ptr<class SwapChain> _swapChain;
};

