/* Includes *************************************************/

#include <Windows.h>
#include <TChar.h>

#include "Renderer.h"

#include "Resource.h"
#include "Field.h"

/* Variables ************************************************/

#define GET_CENTER_ALIGNED_POSITION(DEST_SIZE, SRC_SIZE) \
        ((DEST_SIZE) - (SRC_SIZE)) / 2

/* Private functions ****************************************/

static RESOURCE_BITMAP RdpCubeTypeToBitmapIndex(CUBE_TYPE CubeType)
{
    return CubeType; // enum 순서가 같으므로 둘이 같음.
}

static BOOL RdpRenderField(HWND hDestWindow, HDC hDestDC, UINT uCubeSizePx)
{
    RECT clientRect;
    INT  originX, originY;
    INT  x, y;
    UINT width, height;
    HDC  hMemDC;

    GetClientRect(hDestWindow, &clientRect);

    width   = uCubeSizePx * (FIELD_WIDTH);
    height  = uCubeSizePx * (FIELD_HEIGHT);
    originX = GET_CENTER_ALIGNED_POSITION((INT)clientRect.right, (INT)width);
    originY = GET_CENTER_ALIGNED_POSITION((INT)clientRect.bottom, (INT)height);

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
            HBITMAP hBitmap = RsGetBitmapHandle(RdpCubeTypeToBitmapIndex(FdGetScreenCube(j, i)));
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

/* Public functions *****************************************/

BOOL RdInitialize(VOID)
{
    return TRUE;
}

BOOL RdUninitialize(VOID)
{
    return TRUE;
}

BOOL RdRenderAll(HWND hWnd, HDC hDC, UINT uCubeSizePx)
{
    BOOL ret;
    ret = RdpRenderField(hWnd, hDC, uCubeSizePx);
    return ret;
}