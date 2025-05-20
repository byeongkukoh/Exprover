#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <cstdlib>

int InitGUI(HINSTANCE hInstance, int nCmdShow);
// 메시지를 처리할 콜백 함수
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);