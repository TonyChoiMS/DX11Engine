#pragma once

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

/////////////
// INCLUDE //
/////////////
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

//////////////////////////
// Warning C4316 ó���� //
//////////////////////////
#include "AlignedAllocationPolicy.h"					// ���� �Ҵ�� ��ü�� 16�� ���߾��� ���� ���� ���� �ֽ��ϴ�.