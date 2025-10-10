/* Includes ********************/

#include <Windows.h>
#include <TChar.h>
#include <stdlib.h>

#include "Block.h"
#include "NextBlock.h"

/* Global variables ************/

static BLOCK_SHAPE NextBlocks[NEXT_BLOCK_COUNT];

/* Private functions ***********/

static BLOCK_SHAPE NbpGenerateBlock(VOID)
{
    return rand() % BLOCK_SHAPE_COUNT;
}

static BOOL NbpPushBlock(BLOCK_SHAPE Shape)
{
    BOOL b = BlSetBlock(FALSE, NextBlocks[0]);

    for (UINT i = 0; i < NEXT_BLOCK_COUNT - 1; i++)
    {
        NextBlocks[i] = NextBlocks[i + 1];
    }

    NextBlocks[NEXT_BLOCK_COUNT - 1] = Shape;

    return b;
}

/* Global functions ************/

BOOL NbInitialize(VOID)
{
    srand(time(NULL));

    for (UINT i = 0; i < NEXT_BLOCK_COUNT; i++)
    {
        NextBlocks[i] = NbpGenerateBlock();
    }

    NbpPushBlock(NbpGenerateBlock());

    return TRUE;
}

BOOL NbUninitialize(VOID)
{
    return TRUE;
}

BLOCK_SHAPE NbGetNextBlock(UINT Index)
{
    if (Index >= NEXT_BLOCK_COUNT)
    {
        return BLOCK_SHAPE_COUNT;
    }

    return NextBlocks[Index];
}

BOOL NbNewBlock(BOOL bRandom, BLOCK_SHAPE Shape)
{
    return NbpPushBlock(bRandom ? NbpGenerateBlock() : Shape);
}
