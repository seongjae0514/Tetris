#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <Windows.h>
#include <TChar.h>

#include "Field.h"

BOOL RdInitialize(VOID);
BOOL RdUninitialize(VOID);

BOOL RdRenderAll(HWND hWnd, HDC hDC, UINT uCubeSizePx);



#endif