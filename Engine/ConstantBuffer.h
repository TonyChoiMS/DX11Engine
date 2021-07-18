#pragma once
// CPU에서 GPU로 전송한 데이터를 GPU에 저장된 버퍼
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource> _cbvBuffer;
	BYTE* _mappedBuffer = nullptr;		// Mesh에서 버텍스 버퍼에서 View로 memcpy한 버퍼
	uint32 _elementSize = 0;			// 각 버퍼의 크기
	uint32 _elementCount = 0;			// 버퍼의 갯수

	ComPtr<ID3D12DescriptorHeap> _cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleBegin = {};			// 시작 Handle 주소
	uint32 _handleIncrementSize = 0;					// 몇칸을 띄워 넘어야 다음 Handle로 넘어갈 수 있는지

	uint32 _currentIndex = 0;			// 사용하고 있는 버퍼의 인덱스
};

