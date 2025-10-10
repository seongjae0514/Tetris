#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <Windows.h>
#include <TChar.h>

#include "Field.h"

typedef struct _RENDERER_DATA {
    UINT FieldCubeSizePx;
    UINT NextBlockCubeSizePx;
    UINT NextBlockPaddingPx;
} RENDERER_DATA, *PRENDERER_DATA;

BOOL RdInitialize(VOID);
BOOL RdUninitialize(VOID);

BOOL RdRenderAll(HWND hWnd, HDC hDC, PRENDERER_DATA lpRenderData);



#endif