#include "command.h"
#include "mqtt.h"
#include "Resource.h"

extern HWND hListBox;

void CreateButtonControls(HWND hwnd, HINSTANCE hInstance) {
	// ���� Ű����(��ư) ���� (����Ű + �����̽���)
	CreateWindow(					// ��ư ���� �Լ�
		L"BUTTON",					// Ŭ���� �̸� (BUTTON: ������ �ý��� ���� ��ư ��Ʈ��)
		L"��",						// ��ư ��
		WS_VISIBLE | WS_CHILD,		// ���̰� + �ڽ� ��Ʈ�ѷ� ǥ��
		85, 410, 50, 50,			// ��ġ(x, y), ũ�� (width, height)
		hwnd,						// �θ� ������ �ڵ�
		(HMENU)ID_UP,				// �ν��Ͻ� �ڵ�
		hInstance,					// �ν��Ͻ� �ڵ�
		nullptr						// �߰� ������ (����)
	);

	CreateWindow(
		L"BUTTON", L"��",
		WS_VISIBLE | WS_CHILD,
		25, 470, 50, 50,
		hwnd, (HMENU)ID_LEFT, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"��",
		WS_VISIBLE | WS_CHILD,
		85, 470, 50, 50,
		hwnd, (HMENU)ID_DOWN, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"��",
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
		AddLogMsg(L"[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		AsyncPublish("go");
		break;
	case ID_DOWN:
		AddLogMsg(L"[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		AsyncPublish("back");
		break;
	case ID_LEFT:
		AddLogMsg(L"[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		AsyncPublish("left");
		break;
	case ID_RIGHT:
		AddLogMsg(L"[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		AsyncPublish("right");
		break;
	case ID_SPACE:
		AddLogMsg(L"[INFO] SPACE ��ư�� ���Ƚ��ϴ�.");
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

	// ������ �ٷ� �̵�
	int count = (int)SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	SendMessage(hListBox, LB_SETTOPINDEX, count - 1, 0);
}