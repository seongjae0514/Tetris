#include <Windows.h>
#include <TChar.h>

#include "Window.h"
#include "Field.h"
#include "Renderer.h"
#include "Resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);

            RdRenderAll(hWnd, hDC, 20);

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_SIZE:
        {
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }

    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
    LPCTSTR bitmapNames[] = {
        TEXT("image\\void.bmp"),
        TEXT("image\\red.bmp"),
        TEXT("image\\red.bmp"),
        TEXT("image\\red.bmp"),
        TEXT("image\\red.bmp"),
        TEXT("image\\red.bmp"),
        TEXT("image\\red.bmp")
    };

    WndInitialize(hInstance, TEXT("Tetris"), 500, 500, WndProc);
    RsInitialize(NULL, bitmapNames);
    RdInitialize();
    FdInitialize();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }

    WndUninitialize();
    RsUninitialize();
    RdUninitialize();
    FdUninitialize();
    
    return 0;
}