#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	// 객체를 new를 했으면 delete를 해주는 것과 같이, 이벤트를 등록했으면 해제를 해줘야함.
	// 필수는 아니지만 깔끔한 동작을 위해 작업하는 것이 좋음.
	::CloseHandle(_fenceEvent);
}

// 디바이스를 이용해서 커맨드큐를 만들기 때문에 디바이스는 필수적.
void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// 커맨드큐 생성
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator
	// 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_cmdList->Close();

	// 리소스 로드용
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));

	// Event => 신호등 느낌.
	// 빨간불인 상태로 생성한 뒤, 파란불이 될 때까지 대기한다.
	// 파란불이 됬을 때는 그 때 원하는 동작을 수행하도록 한다.
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// 책에 있는 코드 그대로 적용.
	// 상태에 번호를 매김.
	// CPU가 놀고 있을 수 있기 때문에 좋은 코드라고 볼 순 없음. 개선 필요.
	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	// 현재 버퍼와 백버퍼를 교체하는 일을 예약
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 결과물

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	GEngine->GetCB()->Clear();
	GEngine->GetTableDescHeap()->Clear();

	// SetDescriptorHeaps 함수는 매우 무거운 함수이므로, 프레임마다 한번씩만 호출되게 한다.
	// 어떤 DescriptorHeap을 사용할 것인지 정한다.
	ID3D12DescriptorHeap* descHeap =  GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	// 백버퍼를 꺼내온 다음에, 거기에 대상으로 GPU에게 일을 하면 된다고 알려줌.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);


	// depth buffer를 1.0으로 초기화.
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle();
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);		// OutputMergeState
	_cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void CommandQueue::RenderEnd()
{
	// 렌더가 끝났으므로 백버퍼를 다시 렌더링 용으로 바꿔치기함.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	// 커맨드 리스트 수행
	// 실제로 수행을 하는 곳. Begin은 큐에 넣는것 까지만 하는것.
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present();

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync();

	_swapChain->SwapIndex();
}

// 이 함수가 호출될 때 리소스 로드가 시작됨.
void CommandQueue::FlushResourceCommandQueue()
{
	_resCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}