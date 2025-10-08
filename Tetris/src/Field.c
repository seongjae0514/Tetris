/* Includes *******************/

#include <Windows.h>
#include <Winternl.h>
#include <TChar.h>
#include <BCrypt.h>

#include <Field.h>

/* Defines ********************/

#define MOLD_WIDTH             4
#define MOLD_HEIGHT            4
#define BLOCK_START_POSITION_X 4
#define BLOCK_START_POSITION_Y 0

typedef struct _CURRENT_BLOCK {
    BLOCK_SHAPE   Shape;
    BLOCK_HEADING Heading;
    INT           X, Y;
} CURRENT_BLOCK, *PCURRENT_BLOCK;

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
static       CUBE_TYPE     MainField[FIELD_HEIGHT][FIELD_WIDTH];
static       CUBE_TYPE     BufferField[FIELD_HEIGHT][FIELD_WIDTH];
static       CURRENT_BLOCK CurrentBlock;

/* Private functions **********/

static BOOL FdpInitializeField(VOID)
{
    for (UINT i = 0; i < FIELD_HEIGHT; i++)
    {
        for (UINT j = 0; j < FIELD_WIDTH; j++)
        {
            MainField[i][j] = BufferField[i][j] = CUBE_TYPE_VOID;
        }
    }
    return TRUE;
}

static BOOL FdpSetCurrentBlock(BOOL bRandom, BLOCK_SHAPE Shape)
{
    CurrentBlock.X       = BLOCK_START_POSITION_X;
    CurrentBlock.Y       = BLOCK_START_POSITION_Y;
    CurrentBlock.Heading = BLOCK_HEADING_UP;

    if (bRandom)
    {

        NTSTATUS status = BCryptGenRandom(
            NULL,
            (PUCHAR)&CurrentBlock.Shape,
            sizeof(CurrentBlock.Shape),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG
        );

        if (NT_ERROR(status))
        {
            return FALSE;
        }

        CurrentBlock.Shape %= BLOCK_SHAPE_COUNT;
    }
    else
    {
        CurrentBlock.Shape = Shape;
    }

    return TRUE;
}

static BOOL FdpIsBlockContact(BLOCK_SHAPE Shape, BLOCK_HEADING Heading, INT X, INT Y)
{
    for (INT i = 0; i < MOLD_HEIGHT; i++)
    {
        for (INT j = 0; j < MOLD_WIDTH; j++)
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

            if (MainField[y][x] != CUBE_TYPE_VOID)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

static BOOL FdpCopyCurrentBlockToBufferField(VOID)
{
    for (INT i = 0; i < MOLD_HEIGHT; i++)
    {
        for (INT j = 0; j < MOLD_WIDTH; j++)
        {
            if (Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j] == CUBE_TYPE_VOID)
            {
                continue;
            }

            BufferField[CurrentBlock.Y + i][CurrentBlock.X + j] = Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j];
        }
    }

    return TRUE;
}

static BOOL FdpCopyCurrentBlockToMainField(VOID)
{
    for (INT i = 0; i < MOLD_HEIGHT; i++)
    {
        for (INT j = 0; j < MOLD_WIDTH; j++)
        {
            if (Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j] == CUBE_TYPE_VOID)
            {
                continue;
            }

            MainField[CurrentBlock.Y + i][CurrentBlock.X + j] = Block[CurrentBlock.Shape][CurrentBlock.Heading][i][j];
        }
    }

    return TRUE;
}

static BOOL FdpFlushMainFieldToBufferField(VOID)
{
    for (INT i = 0; i < FIELD_HEIGHT; i++)
    {
        for (INT j = 0; j < FIELD_WIDTH; j++)
        {
            BufferField[i][j] = MainField[i][j];
        }
    }
    return TRUE;
}

static BOOL FdpIsLineComplete(UINT LineIndex)
{
    for (INT i = 0; i < FIELD_WIDTH; i++)
    {
        if (MainField[LineIndex][i] == CUBE_TYPE_VOID)
        {
            return FALSE;
        }
    }
    return TRUE;
}

static BOOL FdpDownMainField(UINT LineIndex)
{
    if (LineIndex == 0)
    {
        return FALSE;
    }

    for (UINT i = LineIndex; i > 0; i--)
    {
        for (INT j = 0; j < FIELD_WIDTH; j++)
        {
            MainField[i][j] = MainField[i - 1][j];
        }
    }

    return TRUE;
}

static BOOL FdpClearLine(VOID)
{
    for (INT i = 1; i < FIELD_HEIGHT; i++)
    {
        if (FdpIsLineComplete(i))
        {
            FdpDownMainField(i);
        }
    }
    return TRUE;
}

static BOOL FdpUpdateBufferField(VOID)
{
    FdpFlushMainFieldToBufferField();
    FdpCopyCurrentBlockToBufferField();
    return TRUE;
}

/* Public functions ***********/

BOOL FdInitialize(VOID)
{
    FdpInitializeField();
    FdpSetCurrentBlock(TRUE, 0);
    FdpUpdateBufferField();
    return TRUE;
}

BOOL FdUninitialize(VOID)
{
    return TRUE;
}

BOOL FdMoveBlockDown(VOID)
{
    if (FdpIsBlockContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X, CurrentBlock.Y + 1))
    {
        FdpCopyCurrentBlockToMainField();
        FdpClearLine();
        FdpSetCurrentBlock(TRUE, 0);
        FdpUpdateBufferField();
        return FALSE;
    }

    CurrentBlock.Y++;

    FdpFlushMainFieldToBufferField();
    FdpCopyCurrentBlockToBufferField();

    return TRUE;
}

BOOL FdMoveBlockLeft(VOID)
{
    if (FdpIsBlockContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X - 1, CurrentBlock.Y))
    {
        return FALSE;
    }

    CurrentBlock.X--;

    FdpFlushMainFieldToBufferField();
    FdpCopyCurrentBlockToBufferField();

    return TRUE;
}

BOOL FdMoveBlockRight(VOID)
{
    if (FdpIsBlockContact(CurrentBlock.Shape, CurrentBlock.Heading, CurrentBlock.X + 1, CurrentBlock.Y))
    {
        return FALSE;
    }

    CurrentBlock.X++;

    FdpFlushMainFieldToBufferField();
    FdpCopyCurrentBlockToBufferField();

    return TRUE;
}

BOOL FdTurnBlock(VOID)
{
    BLOCK_HEADING heading = CurrentBlock.Heading >= 3 ? 0 : CurrentBlock.Heading + 1;

    if (FdpIsBlockContact(CurrentBlock.Shape, heading, CurrentBlock.X, CurrentBlock.Y))
    {
        return FALSE;
    }

    CurrentBlock.Heading = heading;

    FdpFlushMainFieldToBufferField();
    FdpCopyCurrentBlockToBufferField();

    return TRUE;
}

BOOL FdSetCurrentBlock(BOOL bRandom, BLOCK_SHAPE Shape)
{
    if (!FdpSetCurrentBlock(bRandom, Shape))
    {
        return FALSE;
    }

    FdpUpdateBufferField();

    return TRUE;
}

BOOL FdClearLine(VOID)
{
    return FdpClearLine();
}

CUBE_TYPE FdGetScreenCube(INT x, INT y)
{
    return BufferField[y][x];
}

BLOCK_SHAPE FdGetCurrentBlockShape(VOID)
{
    return CurrentBlock.Shape;
}
