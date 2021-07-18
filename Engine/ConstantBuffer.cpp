#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
		{
			_cbvBuffer->Unmap(0, nullptr);
		}

		_cbvBuffer = nullptr;
	}
}

void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다.
	// 0 256 512 768
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer)
	);

	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
}

void ConstantBuffer::CreateView()
{
	// DescriptorHeap 생성
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = _elementCount;			// 버퍼를 만들 때 _elementCount만큼 만들었으므로 똑같이 생성.
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap));

	// 직접 생성하는 것이 아닌 생성된 것을 가져오도록.
	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	// GPU 사양마다 조금씩 달라지기 때문에 꼭 GetDescriptorHandleIncrementSize를 통해서 size를 확인해야함.
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (uint32 i = 0; i < _elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		// 버퍼의 주소
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		// 버퍼의 크기
		cbvDesc.SizeInBytes = _elementSize;		// CB size if required to be 256-byte aligned

		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementSize);

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAdress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAdress += index * _elementSize;
	return objCBAdress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	// _cpuHandleBegin에다가 뒤에 숫자를 더하는 로직. 
	// ptr += offsetDesciptors * desciptorIncrementSize
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}
