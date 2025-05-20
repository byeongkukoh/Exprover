#include "command.h"
#include "mqtt.h"
#include "Resource.h"

extern HWND hLogBox;

void CreateButtonControls(HWND hwnd, HINSTANCE hInstance) {
	// ���� Ű����(��ư) ���� (����Ű + �����̽���)
	CreateWindow(					// ��ư ���� �Լ�
		L"BUTTON",					// Ŭ���� �̸� (BUTTON: ������ �ý��� ���� ��ư ��Ʈ��)
		L"��",						// ��ư ��
		WS_VISIBLE | WS_CHILD,		// ���̰� + �ڽ� ��Ʈ�ѷ� ǥ��
		85, 250, 50, 50,			// ��ġ(x, y), ũ�� (width, height)
		hwnd,						// �θ� ������ �ڵ�
		(HMENU)ID_UP,				// �ν��Ͻ� �ڵ�
		hInstance,					// �ν��Ͻ� �ڵ�
		nullptr						// �߰� ������ (����)
	);

	CreateWindow(
		L"BUTTON", L"��",
		WS_VISIBLE | WS_CHILD,
		25, 310, 50, 50,
		hwnd, (HMENU)ID_LEFT, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"��",
		WS_VISIBLE | WS_CHILD,
		85, 310, 50, 50,
		hwnd, (HMENU)ID_DOWN, hInstance, nullptr
	);

	CreateWindow(
		L"BUTTON", L"��",
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
		wcscat_s(buffer, L"\r\n[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		PublishMQTT("go");
		break;
	}
	case ID_DOWN: {
		wcscat_s(buffer, L"\r\n[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		PublishMQTT("back");
		break;
	}
	case ID_LEFT: {
		wcscat_s(buffer, L"\r\n[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		PublishMQTT("left");
		break;
	}
	case ID_RIGHT: {
		wcscat_s(buffer, L"\r\n[INFO] �� ��ư�� ���Ƚ��ϴ�.");
		PublishMQTT("right");
		break;
	}
	case ID_SPACE: {
		wcscat_s(buffer, L"\r\n[INFO] SPACE ��ư�� ���Ƚ��ϴ�.");
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