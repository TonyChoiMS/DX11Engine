#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return	_cmdList; }

private:
	// �� 3���� �ٽ�.
	// CommandQueue : DX12�� ����
	// GPU�� ���� ��û�� �� �ϳ��� ��û�ϸ� ��ȿ����
	// [Command ���]�� �ϰ��� �������� ����ߴٰ� �ѹ��� ��û�ϴ� ��.
	ComPtr<ID3D12CommandQueue>			_cmdQueue;		// �ϰ� ���	
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;		// �ϰ� ��ü�� �־��ֱ� ���� �޸� ������ �Ҵ�
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;		// �ϰ� ����Ʈ

	// Fence : ��Ÿ��(?)
	// CPU / GPU ����ȭ�� ���� ������ ����
	// �־��� ���� ���� �� �� ���� �� ���� ������ ��ٸ��� �ൿ�� �� �� �ְ� ���ִ� �������̽�
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};
