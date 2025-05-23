#include <string>
#include <vector>

#include "gui.h"
#include "command.h"
#include "mqtt_video_receiver.h"


HWND hListBox;
HFONT hFont;	// 전역 폰트 핸들 선언

HWND hPictureBox;					// 영상 출력용 PictureBox
HBITMAP hCurrentBitmap = nullptr;	// 현재 표시 중인 비트맵

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
		CW_USEDEFAULT, CW_USEDEFAULT, 1080, 570,		// 위치(x, y), 크기 (width, height)
		nullptr, nullptr, hInstance, nullptr		// 부모, 메뉴 없음
	);

	// 영상 창 생성
	hPictureBox = CreateWindow(
		L"STATIC", nullptr,
		WS_VISIBLE | WS_CHILD | SS_BITMAP | WS_BORDER,
		25, 25, 500, 375,  // 좌표 및 크기 조절 가능
		hwnd, nullptr, hInstance, nullptr
	);

	// 버튼 생성
	CreateButtonControls(hwnd, hInstance);

	HWND hLogGroup = CreateWindow(
		L"BUTTON", nullptr,
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		555, 20, 500, 500,
		hwnd, nullptr, hInstance, nullptr
	);

	// 로그 창 생성
	hListBox = CreateWindow(
		L"LISTBOX", nullptr,
		WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_NOTIFY,
		565, 55, 480, 460,
		hwnd, (HMENU)3000, hInstance, nullptr
	);

	StartVideoReceiver();

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
	case WM_KEYDOWN:
		HandleKeyDown(hwnd, wParam);
		break;
	case WM_DESTROY:			//	창 닫기 버튼 눌림
		if (g_isRunning) {
			TerminateProcess(g_pyProcess.hProcess, 0);
			CloseHandle(g_pyProcess.hProcess);
			CloseHandle(g_pyProcess.hThread);
		}

		if (hCurrentBitmap) {
			DeleteObject(hCurrentBitmap);
			hCurrentBitmap = nullptr;
		}

		PostQuitMessage(0);		// 메시지 루프 종료 요청

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);	// 나머지는 기본 처리
}