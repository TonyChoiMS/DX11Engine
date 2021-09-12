#pragma once
#include "Object.h"

// �� �ѹ��� �ε��ؼ� �� �ؽ��ĸ� ��� ����Ѵ�.
class Texture : public Object
{
public:
	Texture();
	virtual ~Texture();
	virtual void Load(const wstring& path) override;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }

public:
	void CreateTexture(const wstring& path);
	void CreateView();

private:
	ScratchImage _image;
	ComPtr<ID3D12Resource> _tex2D;

	ComPtr<ID3D12DescriptorHeap> _srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _srvHandle;
};

