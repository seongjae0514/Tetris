/* Includes *******************/

#include <Windows.h>
#include <Winternl.h>
#include <TChar.h>
#include <BCrypt.h>

#include <Field.h>

/* Global variables ***********/

static CUBE_TYPE MainField[FIELD_HEIGHT][FIELD_WIDTH];
static CUBE_TYPE BufferField[FIELD_HEIGHT][FIELD_WIDTH];

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

/* Public functions ***********/

BOOL FdInitialize(VOID)
{
    FdpInitializeField();
    return TRUE;
}

BOOL FdUninitialize(VOID)
{
    return TRUE;
}

BOOL FdClearLine(VOID)
{
    return FdpClearLine();
}

CUBE_TYPE FdGetBufferCube(INT x, INT y)
{
    return BufferField[y][x];
}

BOOL FdSetBufferCube(INT x, INT y, CUBE_TYPE CubeType)
{
    BufferField[y][x] = CubeType;
    return TRUE;
}

CUBE_TYPE FdGetMainCube(INT x, INT y)
{
    return MainField[y][x];
}

BOOL FdSetMainCube(INT x, INT y, CUBE_TYPE CubeType)
{
    MainField[y][x] = CubeType;
    return TRUE;
}

BOOL FdUpdateBufferField(VOID)
{
    return FdpFlushMainFieldToBufferField();
}
