#include <string>
#include <vector>

#include "gui.h"
#include "command.h"
#include "mqtt_video_receiver.h"


HWND hListBox;
HFONT hFont;	// ���� ��Ʈ �ڵ� ����

HWND hPictureBox;					// ���� ��¿� PictureBox
HBITMAP hCurrentBitmap = nullptr;	// ���� ǥ�� ���� ��Ʈ��

PROCESS_INFORMATION g_pyProcess = {};
bool g_isRunning = false;

int InitGUI(HINSTANCE hInstance, int nCmdShow) {
	// 1. ������ Ŭ���� �̸� ���� (�ĺ���)
	const wchar_t CLASS_NAME[] = L"DASHBOARD";

	// 2. ������ Ŭ���� ����ü �ʱ�ȭ
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;						// �޽��� ó�� �Լ� ���
	wc.hInstance = hInstance;						// ���� ���� ���� �ν��Ͻ� �ڵ�
	wc.lpszClassName = CLASS_NAME;					// Ŭ���� �̸� ����
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// ���� ���� (���)

	// 3. Ŭ���� ���
	RegisterClass(&wc);

	// 4. ������ ����
	HWND hwnd = CreateWindowEx(
		0,											// Ȯ�� ��Ÿ�� (����)
		CLASS_NAME,									// �տ��� ����� Ŭ���� �̸�
		L"MQTT DASHBOARD",							// â ����
		WS_OVERLAPPEDWINDOW,						// �⺻ ������ ��Ÿ��
		CW_USEDEFAULT, CW_USEDEFAULT, 1080, 570,		// ��ġ(x, y), ũ�� (width, height)
		nullptr, nullptr, hInstance, nullptr		// �θ�, �޴� ����
	);

	// ���� â ����
	hPictureBox = CreateWindow(
		L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD | SS_BITMAP | WS_BORDER,
		25, 25, 500, 375,  // ��ǥ �� ũ�� ���� ����
		hwnd, nullptr, hInstance, nullptr
	);

	// ��ư ����
	CreateButtonControls(hwnd, hInstance);

	HWND hLogGroup = CreateWindow(
		L"BUTTON", nullptr,
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		555, 20, 500, 500,
		hwnd, nullptr, hInstance, nullptr
	);

	// �α� â ����
	hListBox = CreateWindow(
		L"LISTBOX", nullptr,
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_NOTIFY,
		565, 55, 480, 460,
		hwnd, (HMENU)3000, hInstance, nullptr
	);

	StartVideoReceiver();

	// 5. â ǥ��
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// 6. �޽��� ����
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);		// Ű���� �޽��� ����
		DispatchMessage(&msg);		// WndProc�� �޽��� ����
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_KEYDOWN:
		HandleKeyDown(hwnd, wParam);
		break;
	case WM_DESTROY:			//	â �ݱ� ��ư ����
		if (g_isRunning) {
			TerminateProcess(g_pyProcess.hProcess, 0);
			CloseHandle(g_pyProcess.hProcess);
			CloseHandle(g_pyProcess.hThread);
		}

		if (hCurrentBitmap) {
			DeleteObject(hCurrentBitmap);
			hCurrentBitmap = nullptr;
		}

		PostQuitMessage(0);		// �޽��� ���� ���� ��û

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);	// �������� �⺻ ó��
}