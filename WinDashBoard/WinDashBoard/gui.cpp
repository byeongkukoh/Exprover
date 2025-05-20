#include "gui.h"
#include "command.h"
#include <string>
#include <vector>

HWND hLogBox;

PROCESS_INFORMATION g_pyProcess = {};
bool g_isRunning = false;

int InitGUI(HINSTANCE hInstance, int nCmdShow) {
	// 1. 윈도우 클래스 이름 정의 (식별용)
	const wchar_t CLASS_NAME[] = L"DASHBOARD";

	// 2. 윈도우 클래스 구조체 초기화
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;						// 메시지 처리 함수 등록
	wc.hInstance = hInstance;						// 현재 실행 중인 인스턴스 핸들
	wc.lpszClassName = CLASS_NAME;					// 클래스 이름 정의
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	// 배경색 설정 (흰색)

	// 3. 클래스 등록
	RegisterClass(&wc);

	// 4. 윈도우 생성
	HWND hwnd = CreateWindowEx(
		0,											// 확장 스타일 (없음)
		CLASS_NAME,									// 앞에서 등록한 클래스 이름
		L"MQTT DASHBOARD",							// 창 제목
		WS_OVERLAPPEDWINDOW,						// 기본 윈도우 스타일
		CW_USEDEFAULT, CW_USEDEFAULT, 900, 500,		// 위치(x, y), 크기 (width, height)
		nullptr, nullptr, hInstance, nullptr		// 부모, 메뉴 없음
	);

	// 로그 창 생성
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
		L"카메라 ON",  // 초기 버튼 텍스트
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		600, 70, 200, 40,
		hwnd,
		(HMENU)2001,   // 버튼 ID
		hInstance,
		nullptr
	);

	// 버튼 생성
	CreateButtonControls(hwnd, hInstance);

	// 5. 창 표시
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// 6. 메시지 루프
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);		// 키보드 메시지 번역
		DispatchMessage(&msg);		// WndProc에 메시지 전달
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 2001:  // 카메라 ON/OFF 토글
			if (!g_isRunning) {
				// Python 실행
				STARTUPINFO si = { sizeof(STARTUPINFO) };
				PROCESS_INFORMATION pi = {};
				std::wstring cmd = L"python C:\\Users\\luna\\Downloads\\main\\connect.py";
				std::vector<wchar_t> cmdBuffer(cmd.begin(), cmd.end());
				cmdBuffer.push_back(0);

				if (CreateProcessW(nullptr, cmdBuffer.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
					g_pyProcess = pi;
					g_isRunning = true;
					SetWindowText(GetDlgItem(hwnd, 2001), L"카메라 OFF");  // 텍스트 변경
				}
				else {
					MessageBox(hwnd, L"Python 실행 실패!", L"에러", MB_ICONERROR);
				}
			}
			else {
				// Python 종료
				TerminateProcess(g_pyProcess.hProcess, 0);
				CloseHandle(g_pyProcess.hProcess);
				CloseHandle(g_pyProcess.hThread);
				g_isRunning = false;
				SetWindowText(GetDlgItem(hwnd, 2001), L"카메라 ON");  // 텍스트 변경
			}
			break;

		default:
			HandleButtonCommand(hwnd, wParam);  // 기존 다른 버튼 처리
			break;
		}
		break;
		//HandleButtonCommand(hwnd, wParam);
		//break;
	case WM_KEYDOWN:
		HandleKeyDown(hwnd, wParam);
		break;
	case WM_DESTROY:			//	창 닫기 버튼 눌림
		if (g_isRunning) {
			TerminateProcess(g_pyProcess.hProcess, 0);
			CloseHandle(g_pyProcess.hProcess);
			CloseHandle(g_pyProcess.hThread);
		}
		PostQuitMessage(0);		// 메시지 루프 종료 요청

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);	// 나머지는 기본 처리
}