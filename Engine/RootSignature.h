#pragma once

// [계약서 / 결재]
// CPU [        ]     GPU [         ]

// 어떤 정책을 사용할 것인가
// GPU 메모리와 관련된 부분
// GPU 레지스터에 메모리를 어떻게 사용할지 정책을 설명해줘야함.
class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature>	_signature;
};

