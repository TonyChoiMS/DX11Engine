#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

// constant buffer : hlsl bindslot => b로 시작

void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();
}

void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

}

void RootSignature::CreateRootSignature()
{
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0),		// b0~b4
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0),		// t0~t4
	};
	CD3DX12_ROOT_PARAMETER param[1];

	// RootParam에 에 RootDescriptor(ConstantBuffer) 자체를 넣는 방법.
	// 64 DWORD 한계.
	//param[0].InitAsConstantBufferView(0);		// 0번 -> b0 -> CBV, 버퍼에 있는 어떤 View를 가리키고 있는지 알려주는 일종의 포인터
	//param[1].InitAsConstantBufferView(1);		// 1번 -> b1 -> CBV

	// RootTable을 이용한 방법. (DescriptorHeap)
	// 복잡하긴 하지만, 용량의 제한에 관계없이 원하는 View를 많이 정의해 놓을 수 있다.
	// RootTable을 On/Off시킴에따라 활성화되는 View가 달라져서 
	param[0].InitAsDescriptorTable(_countof(ranges), ranges);

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}
