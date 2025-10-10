/* Includes *************************************************/

#include <Windows.h>
#include <TChar.h>

#include "Resource.h"

/* Variables ************************************************/

static HBITMAP Bitmaps[RESOURCE_BITMAP_COUNT];
static LPCTSTR BitmapNames[] = {
    MAKEINTRESOURCE(IDB_CUBE_VOID),
    MAKEINTRESOURCE(IDB_CUBE_RED),
    MAKEINTRESOURCE(IDB_CUBE_YELLOW),
    MAKEINTRESOURCE(IDB_CUBE_GREEN),
    MAKEINTRESOURCE(IDB_CUBE_BLUE),
    MAKEINTRESOURCE(IDB_CUBE_PINK)
};

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

BOOL RsInitialize(HINSTANCE hInstance)
{
    if (!RspLoadBitmaps(hInstance, BitmapNames))
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
