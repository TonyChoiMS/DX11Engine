#pragma once

// 딱 한번만 로딩해서 그 텍스쳐를 계속 사용한다.
class Texture
{
public:
	void Init(const wstring& path);

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

