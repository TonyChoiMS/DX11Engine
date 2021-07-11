#pragma once

// ��ȯ �罽
// [CPU�� GPU���� ���� �ñ�� ����]
// - ���� ���� ���� �ִ� ��Ȳ�� ����
// - � �������� ��� ������� ������
// - GPU�� ������ ���
// - ����� �޾Ƽ� ȭ�鿡 �׷���.

// [�����]
// - � ����(Buffer)�� �׷��� �ǳ��޶�� ��Ź�غ���.
// - Ư�� ���̸� ���� -> ó���� �ǳ��ְ� -> ������� �ش� ���̿� �޴´� OK
// - �츮 ȭ�鿡 Ư�� ����(���� �����) ������ش�.
// - Ư�� ���̸� �ϳ��� ���� �Ǹ� ȭ�鿡 ���̰� �Ǵ� ������ �Ͼ��.(�̰� ����� �ϴ���..)

// [Double Buffering]
// - �׷��� ȭ�鿡 ���� ����� ����ϴ� ���߿�, ���� ȭ�鵵 ���ָ� �ðܾ� ��.
// - ���� ȭ�� ������� �̹� ȭ�� ��¿� �����
// - Ư�� ���̸� 2�� ���� �ϳ��� ���� ȭ���� �׷��ְ�, �ϳ��� ���� �ñ��

// [1] [2]
// ���� ȭ�� [1] <-> GPU �۾��� [2] BackBuffer
// �� �̰��� SwapChain �� �ϴ� ��.
// - Buffer �� �� 2���� ����ؾ� �ϴ� ���� �ƴ�����, 2���� ����ص� ����ϱ� ������ ���� 2���� �����.
class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }
	ComPtr<ID3D12Resource> GetRenderTarget(int32 Index) { return _renderTargets[Index]; }

	uint32 GetCurrentBackBufferIndex() { return _backBufferIndex; }
	ComPtr<ID3D12Resource> GetCurretnBackBufferResource() { return _renderTargets[_backBufferIndex]; }

private:
	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D12Resource> _renderTargets[SWAP_CHAIN_BUFFER_COUNT];		// ���� Buffer�� �����س��� ��.
	uint32 _backBufferIndex = 0;			// ���� ����۸� �����ϱ� ���� ����.

};

