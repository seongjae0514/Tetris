#ifndef GAME_NEXTBLOCK_H
#define GAME_NEXTBLOCK_H

#include <Windows.h>
#include "Block.h"

#define NEXT_BLOCK_COUNT 4

BOOL        NbInitialize(VOID);
BOOL        NbUninitialize(VOID);
BLOCK_SHAPE NbGetNextBlock(UINT Index);
BOOL        NbNewBlock(BOOL bRandom, BLOCK_SHAPE Shape);

#endif