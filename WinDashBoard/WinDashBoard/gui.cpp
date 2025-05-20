#include "gui.h"
#include "command.h"
#include <string>
#include <vector>

HWND hLogBox;

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
		CW_USEDEFAULT, CW_USEDEFAULT, 900, 500,		// ��ġ(x, y), ũ�� (width, height)
		nullptr, nullptr, hInstance, nullptr		// �θ�, �޴� ����
	);

	// �α� â ����
	hLogBox = CreateWindow(
		L"EDIT",
		L"",
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
		25, 25, 550, 200,
		hwnd,
		nullptr,
		hInstance,
		nullptr
	);

	HWND hToggleBtn = CreateWindow(
		L"BUTTON",
		L"ī�޶� ON",  // �ʱ� ��ư �ؽ�Ʈ
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		600, 70, 200, 40,
		hwnd,
		(HMENU)2001,   // ��ư ID
		hInstance,
		nullptr
	);

	// ��ư ����
	CreateButtonControls(hwnd, hInstance);

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
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2001:  // ī�޶� ON/OFF ���
			if (!g_isRunning) {
				// Python ����
				STARTUPINFO si = { sizeof(STARTUPINFO) };
				PROCESS_INFORMATION pi = {};
				std::wstring cmd = L"python C:\\Users\\luna\\Downloads\\main\\connect.py";
				std::vector<wchar_t> cmdBuffer(cmd.begin(), cmd.end());
				cmdBuffer.push_back(0);

				if (CreateProcessW(nullptr, cmdBuffer.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
					g_pyProcess = pi;
					g_isRunning = true;
					SetWindowText(GetDlgItem(hwnd, 2001), L"ī�޶� OFF");  // �ؽ�Ʈ ����
				}
				else {
					MessageBox(hwnd, L"Python ���� ����!", L"����", MB_ICONERROR);
				}
			}
			else {
				// Python ����
				TerminateProcess(g_pyProcess.hProcess, 0);
				CloseHandle(g_pyProcess.hProcess);
				CloseHandle(g_pyProcess.hThread);
				g_isRunning = false;
				SetWindowText(GetDlgItem(hwnd, 2001), L"ī�޶� ON");  // �ؽ�Ʈ ����
			}
			break;

		default:
			HandleButtonCommand(hwnd, wParam);  // ���� �ٸ� ��ư ó��
			break;
		}
		break;
		//HandleButtonCommand(hwnd, wParam);
		//break;
	case WM_KEYDOWN:
		HandleKeyDown(hwnd, wParam);
		break;
	case WM_DESTROY:			//	â �ݱ� ��ư ����
		if (g_isRunning) {
			TerminateProcess(g_pyProcess.hProcess, 0);
			CloseHandle(g_pyProcess.hProcess);
			CloseHandle(g_pyProcess.hThread);
		}
		PostQuitMessage(0);		// �޽��� ���� ���� ��û

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);	// �������� �⺻ ó��
}