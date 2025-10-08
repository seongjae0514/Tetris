/* Includes ***************/

#include <Windows.h>
#include <TChar.h>

#include "Main.h"
#include "Window.h"

/* Functions **************/

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
        {
            MainInit();
            return 0;
        }

        case WM_DESTROY:
        {
            MainEnd();
            PostQuitMessage(0);
            return 0;
        }

        case WM_PAINT:
        {
            MainOnRender();
            return 0;
        }

        case WM_SIZE:
        {
            WndUpdateWindow();
            break;
        }

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_LEFT:
                {
                    MainLeftKeyDown();
                    break;
                }
                case VK_RIGHT:
                {
                    MainRightKeyDown();
                    break;
                }
                case VK_DOWN:
                {
                    MainDownKeyDown();
                    break;
                }
                case VK_SPACE:
                {
                    MainSpaceKeyDown();
                    break;
                }
            }
            return 0;
        }
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT APIENTRY _tWinMain(_In_     HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_     LPTSTR    lpCmdLine,
                       _In_     INT       nCmdShow)
{
    MSG  msg         = { 0 };
    HWND hMainWindow = WndInitialize(hInstance, TEXT("Tetris"), 1000, 1000, WndProc);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (INT)msg.wParam;
}