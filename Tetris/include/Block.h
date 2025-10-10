#ifndef GAME_BLOCK_H
#define GAME_BLOCK_H

#include <Windows.h>
#include "Field.h"

typedef UINT BLOCK_SHAPE;
enum {
    BLOCK_SHAPE_T,
    BLOCK_SHAPE_I,
    BLOCK_SHAPE_S,
    BLOCK_SHAPE_R_S,
    BLOCK_SHAPE_L,
    BLOCK_SHAPE_R_L,
    BLOCK_SHAPE_RECT,
    BLOCK_SHAPE_COUNT
};

typedef UINT BLOCK_HEADING;
enum {
    BLOCK_HEADING_UP,
    BLOCK_HEADING_RIGHT,
    BLOCK_HEADING_DOWN,
    BLOCK_HEADING_LEFT,
    BLOCK_HEADING_COUNT
};

BOOL        BlInitialize(VOID);
BOOL        BlUninitialize(VOID);
BOOL        BlTurnBlock(VOID);
BOOL        BlMoveBlockDown(VOID);
BOOL        BlMoveBlockRight(VOID);
BOOL        BlMoveBlockLeft(VOID);
BOOL        BlSetBlock(BOOL bRandom, BLOCK_SHAPE Shape);
BLOCK_SHAPE BlGetBlock(VOID);
BOOL        BlFixBlock(VOID);
BOOL        BlDrawBlockInBuffer(VOID);
CUBE_TYPE   BlGetBlockShapeCube(BLOCK_SHAPE Shape, BLOCK_HEADING Heading, INT X, INT Y);

#endif