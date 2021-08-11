#pragma once

enum class CONSTANT_BUFFER_TYPE : uint8 
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};

// CPU���� GPU�� ������ �����͸� GPU�� ����� ����
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void Clear();
	void PushData(void* buffer, uint32 size);

	void SetGlobalData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource> _cbvBuffer;
	BYTE* _mappedBuffer = nullptr;		// Mesh���� ���ؽ� ���ۿ��� View�� memcpy�� ����
	uint32 _elementSize = 0;			// �� ������ ũ��
	uint32 _elementCount = 0;			// ������ ����

	ComPtr<ID3D12DescriptorHeap> _cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleBegin = {};			// ���� Handle �ּ�
	uint32 _handleIncrementSize = 0;					// ��ĭ�� ��� �Ѿ�� ���� Handle�� �Ѿ �� �ִ���

	uint32 _currentIndex = 0;			// ����ϰ� �ִ� ������ �ε���

	CBV_REGISTER _reg = {};
};

