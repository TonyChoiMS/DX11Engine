#pragma once

// ************************
// GraphicsDescriptorHeap
// ************************

class GraphicsDescriptorHeap
{
public:
	void Init(uint32 count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap;			// CreateDescriptorHeap을 통해 생성했던 Heap
	// 
	uint64 _handleSize = 0;			// == handleIncrementSize 
	uint64 _groupSize = 0;			// handleSize로도 구할 수 있으나 관리 차원에서 변수 하나 더 정의.
	uint64 _groupCount = 0;			// table을 하나가 아니라 몇백개가 될 수 있으므로 

	uint32 _currentGroupIndex = 0;
};

// ************************
// ComputeDescriptorHeap
// ************************

class ComputeDescriptorHeap
{
public:
	void Init();

	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
	void SetUAV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, UAV_REGISTER reg);

	void CommitTable();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(SRV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(UAV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap;
	uint64 _handleSize = 0;
};
