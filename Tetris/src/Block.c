/* Includes *******************/

#include <Windows.h>
#include <Winternl.h>
#include <TChar.h>
#include <BCrypt.h>
#include <stdlib.h>
#include <time.h>

#include "Field.h"
#include "Block.h"

/* Defines ********************/

#define MOLD_WIDTH             4
#define MOLD_HEIGHT            4
#define BLOCK_START_POSITION_X 4
#define BLOCK_START_POSITION_Y 0

typedef struct _CURRENT_BLOCK {
    BLOCK_SHAPE   Shape;
    BLOCK_HEADING Heading;
    INT           X, Y;
} CURRENT_BLOCK, * PCURRENT_BLOCK;

/* Global variables ***********/

static const CUBE_TYPE     Block[BLOCK_SHAPE_COUNT][BLOCK_HEADING_COUNT][MOLD_HEIGHT][MOLD_WIDTH] = {
    // T
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_RED, CUBE_TYPE_RED, CUBE_TYPE_RED, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_RED, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_RED, CUBE_TYPE_RED, CUBE_TYPE_RED, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_RED, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_RED, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        }
    },
    // I
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_BLUE, CUBE_TYPE_BLUE, CUBE_TYPE_BLUE, CUBE_TYPE_BLUE},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_BLUE, CUBE_TYPE_BLUE, CUBE_TYPE_BLUE, CUBE_TYPE_BLUE},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_BLUE, CUBE_TYPE_VOID}
        }
    },
    // S
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID}
        }
    },
    // rS
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_PINK, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_PINK},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_PINK, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_PINK, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_PINK},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_PINK, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_PINK, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        }
    },
    // L
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_YELLOW, CUBE_TYPE_YELLOW},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_YELLOW}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_YELLOW, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_YELLOW, CUBE_TYPE_YELLOW, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_YELLOW, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        }
    },
    // rL
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        }
    },
    // Rect
    {
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        },
        {
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_GREEN, CUBE_TYPE_GREEN, CUBE_TYPE_VOID},
            {CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID, CUBE_TYPE_VOID}
        }
    }
};
static       CURRENT_BLOCK CurrentBlock;

/* Private functions **********/

static BOOL BlpIsContact(BLOCK_SHAPE Shape, BLOCK_HEADING Heading, UINT X, UINT Y)
{
    for (UINT i = 0; i < MOLD_HEIGHT; i++)
    {
        for (UINT j = 0; j < MOLD_WIDTH; j++)
        {
            if (Block[Shape][Heading][i][j] == CUBE_TYPE_VOID)
            {
                continue;
            }

            INT x = X + j;
            INT y = Y + i;

            if (x >= FIELD_WIDTH || x < 0 || y >= FIELD_HEIGHT || y < 0)
            {
                return TRUE;
            }

            if (FdGetMainCube(x, y) != CUBE_TYPE_VOID)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

static BOOL BlpCopyCurrentBlockToBufferField(VOID)
{
    for (UINT i = 0; i < MOLD_HEIGHT; i++)
    {
        for (UINT j = 0; j < MOLD_WIDTH; j++)
        {
            if (Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j] != CUBE_TYPE_VOID)
            {
                FdSetBufferCube(CurrentBlock.X + j, CurrentBlock.Y + i, Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j]);
            }
        }
    }
    return TRUE;
}

static BOOL BlpCopyCurrentBlockToMainField(VOID)
{
    for (UINT i = 0; i < MOLD_HEIGHT; i++)
    {
        for (UINT j = 0; j < MOLD_WIDTH; j++)
        {
            if (Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j] != CUBE_TYPE_VOID)
            {
                FdSetMainCube(CurrentBlock.X + j, CurrentBlock.Y + i, Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j]);
            }
        }
    }
    return TRUE;
}

/* Public functions ***********/

BOOL BlInitialize(VOID)
{
    srand((unsigned int)time(NULL));
    return TRUE;
}

BOOL BlUninitialize(VOID)
{
    return TRUE;
}

BOOL BlTurnBlock(VOID)
{
    BLOCK_HEADING heading = CurrentBlock.Heading >= 3 ? 0 : CurrentBlock.Heading + 1;

    if (BlpIsContact(CurrentBlock.Shape, heading, CurrentBlock.X, CurrentBlock.Y))
    {
        return FALSE;
    }

    CurrentBlock.Heading = heading;

    return TRUE;
}

BOOL BlMoveBlockDown(VOID)
{
    if (BlpIsContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X, CurrentBlock.Y + 1))
    {
        return FALSE;
    }

    CurrentBlock.Y++;

    return TRUE;
}

BOOL BlMoveBlockRight(VOID)
{
    if (BlpIsContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X + 1, CurrentBlock.Y))
    {
        return FALSE;
    }

    CurrentBlock.X++;

    return TRUE;
}

BOOL BlMoveBlockLeft(VOID)
{
    if (BlpIsContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X - 1, CurrentBlock.Y))
    {
        return FALSE;
    }

    CurrentBlock.X--;

    return TRUE;
}

BOOL BlSetBlock(BOOL bRandom, BLOCK_SHAPE Shape)
{
    if (bRandom)
    {
        CurrentBlock.Shape = (UINT)(rand() % BLOCK_SHAPE_COUNT);
    }
    else
    {
        CurrentBlock.Shape = Shape;
    }

    CurrentBlock.X = BLOCK_START_POSITION_X;
    CurrentBlock.Y = BLOCK_START_POSITION_Y;
    CurrentBlock.Heading = BLOCK_HEADING_UP;

    if (BlpIsContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X, CurrentBlock.Y))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BLOCK_SHAPE BlGetBlock(VOID)
{
    return CurrentBlock.Shape;
}

BOOL BlFixBlock(VOID)
{
    return BlpCopyCurrentBlockToMainField();
}

BOOL BlDrawBlockInBuffer(VOID)
{
    return BlpCopyCurrentBlockToBufferField();
}

CUBE_TYPE BlGetBlockShapeCube(BLOCK_SHAPE Shape, BLOCK_HEADING Heading, INT X, INT Y)
{
    if (Shape >= BLOCK_SHAPE_COUNT || Heading >= BLOCK_HEADING_COUNT ||
        X >= MOLD_WIDTH || Y >= MOLD_HEIGHT)
    {
        return 0;
    }

    return Block[Shape][Heading][Y][X];
}
