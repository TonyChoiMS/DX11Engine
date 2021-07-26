#pragma once

// 키보드 입력을 감지해서 내가 누르고 있거나, 떼고 있거나, 계속 누른 상태인지 판별
// 윈도우 환경 뿐만 아니라 다른 환경에서도 동작 가능할 수 있도록 키 맵핑을 한 뒤 작업 진행.
class Input
{
	void Init(HWND hwnd);
	void Update();

	// 누르고 있을 때
	//bool GetButton(KEY_TYPE type)
};

