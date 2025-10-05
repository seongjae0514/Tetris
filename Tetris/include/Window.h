#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <Windows.h>
#include <tchar.h>

extern BOOL WndInitialize     (HINSTANCE hInstance, LPCTSTR lpszWindowName, INT nWidth, INT nHeight, WNDPROC lpfnWndProc);
extern BOOL WndUninitialize   (VOID);
extern HWND WndGetWindowHandle(VOID);
extern BOOL WndGetClientRect  (LPRECT lpRect);

#endif /* GAME_WINDOW_H */