/* Includes *****************/

#include <Windows.h>
#include <TChar.h>

#include "Field.h"
#include "Block.h"

/* Functions *****************/

BOOL BpvInitialize(VOID)
{
    return TRUE;
}

BOOL BpvUninitialize(VOID)
{
    return TRUE;
}

BOOL BpvDrawPreviewBlockInBuffer(VOID)
{
    BLOCK currentBlock;
    UINT  y = 0;

    BlGetBlockInformation(&currentBlock);

    for (UINT i = 0; i < FIELD_HEIGHT; i++)
    {
        if (BlIsContact(currentBlock.Shape, currentBlock.Heading, currentBlock.X, i + 1))
        {
            y = i;
            break;
        }
    }

    for (UINT i = 0; i < BLOCK_MOLD_HEIGHT; i++)
    {
        for (UINT j = 0; j < BLOCK_MOLD_WIDTH; j++)
        {
            if (BlGetBlockShapeCube(currentBlock.Shape, currentBlock.Heading, j, i) != CUBE_TYPE_VOID)
            {
                FdSetBufferCube(j + currentBlock.X, i + y, CUBE_TYPE_PREVIEW);
            }
        }
    }

    return TRUE;
}
