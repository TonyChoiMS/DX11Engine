#pragma once
class DepthStencilBuffer
{
public:
	// format은 버퍼를 만들어줄 때 깊이 용도로 할것인지, 깊이 + 스텐실 용도로 사용할 것인지에 대한 옵션
	void Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT GetDSVFormat() { return _dsvFormat; }

private:
	// Depth Stencil View
	// Swapchain에서 RTV를 만들어줄 때는 2개를 만들어서 서로 교체하며 렌더링을 해줬지만, 
	// DSV는 하나만 만들어줘도 됩니다.
	ComPtr<ID3D12Resource> _dsvBuffer;
	ComPtr<ID3D12DescriptorHeap> _dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _dsvHandle = {};
	DXGI_FORMAT _dsvFormat = {};
};

