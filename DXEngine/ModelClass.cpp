#include "stdafx.h"
#include "ModelClass.h"


ModelClass::ModelClass()
{
}

ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device)
{
	return false;
}

void ModelClass::Shutdown()
{
}

void ModelClass::Render(ID3D11DeviceContext *)
{
}

int ModelClass::GetIndexCount()
{
	return 0;
}

bool ModelClass::InitializeBuffers(ID3D11Device *)
{
	return false;
}

void ModelClass::ShutdownBuffers()
{
}

void ModelClass::RenderBuffers(ID3D11DeviceContext *)
{
}
