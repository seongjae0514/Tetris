#include <Windows.h>
#include <TChar.h>

#include "Window.h"
#include "Field.h"
#include "Renderer.h"
#include "Resource.h"
#include "resource_name.h"
#include "winbase.h"

LRESULT CALLBACK WndProc(_In_     HWND   hWnd,
                         _In_opt_ UINT   uMsg, 
                         _In_     WPARAM wParam,
                         _In_     LPARAM lParam)
{
    switch (uMsg)
    {

        case WM_CREATE:
        {
            SetTimer(hWnd, 1, 1000, NULL);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hDC = BeginPaint(hWnd, &ps);

            RdRenderAll(hWnd, hDC, 35);

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

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_LEFT:
                {
                    FdMoveBlockLeft();
                    break;
                }
                case VK_RIGHT:
                {
                    FdMoveBlockRight();
                    break;
                }
                case VK_SPACE:
                {
                    FdTurnBlock();
                    break;
                }
                case VK_DOWN:
                {
                    FdMoveBlockDown();
                    break;
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }

        case WM_TIMER:
        {
            FdMoveBlockDown();
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        }

    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, INT nCmdShow)
{
    LPCTSTR bitmapNames[] = {
        MAKEINTRESOURCE(IDB_CUBE_VOID),
        MAKEINTRESOURCE(IDB_CUBE_RED),
        MAKEINTRESOURCE(IDB_CUBE_YELLOW),
        MAKEINTRESOURCE(IDB_CUBE_GREEN),
        MAKEINTRESOURCE(IDB_CUBE_BLUE),
        MAKEINTRESOURCE(IDB_CUBE_PINK),
        MAKEINTRESOURCE(IDB_CUBE_WALL)
    };

    WndInitialize(hInstance, TEXT("Tetris"), 500, 500, WndProc);
    RsInitialize(hInstance, bitmapNames);
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