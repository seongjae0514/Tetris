/* Includes *************************************************/

#include <Windows.h>
#include <TChar.h>

#include "Resource.h"

/* Variables ************************************************/

static HBITMAP Bitmaps[RESOURCE_BITMAP_COUNT];

/* Private functions ****************************************/

static BOOL RspLoadBitmaps(HINSTANCE hInstance, LPCTSTR* BitmapNamesArray)
{
    for (RESOURCE_BITMAP i = 0; i < RESOURCE_BITMAP_COUNT; i++)
    {
        if (!(Bitmaps[i] = (HBITMAP)LoadImage(hInstance, BitmapNamesArray[i], IMAGE_BITMAP, 0, 0, (hInstance ? 0 : LR_LOADFROMFILE) | LR_DEFAULTCOLOR)))
        {
            return FALSE;
        }
    }
    return TRUE;
}

static BOOL RspUnloadBitmaps(VOID)
{
    for (RESOURCE_BITMAP i = 0; i < RESOURCE_BITMAP_COUNT; i++)
    {
        DeleteObject(Bitmaps[i]);
        Bitmaps[i] = NULL;
    }
    return TRUE;
}

/* Public functions *****************************************/

BOOL RsInitialize(HINSTANCE hInstance, LPCTSTR* BitmapNamesArray)
{
    if (!RspLoadBitmaps(hInstance, BitmapNamesArray))
    {
        RsUninitialize();
        return FALSE;
    }

    return TRUE;
}

BOOL RsUninitialize(VOID)
{
    RspUnloadBitmaps();
    return TRUE;
}

HBITMAP RsGetBitmapHandle(RESOURCE_BITMAP Index)
{
    if (Index >= RESOURCE_BITMAP_COUNT)
    {
        return NULL;
    }

    return Bitmaps[Index];
}
