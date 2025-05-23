#pragma once
#include <windows.h>

std::wstring GetCurrentTimeString();
std::wstring ConvertToWString(const std::string& str);
void CreateButtonControls(HWND hwnd, HINSTANCE hinstance);
void HandleButtonCommand(HWND hwnd, WPARAM wParam);
void HandleKeyDown(HWND hwnd, WPARAM wParam);
void AddLogMsg(const wchar_t* message);