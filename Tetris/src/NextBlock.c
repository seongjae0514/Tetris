/* Includes ********************/

#include <Windows.h>
#include <TChar.h>
#include <BCrypt.h>

#include "Block.h"

/* Global variables ************/

#define NEXT_BLOCK_COUNT 4

static BLOCK_SHAPE NextBlocks[NEXT_BLOCK_COUNT];

/* Private functions ***********/

static BLOCK_SHAPE NbpGenerateBlock(VOID)
{
    BLOCK_SHAPE shape;
    BCryptGenRandom(NULL, (PUCHAR)&shape, sizeof(shape), BCRYPT_USE_SYSTEM_PREFERRED_RNG);

    return shape % BLOCK_SHAPE_COUNT;
}

static VOID NbpPushBlock(BLOCK_SHAPE Shape)
{
    BlSetBlock(FALSE, NextBlocks[0]);

    for (UINT i = 0; i < NEXT_BLOCK_COUNT - 1; i++)
    {
        NextBlocks[i] = NextBlocks[i + 1];
    }

    NextBlocks[NEXT_BLOCK_COUNT - 1] = Shape;
}

/* Global functions ************/

BOOL NbInitialize(VOID)
{
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
    NbpPushBlock(bRandom ? NbpGenerateBlock() : Shape);
    return TRUE;
}
