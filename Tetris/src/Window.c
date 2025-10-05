/* Includes *************************************************/

#include <Windows.h>
#include <tchar.h>

#include "Window.h"

/* Global variables *****************************************/

static HWND  hMainWindow;
static TCHAR szMainWindowClass[] = TEXT("TetrisMainWindow");

/* Private functions ****************************************/

BOOL WndpRegisterClass(HINSTANCE hInstance, WNDPROC lpfnWndProc)
{
    WNDCLASS wc            = { 0 };
    ATOM     atomMainClass = 0;

    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = szMainWindowClass;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpfnWndProc   = lpfnWndProc;

    atomMainClass = RegisterClass(&wc);

    return atomMainClass ? TRUE : FALSE;
}

BOOL WndpCreateWindow(HINSTANCE hInstance, LPCTSTR lpszWindowName, INT nWidth, INT nHeight)
{
    hMainWindow = CreateWindow(
        szMainWindowClass,
        lpszWindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nWidth, nHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    return hMainWindow ? TRUE : FALSE;
}

/* Global functions *****************************************/

BOOL WndInitialize(HINSTANCE hInstance, LPCTSTR lpszWindowName, INT nWidth, INT nHeight, WNDPROC lpfnWndProc)
{
    if (!WndpRegisterClass(hInstance, lpfnWndProc) ||
        !WndpCreateWindow(hInstance, lpszWindowName, nWidth, nHeight))
    {
        return FALSE;
    }
    return TRUE;
}

BOOL WndUninitialize(VOID)
{
    return TRUE;
}

HWND WndGetWindowHandle(VOID)
{
    return hMainWindow;
}

BOOL WndGetClientRect(LPRECT lpRect)
{
    return GetClientRect(hMainWindow, lpRect);
}



