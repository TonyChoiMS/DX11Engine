#pragma once

// 교환 사슬
// [CPU가 GPU에게 일을 맡기는 과정]
// - 현재 게임 세상에 있는 상황을 묘사
// - 어떤 공식으로 어떻게 계산할지 던져줌
// - GPU가 열심히 계산
// - 결과물 받아서 화면에 그려줌.

// [결과물]
// - 어떤 종이(Buffer)에 그려서 건내달라고 부탁해보자.
// - 특수 종이를 만들어서 -> 처음에 건내주고 -> 결과물을 해당 종이에 받는다 OK
// - 우리 화면에 특수 종이(외주 결과물) 출력해준다.
// - 특수 종이를 하나만 쓰게 되면 화면에 섞이게 되는 현상이 일어난다.(이걸 뭐라고 하더라..)

// [Double Buffering]
// - 그러넫 화면에 현재 결과물 출력하는 와중에, 다음 화면도 외주를 맡겨야 함.
// - 현재 화면 결과물은 이미 화면 출력에 사용중
// - 특수 종이를 2개 만들어서 하나는 현재 화면을 그려주고, 하나는 외주 맡기고

// [1] [2]
// 현재 화면 [1] <-> GPU 작업중 [2] BackBuffer
// ※ 이것이 SwapChain 이 하는 일.
// - Buffer 를 꼭 2개를 사용해야 하는 것은 아니지만, 2개를 사용해도 충분하기 때문에 보통 2개를 사용함.
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
	ComPtr<ID3D12Resource> _renderTargets[SWAP_CHAIN_BUFFER_COUNT];		// 만든 Buffer를 저장해놓을 곳.
	uint32 _backBufferIndex = 0;			// 현재 백버퍼를 추적하기 위한 변수.

};

