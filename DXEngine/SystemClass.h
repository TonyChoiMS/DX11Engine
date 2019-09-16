#pragma once


// 전방 선언을 통해 빠른 컴파일을 작성할 수 있음.
class InputClass;
class GraphicsClass;

// WinAPI 윈도우 프로그래밍을 클래스화 한 것.
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);		// 응용프로그램이 실행되는 동안 응용 프로그램에 전송될 Windows 시스템 메시지를 처리

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input = nullptr;
	GraphicsClass* m_Graphics = nullptr;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;