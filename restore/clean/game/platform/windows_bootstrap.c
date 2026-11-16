#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pharaoh/application_identity.h"

static const wchar_t window_class_name[] = L"PharaohCleanWindow";

static LRESULT CALLBACK bootstrap_window_proc(HWND window, UINT message,
    WPARAM wparam, LPARAM lparam)
{
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previous, PWSTR command_line,
    int show_command)
{
    WNDCLASSEXW window_class = { sizeof(window_class) };
    HWND window;
    MSG message;
    HICON application_icon = (HICON)LoadImageW(instance,
        MAKEINTRESOURCEW(PHARAOH_CLEAN_APPLICATION_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
    HCURSOR application_cursor = (HCURSOR)LoadImageW(instance,
        MAKEINTRESOURCEW(102), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
    (void)previous;
    (void)command_line;
    window_class.lpfnWndProc = bootstrap_window_proc;
    window_class.hInstance = instance;
    window_class.hCursor = application_cursor ? application_cursor :
        LoadCursorW(NULL, MAKEINTRESOURCEW(32512));
    window_class.hIcon = application_icon;
    window_class.hIconSm = application_icon;
    window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    window_class.lpszClassName = window_class_name;
    if (!RegisterClassExW(&window_class)) return (int)GetLastError();

    window = CreateWindowExW(0, window_class_name, PHARAOH_CLEAN_WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 960, 640,
        NULL, NULL, instance, NULL);
    if (!window) return (int)GetLastError();
    ShowWindow(window, show_command);
    UpdateWindow(window);

    while (GetMessageW(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return (int)message.wParam;
}
