#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 20

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

typedef UINT CUBE_TYPE;
enum {
    CUBE_TYPE_VOID,
    CUBE_TYPE_RED,
    CUBE_TYPE_ORANGE,
    CUBE_TYPE_YELLOW,
    CUBE_TYPE_GREEN,
    CUBE_TYPE_BLUE,
    CUBE_TYPE_PINK,
    CUBE_TYPE_COUNT
};

BOOL        FdInitialize(VOID);
BOOL        FdUninitialize(VOID);
BOOL        FdMoveBlockDown(VOID);
BOOL        FdMoveBlockLeft(VOID);
BOOL        FdMoveBlockRight(VOID);
BOOL        FdTurnBlock(VOID);
BOOL        FdSetCurrentBlock(BOOL bRandom, BLOCK_SHAPE Shape);
CUBE_TYPE   FdGetScreenCube(INT x, INT y);
BLOCK_SHAPE FdGetCurrentBlockShape(VOID);

#endif /* GAME_FIELD_H */  