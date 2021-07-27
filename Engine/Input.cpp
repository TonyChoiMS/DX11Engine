#include "pch.h"
#include "Input.h"

void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

// 동일한 프레임 안에서는 키 상태가 일정하다고 가정해야하기 때문에 매 프레임 Update를 호출합니다.
void Input::Update()
{
	HWND hwnd = GetActiveWindow();
	// 현재 활성화된 윈도우 창이 이 창이 아니면 눌려있는 키의 입력 상태를 모두 none으로 초기화
	// 이 엔진이 실행되고 있을 때만 키 입력을 받게 합니다.
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
		{
			_states[key] = KEY_STATE::NONE;
		}

		return;
	}

	BYTE asciiKeys[KEY_TYPE_COUNT] = {};

	//255개의 가상키보드 키 상태를 받아온다.
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// window API 키 체크하는 함수.
		// 키가 눌려 있으면 true
		//if (::GetAsyncKeyState(key) & 0x8000)
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
				state = KEY_STATE::DOWN;
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}
}
