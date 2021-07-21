#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();
shared_ptr<Texture> texture = make_shared<Texture>();

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.f, 0.f);
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.f, 0.f);
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.f, 1.f);
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.f, 1.f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	// 삼각형은 두개 그려지지만 메쉬는 하나만 존재함.
	// 같은 메쉬를 사용하는게 천개 있어도 메쉬정보는 하나만 존재.(공통적인 메쉬를 이용)
	mesh->Init(vec, indexVec);
	
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	texture->Init(L"..\\Resources\\Texture\\images.jpg");

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	shader->Update();

	{
		Transform t;
		t.offset = Vec4(0.f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->SetTexture(texture);

		mesh->Render();
	}

	GEngine->RenderEnd();
}
