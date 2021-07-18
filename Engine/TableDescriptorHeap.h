#pragma once
class TableDescriptorHeap
{
public:
	void Init(uint32 count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32 reg);

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap;			// CreateDescriptorHeap�� ���� �����ߴ� Heap
	// 
	uint64 _handleSize = 0;			// == handleIncrementSize 
	uint64 _groupSize = 0;			// handleSize�ε� ���� �� ������ ���� �������� ���� �ϳ� �� ����.
	uint64 _groupCount = 0;			// table�� �ϳ��� �ƴ϶� ��鰳�� �� �� �����Ƿ� 

	uint32 _currentGroupIndex = 0;
};
