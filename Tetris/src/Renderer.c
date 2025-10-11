/* Includes *************************************************/

#include <Windows.h>
#include <TChar.h>

#include "Renderer.h"

#include "Resource.h"
#include "Field.h"
#include "NextBlock.h"
#include "Block.h"

/* Variables ************************************************/

#define GET_CENTER_ALIGNED_POSITION(DEST_SIZE, SRC_SIZE) \
        ((DEST_SIZE) - (SRC_SIZE)) / 2

/* Private functions ****************************************/

static RESOURCE_BITMAP RdpCubeTypeToBitmapIndex(CUBE_TYPE CubeType)
{
    return CubeType; // enum 순서가 같으므로 둘이 같음.
}

static VOID RdpGetFieldRenderSize(UINT uCubeSizePx, LPSIZE lpSize)
{
    if (lpSize)
    {
        lpSize->cx = uCubeSizePx * FIELD_WIDTH;
        lpSize->cy = uCubeSizePx * FIELD_HEIGHT;
    }
}

static BOOL RdpRenderField(HWND hDestWindow, HDC hDestDC, UINT uCubeSizePx, INT X, INT Y)
{
    INT  originX = X;
    INT  originY = Y;
    INT  x;
    INT  y;
    HDC  hMemDC;

    x = originX;
    y = originY;

    hMemDC = CreateCompatibleDC(hDestDC);

    if (!hMemDC)
    {
        return FALSE;
    }

    for (INT i = 0; i < FIELD_HEIGHT; i++)
    {
        for (INT j = 0; j < FIELD_WIDTH; j++)
        {
            HBITMAP hBitmap = RsGetBitmapHandle(RdpCubeTypeToBitmapIndex(FdGetBufferCube(j, i)));
            HBITMAP hOldBitmap;
            BITMAP  bmp;
            
            if (!hBitmap)
            {
                continue;
            }

            GetObject(hBitmap, sizeof(bmp), &bmp);
            hOldBitmap = SelectObject(hMemDC, hBitmap);
            
            StretchBlt(hDestDC, x, y, uCubeSizePx, uCubeSizePx,
                       hMemDC,  0, 0, bmp.bmWidth, bmp.bmHeight,
                       SRCCOPY);

            SelectObject(hMemDC, hOldBitmap);

            x += uCubeSizePx;
        }
        x =  originX;
        y += uCubeSizePx;
    }

    DeleteDC(hMemDC);

    return TRUE;
}

static VOID RdpGetNextBlockRenderSize(UINT uCubeSizePx, LPSIZE lpSize)
{
    if (lpSize)
    {
        lpSize->cx = uCubeSizePx * BLOCK_MOLD_WIDTH;
        lpSize->cy = uCubeSizePx * BLOCK_MOLD_HEIGHT;
    }
}

static BOOL RdpRenderNextBlock(HWND hDestWindow, HDC hDestDC, UINT uCubeSizePx, INT X, INT Y, UINT NextBlockIndex)
{

    INT x = X, y = Y;
    HDC hMemDC;

    hMemDC = CreateCompatibleDC(hDestDC);

    if (!hMemDC)
    {
        return FALSE;
    }

    for (INT i = 0; i < BLOCK_MOLD_HEIGHT; i++)
    {
        for (INT j = 0; j < BLOCK_MOLD_WIDTH; j++)
        {
            HBITMAP hBitmap = RsGetBitmapHandle(RdpCubeTypeToBitmapIndex(BlGetBlockShapeCube(NbGetNextBlock(NextBlockIndex), BLOCK_HEADING_UP, i, j)));
            HBITMAP hOldBitmap;
            BITMAP  bmp;

            if (!hBitmap)
            {
                continue;
            }

            GetObject(hBitmap, sizeof(bmp), &bmp);
            hOldBitmap = SelectObject(hMemDC, hBitmap);

            StretchBlt(hDestDC, x, y, uCubeSizePx, uCubeSizePx,
                       hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight,
                       SRCCOPY);

            SelectObject(hMemDC, hOldBitmap);

            x += uCubeSizePx;
        }
        x = X;
        y += uCubeSizePx;
    }

    DeleteDC(hMemDC);

    return TRUE;
}



/* Public functions *****************************************/

BOOL RdInitialize(VOID)
{
    return TRUE;
}

BOOL RdUninitialize(VOID)
{
    return TRUE;
}

BOOL RdRenderAll(HWND hWnd, HDC hDC, PRENDERER_DATA lpRenderData)
{
    RECT clientRect;
    SIZE fieldRenderSize;
    SIZE nextBlockRenderSize;
    INT  fieldX, fieldY;
    INT  nextBlockX, nextBlockY;

    GetClientRect(hWnd, &clientRect);
    RdpGetNextBlockRenderSize(lpRenderData->NextBlockCubeSizePx, &nextBlockRenderSize);
    RdpGetFieldRenderSize(lpRenderData->FieldCubeSizePx, &fieldRenderSize);

    fieldX = GET_CENTER_ALIGNED_POSITION(clientRect.right, fieldRenderSize.cx);
    fieldY = GET_CENTER_ALIGNED_POSITION(clientRect.bottom, fieldRenderSize.cy);

    nextBlockX = fieldX + fieldRenderSize.cx + lpRenderData->NextBlockPaddingPx;
    nextBlockY = fieldY;

    RdpRenderField(hWnd, hDC, lpRenderData->FieldCubeSizePx, fieldX, fieldY);
    for (UINT i = 0; i < NEXT_BLOCK_COUNT; i++)
    {
        RdpRenderNextBlock(hWnd, hDC, lpRenderData->NextBlockCubeSizePx, nextBlockX, nextBlockY, i);
        nextBlockY += nextBlockRenderSize.cy + 10;
    }

    return TRUE;
}
