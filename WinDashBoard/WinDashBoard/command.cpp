#include "command.h"
#include "mqtt.h"
#include "Resource.h"

extern HWND hListBox;

void CreateButtonControls(HWND hwnd, HINSTANCE hInstance) {
	// 가상 키보드(버튼) 생성 (방향키 + 스페이스바)
	CreateWindow(					// 버튼 생성 함수
		L"BUTTON",					// 클래스 이름 (BUTTON: 윈도우 시스템 내장 버튼 컨트롤)
		L"↑",						// 버튼 라벨
		WS_VISIBLE | WS_CHILD,		// 보이게 + 자식 컨트롤로 표시
		85, 410, 50, 50,			// 위치(x, y), 크기 (width, height)
		hwnd,						// 부모 윈도우 핸들
		(HMENU)ID_UP,				// 인스턴스 핸들
		hInstance,					// 인스턴스 핸들
		nullptr						// 추가 데이터 (없음)
	);

	CreateWindow(
		L"BUTTON", L"←",
		WS_VISIBLE | WS_CHILD,
		25, 470, 50, 50,
		hwnd, (HMENU)ID_LEFT, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"↓",
		WS_VISIBLE | WS_CHILD,
		85, 470, 50, 50,
		hwnd, (HMENU)ID_DOWN, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"→",
		WS_VISIBLE | WS_CHILD,
		145, 470, 50, 50,
		hwnd, (HMENU)ID_RIGHT, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"Space",
		WS_VISIBLE | WS_CHILD,
		205, 470, 200, 50,
		hwnd, (HMENU)ID_SPACE, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"Ctrl",
		WS_VISIBLE | WS_CHILD,
		415, 470, 110, 50,
		hwnd, (HMENU)ID_SPACE, hInstance, nullptr
	);
}

void HandleButtonCommand(HWND hwnd, WPARAM wParam) {
	wchar_t buffer[1024] = L"";
	GetWindowText(hListBox, buffer, 1024);

	switch (LOWORD(wParam)) {
	case ID_UP:
		AddLogMsg(L"[INFO] ↑ 버튼이 눌렸습니다.");
		AsyncPublish("go");
		break;
	case ID_DOWN:
		AddLogMsg(L"[INFO] ↓ 버튼이 눌렸습니다.");
		AsyncPublish("back");
		break;
	case ID_LEFT:
		AddLogMsg(L"[INFO] ← 버튼이 눌렸습니다.");
		AsyncPublish("left");
		break;
	case ID_RIGHT:
		AddLogMsg(L"[INFO] → 버튼이 눌렸습니다.");
		AsyncPublish("right");
		break;
	case ID_SPACE:
		AddLogMsg(L"[INFO] SPACE 버튼이 눌렸습니다.");
		AsyncPublish("stop");
		break;
	}
}

void HandleKeyDown(HWND hwnd, WPARAM wParam) {
	switch (wParam) {
	case VK_UP:
		HandleButtonCommand(hwnd, ID_UP);
		break;
	case VK_DOWN:
		HandleButtonCommand(hwnd, ID_DOWN);
		break;
	case VK_LEFT:
		HandleButtonCommand(hwnd, ID_LEFT);
		break;
	case VK_RIGHT:
		HandleButtonCommand(hwnd, ID_RIGHT);
		break;
	case VK_SPACE:
		HandleButtonCommand(hwnd, ID_SPACE);
		break;
	}
}

void AddLogMsg(const wchar_t* message) {
	SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)message);

	// 마지막 줄로 이동
	int count = (int)SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	SendMessage(hListBox, LB_SETTOPINDEX, count - 1, 0);
}