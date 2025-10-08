/* Includes ****************/

#include <Windows.h>
#include <TChar.h>

#include "Window.h"
#include "Field.h"
#include "Renderer.h"
#include "Resource.h"
#include "resource_name.h"

/* Variables ***************/

static UINT_PTR uTimerId;

/* Functions ***************/

static VOID TimerProc(HWND hWnd, UINT uMsg, UINT_PTR wParam, DWORD lParam)
{
    FdMoveBlockDown();
    WndUpdateWindow();
}

VOID MainInit(VOID)
{
    LPTSTR bitmapNames[] = {
        MAKEINTRESOURCE(IDB_CUBE_VOID),
        MAKEINTRESOURCE(IDB_CUBE_RED),
        MAKEINTRESOURCE(IDB_CUBE_YELLOW),
        MAKEINTRESOURCE(IDB_CUBE_GREEN),
        MAKEINTRESOURCE(IDB_CUBE_BLUE),
        MAKEINTRESOURCE(IDB_CUBE_PINK)
    };

    RsInitialize(GetModuleHandle(NULL), bitmapNames);
    RdInitialize();
    FdInitialize();

    FdSetCurrentBlock(TRUE, 0);
    
    uTimerId = SetTimer(NULL, 0, 1000, TimerProc);
}

VOID MainEnd(VOID)
{
    KillTimer(NULL, uTimerId);
    FdUninitialize();
    RdUninitialize();
    RsUninitialize();
}

VOID MainLeftKeyDown(VOID)
{
    FdMoveBlockLeft();
    WndUpdateWindow();
}

VOID MainRightKeyDown(VOID)
{
    FdMoveBlockRight();
    WndUpdateWindow();
}

VOID MainDownKeyDown(VOID)
{
    FdMoveBlockDown();
    WndUpdateWindow();
}

VOID MainSpaceKeyDown(VOID)
{
    FdTurnBlock();
    WndUpdateWindow();
}

VOID MainOnRender(VOID)
{
    PAINTSTRUCT ps;
    HDC         hDC;
    HDC         hMemDC;
    HBITMAP     hMemBitmap;
    HBITMAP     hOldBitmap;
    RECT        clientRect;

    /* 페인트 초기화 */

    hDC = BeginPaint(WndGetWindowHandle(), &ps);
    WndGetClientRect(&clientRect);

    /* 백버퍼 구성 */

    hMemDC = CreateCompatibleDC(hDC);
    hMemBitmap = CreateCompatibleBitmap(hDC, clientRect.right, clientRect.bottom);

    hOldBitmap = SelectObject(hMemDC, hMemBitmap);

    /* 렌더 */

    RdRenderAll(WndGetWindowHandle(), hMemDC, 35);

    /* 정리 */

    BitBlt(hDC,    0, 0, clientRect.right, clientRect.bottom,
           hMemDC, 0, 0,
           SRCCOPY);

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hMemBitmap);
    DeleteDC(hMemDC);

    EndPaint(WndGetWindowHandle(), &ps);
}
