#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Graphics 클래스 역할 : 모든 그래픽 기능을 이 클래스에 캡슐화됩니다. 
// 이 파일의 헤더를 전체 화면 또는 창 모드와 같이 변경할 수 있는 모든 그래픽 관련 전역 설정에 사용합니다.
// 추후 모든 그래픽 객체가 포함될 클래스
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();
};