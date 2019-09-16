#pragma once

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Graphics Ŭ���� ���� : ��� �׷��� ����� �� Ŭ������ ĸ��ȭ�˴ϴ�. 
// �� ������ ����� ��ü ȭ�� �Ǵ� â ���� ���� ������ �� �ִ� ��� �׷��� ���� ���� ������ ����մϴ�.
// ���� ��� �׷��� ��ü�� ���Ե� Ŭ����
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