/* Includes ****************/

#include <Windows.h>
#include <TChar.h>

#include "Window.h"
#include "Field.h"
#include "Renderer.h"
#include "Resource.h"
#include "Block.h"
#include "NextBlock.h"

/* Variables ***************/

static UINT_PTR uTimerId;

/* Functions ***************/

static VOID BlockDown(VOID)
{
    FdUpdateBufferField();
    if (!BlMoveBlockDown())
    {
        BlFixBlock();
        FdClearLine();
        FdUpdateBufferField();
        
        if (!NbNewBlock(TRUE, 0))
        {
            KillTimer(NULL, uTimerId);
            WndMessageBoxF(
                MB_OK | MB_ICONINFORMATION,
                TEXT("게임 오버"),
                TEXT("게임 오버!")
            );
            WndDestroy();
            return;
        }
    }
    BlDrawBlockInBuffer();
    WndUpdateWindow();
}

static VOID BlockLeft(VOID)
{
    FdUpdateBufferField();
    BlMoveBlockLeft();
    BlDrawBlockInBuffer();
    WndUpdateWindow();
}

static VOID BlockRight(VOID)
{
    FdUpdateBufferField();
    BlMoveBlockRight();
    BlDrawBlockInBuffer();
    WndUpdateWindow();
}

static VOID BlockTurn(VOID)
{
    FdUpdateBufferField();
    BlTurnBlock();
    BlDrawBlockInBuffer();
    WndUpdateWindow();
}

VOID MainInit(VOID)
{
    RsInitialize(GetModuleHandle(NULL));
    RdInitialize();
    FdInitialize();
    BlInitialize();
    NbInitialize();

    FdUpdateBufferField();
    BlDrawBlockInBuffer();
    
    uTimerId = SetTimer(NULL, 0, 1000, (TIMERPROC)BlockDown);
}

VOID MainEnd(VOID)
{
    KillTimer(NULL, uTimerId);
    FdUninitialize();
    RdUninitialize();
    RsUninitialize();
    BlUninitialize();
    NbUninitialize();
}

VOID MainLeftKeyDown(VOID)
{
    BlockLeft();
}

VOID MainRightKeyDown(VOID)
{
    BlockRight();
}

VOID MainDownKeyDown(VOID)
{
    BlockDown();
}

VOID MainSpaceKeyDown(VOID)
{
    BlockTurn();
}

VOID MainOnRender(VOID)
{
    PAINTSTRUCT   ps;
    HDC           hDC;
    HDC           hMemDC;
    HBITMAP       hMemBitmap;
    HBITMAP       hOldBitmap;
    RECT          clientRect;
    RENDERER_DATA rendererData;

    /* 페인트 초기화 */

    hDC = BeginPaint(WndGetWindowHandle(), &ps);
    WndGetClientRect(&clientRect);

    /* 백버퍼 구성 */

    if (!(hMemDC = CreateCompatibleDC(hDC)))
    {
        KillTimer(NULL, uTimerId);
        WndMessageBoxF(
            MB_OK | MB_ICONERROR,
            TEXT("오류"),
            TEXT("DC 생성을 실패했습니다. \n"
                 "오류 코드: %d"),
            GetLastError()
        );
        WndDestroy();
        return;
    }

    if (!(hMemBitmap = CreateCompatibleBitmap(hDC, clientRect.right, clientRect.bottom)))
    {
        KillTimer(NULL, uTimerId);
        WndMessageBoxF(
            MB_OK | MB_ICONERROR,
            TEXT("오류"),
            TEXT("비트맵 생성을 실패했습니다. \n"
                 "오류 코드: %d"),
            GetLastError()
        );
        WndDestroy();
        return;
    }

    hOldBitmap = SelectObject(hMemDC, hMemBitmap);

    /* 렌더 */

    rendererData.FieldCubeSizePx = 35;
    rendererData.NextBlockCubeSizePx = 15;
    rendererData.NextBlockPaddingPx = 15;

    RdRenderAll(WndGetWindowHandle(), hMemDC, &rendererData);

    /* 정리 */

    BitBlt(hDC,    0, 0, clientRect.right, clientRect.bottom,
           hMemDC, 0, 0,
           SRCCOPY);

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hMemBitmap);
    DeleteDC(hMemDC);

    EndPaint(WndGetWindowHandle(), &ps);
}
