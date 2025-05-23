#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "command.h"
#include "mqtt.h"
#include "Resource.h"

extern HWND hListBox;

std::wstring GetCurrentTimeString() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm local_tm;
    localtime_s(&local_tm, &t);

    std::wstringstream ss;
    ss << L"[" << std::put_time(&local_tm, L"%Y-%m-%d %H:%M:%S") << L"]";
    return ss.str();
}

void CreateButtonControls(HWND hwnd, HINSTANCE hInstance) {
    CreateWindow(
        L"BUTTON", L"↑",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, // WS_TABSTOP 제거
        85, 460, 50, 50,
        hwnd, (HMENU)ID_UP, hInstance, nullptr
    );
    CreateWindow(
        L"BUTTON", L"←",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        25, 520, 50, 50,
        hwnd, (HMENU)ID_LEFT, hInstance, nullptr
    );
    CreateWindow(
        L"BUTTON", L"↓",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        85, 520, 50, 50,
        hwnd, (HMENU)ID_DOWN, hInstance, nullptr
    );
    CreateWindow(
        L"BUTTON", L"→",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        145, 520, 50, 50,
        hwnd, (HMENU)ID_RIGHT, hInstance, nullptr
    );
    CreateWindow(
        L"BUTTON", L"Space",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        205, 520, 200, 50,
        hwnd, (HMENU)ID_SPACE, hInstance, nullptr
    );
    CreateWindow(
        L"BUTTON", L"Ctrl",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        415, 520, 110, 50,
        hwnd, (HMENU)ID_CTRL, hInstance, nullptr
    );
}

void HandleButtonCommand(HWND hwnd, WPARAM wParam) {
    std::wstring log = L"DEBUG | HandleButtonCommand called with wParam: " + std::to_wstring(wParam);
    AddLogMsg(log.c_str());
    switch (LOWORD(wParam)) {
    case ID_UP:
        AddLogMsg(L"INFO | ↑ 버튼이 눌렸습니다.");
        AsyncPublish("go");
        break;
    case ID_DOWN:
        AddLogMsg(L"INFO | ↓ 버튼이 눌렸습니다.");
        AsyncPublish("back");
        break;
    case ID_LEFT:
        AddLogMsg(L"INFO | ← 버튼이 눌렸습니다.");
        AsyncPublish("left");
        break;
    case ID_RIGHT:
        AddLogMsg(L"INFO | → 버튼이 눌렸습니다.");
        AsyncPublish("right");
        break;
    case ID_SPACE:
        AddLogMsg(L"INFO | SPACE 버튼이 눌렸습니다.");
        AsyncPublish("stop");
        break;
    case ID_CTRL:
        AddLogMsg(L"INFO | CTRL 버튼이 눌렸습니다.");
        AsyncPublish("hi");
        break;
    }
}

void HandleKeyDown(HWND hwnd, WPARAM wParam) {
    std::wstring log = L"DEBUG | HandleKeyDown called with wParam: " + std::to_wstring(wParam) + L" (Hex: " + std::to_wstring(wParam) + L")";
    AddLogMsg(log.c_str());
    switch (wParam) {
    case VK_UP:
        AddLogMsg(L"DEBUG | Calling HandleButtonCommand for VK_UP (0x26)");
        HandleButtonCommand(hwnd, ID_UP);
        break;
    case VK_DOWN:
        AddLogMsg(L"DEBUG | Calling HandleButtonCommand for VK_DOWN (0x28)");
        HandleButtonCommand(hwnd, ID_DOWN);
        break;
    case VK_LEFT:
        AddLogMsg(L"DEBUG | Calling HandleButtonCommand for VK_LEFT (0x25)");
        HandleButtonCommand(hwnd, ID_LEFT);
        break;
    case VK_RIGHT:
        AddLogMsg(L"DEBUG | Calling HandleButtonCommand for VK_RIGHT (0x27)");
        HandleButtonCommand(hwnd, ID_RIGHT);
        break;
    case VK_SPACE:
        AddLogMsg(L"DEBUG | Calling HandleButtonCommand for VK_SPACE (0x20)");
        HandleButtonCommand(hwnd, ID_SPACE);
        break;
    case VK_CONTROL:
        AddLogMsg(L"DEBUG | Calling HandleButtonCommand for VK_CONTROL (0x20)");
        HandleButtonCommand(hwnd, ID_CTRL);
        break;
    default:
        AddLogMsg((L"DEBUG | Unknown key code: " + std::to_wstring(wParam)).c_str());
        break;
    }
}

void AddLogMsg(const wchar_t* message) {
    std::wstring time = GetCurrentTimeString();
    std::wstring fullMessage = time + L" " + message;
    SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)fullMessage.c_str());
    int count = (int)SendMessage(hListBox, LB_GETCOUNT, 0, 0);
    SendMessage(hListBox, LB_SETTOPINDEX, count - 1, 0);
}

std::wstring ConvertToWString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
    wstr.pop_back();
    return wstr;
}