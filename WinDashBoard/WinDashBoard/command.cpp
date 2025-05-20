#include "command.h"
#include "mqtt.h"
#include "Resource.h"

extern HWND hLogBox;

void CreateButtonControls(HWND hwnd, HINSTANCE hInstance) {
	// 가상 키보드(버튼) 생성 (방향키 + 스페이스바)
	CreateWindow(					// 버튼 생성 함수
		L"BUTTON",					// 클래스 이름 (BUTTON: 윈도우 시스템 내장 버튼 컨트롤)
		L"↑",						// 버튼 라벨
		WS_VISIBLE | WS_CHILD,		// 보이게 + 자식 컨트롤로 표시
		85, 250, 50, 50,			// 위치(x, y), 크기 (width, height)
		hwnd,						// 부모 윈도우 핸들
		(HMENU)ID_UP,				// 인스턴스 핸들
		hInstance,					// 인스턴스 핸들
		nullptr						// 추가 데이터 (없음)
	);

	CreateWindow(
		L"BUTTON", L"←",
		WS_VISIBLE | WS_CHILD,
		25, 310, 50, 50,
		hwnd, (HMENU)ID_LEFT, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"↓",
		WS_VISIBLE | WS_CHILD,
		85, 310, 50, 50,
		hwnd, (HMENU)ID_DOWN, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"→",
		WS_VISIBLE | WS_CHILD,
		145, 310, 50, 50,
		hwnd, (HMENU)ID_RIGHT, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"Space",
		WS_VISIBLE | WS_CHILD,
		205, 310, 200, 50,
		hwnd, (HMENU)ID_SPACE, hInstance, nullptr
	);
}

void HandleButtonCommand(HWND hwnd, WPARAM wParam) {
	wchar_t buffer[1024] = L"";
	GetWindowText(hLogBox, buffer, 1024);

	switch (LOWORD(wParam)) {
	case ID_UP: {
		wcscat_s(buffer, L"\r\n[INFO] ↑ 버튼이 눌렸습니다.");
		PublishMQTT("go");
		break;
	}
	case ID_DOWN: {
		wcscat_s(buffer, L"\r\n[INFO] ↓ 버튼이 눌렸습니다.");
		PublishMQTT("back");
		break;
	}
	case ID_LEFT: {
		wcscat_s(buffer, L"\r\n[INFO] ← 버튼이 눌렸습니다.");
		PublishMQTT("left");
		break;
	}
	case ID_RIGHT: {
		wcscat_s(buffer, L"\r\n[INFO] → 버튼이 눌렸습니다.");
		PublishMQTT("right");
		break;
	}
	case ID_SPACE: {
		wcscat_s(buffer, L"\r\n[INFO] SPACE 버튼이 눌렸습니다.");
		PublishMQTT("stop");
		break;
	}
	}

	SetWindowText(hLogBox, buffer);
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