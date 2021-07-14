#pragma once

// [��༭ / ����]
// CPU [        ]     GPU [         ]

// � ��å�� ����� ���ΰ�
// GPU �޸𸮿� ���õ� �κ�
class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature>	_signature;
};
