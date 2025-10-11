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
                TEXT("���� ����"),
                TEXT("���� ����!")
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

    /* ����Ʈ �ʱ�ȭ */

    hDC = BeginPaint(WndGetWindowHandle(), &ps);
    WndGetClientRect(&clientRect);

    /* ����� ���� */

    if (!(hMemDC = CreateCompatibleDC(hDC)))
    {
        KillTimer(NULL, uTimerId);
        WndMessageBoxF(
            MB_OK | MB_ICONERROR,
            TEXT("����"),
            TEXT("DC ������ �����߽��ϴ�. \n"
                 "���� �ڵ�: %d"),
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
            TEXT("����"),
            TEXT("��Ʈ�� ������ �����߽��ϴ�. \n"
                 "���� �ڵ�: %d"),
            GetLastError()
        );
        WndDestroy();
        return;
    }

    hOldBitmap = SelectObject(hMemDC, hMemBitmap);

    /* ���� */

    rendererData.FieldCubeSizePx = 35;
    rendererData.NextBlockCubeSizePx = 15;
    rendererData.NextBlockPaddingPx = 15;

    RdRenderAll(WndGetWindowHandle(), hMemDC, &rendererData);

    /* ���� */

    BitBlt(hDC,    0, 0, clientRect.right, clientRect.bottom,
           hMemDC, 0, 0,
           SRCCOPY);

    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hMemBitmap);
    DeleteDC(hMemDC);

    EndPaint(WndGetWindowHandle(), &ps);
}
