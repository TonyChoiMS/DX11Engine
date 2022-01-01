#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	COMPUTE,
	SHADOW,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,		// 무시하는 것 없이 모든 오브젝트를 렌더링합니다.
	CULL_FRONT,		// 시계방향인 오브젝트를 무시한다.
	CULL_BACK,		// 반시계 방향인 오브젝트를 무시한다.
	WIREFRAME,
};

// 깊이 테스트와 관련된 내용
// 깊이 비교 공식을 판별하는 옵션입니다.
// 라이팅과 관련된 연산을 할 떄는 DepthBuffer를 이용해서 어떤 물체를 스킵하면 안됨.
enum class DEPTH_STENCIL_TYPE : uint8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_DEPTH_TEST,				// 깊이 테스트(X) + 깊이 기록(O)
	NO_DEPTH_TEST_NO_WRITE,		// 깊이 테스트(X) + 깊이 기록(X)
	LESS_NO_WRITE,				// 깊이 테스트(O) + 깊이 기록(X)
};

// 픽셀 쉐이더에서 계산한 결과물과 렌더타켓에 있는 결과물을 어떻게 둘을 혼합할지 결정
enum class BLEND_TYPE : uint8
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_TO_ONE_BLEND,
	END,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

// [일감 기술서] 외주 인력들이 뭘 해야할지 기술
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void CreateGraphicsShader(const wstring& path, ShaderInfo info = ShaderInfo(), const string& vs = "VS_Main", const string& ps = "PS_Main", const string& gs ="");
	void CreateComputeShader(const wstring& path, const string& name, const string& version);

	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);
	void CreateGeometryShader(const wstring& path, const string& name, const string& version);
	
private:
	ShaderInfo _info;
	ComPtr<ID3D12PipelineState>			_pipelineState;

	// GraphicsShader
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_errBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _graphicsPipelineDesc = {};

	// ComputeShader
	ComPtr<ID3DBlob> _csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC _computePipelineDesc = {};
};

