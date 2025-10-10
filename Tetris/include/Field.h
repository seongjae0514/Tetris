#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#define FIELD_WIDTH  12
#define FIELD_HEIGHT 20

typedef UINT CUBE_TYPE;
enum {
    CUBE_TYPE_VOID,
    CUBE_TYPE_RED,
    CUBE_TYPE_YELLOW,
    CUBE_TYPE_GREEN,
    CUBE_TYPE_BLUE,
    CUBE_TYPE_PINK,
    CUBE_TYPE_PREVIEW,
    CUBE_TYPE_WALL,
    CUBE_TYPE_COUNT
};

BOOL        FdInitialize(VOID);
BOOL        FdUninitialize(VOID);

BOOL        FdClearLine(VOID);

CUBE_TYPE   FdGetBufferCube(INT x, INT y);
BOOL        FdSetBufferCube(INT x, INT y, CUBE_TYPE CubeType);

CUBE_TYPE   FdGetMainCube(INT x, INT y);
BOOL        FdSetMainCube(INT x, INT y, CUBE_TYPE CubeType);

BOOL        FdUpdateBufferField(VOID);

#endif /* GAME_FIELD_H */  