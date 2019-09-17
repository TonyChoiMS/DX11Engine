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
// Warning C4316 처리용 //
//////////////////////////
#include "AlignedAllocationPolicy.h"					// 힙에 할당된 개체가 16에 맞추어져 있지 않을 수도 있습니다.