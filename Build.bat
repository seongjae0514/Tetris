@echo off
echo Building...

gcc ^
  source/codes/main.c ^
  source/codes/Audio.c ^
  source/codes/Block.c ^
  source/codes/Button.c ^
  source/codes/CurBlock.c ^
  source/codes/Draw.c ^
  source/codes/Field.c ^
  source/codes/GameLoop.c ^
  source/codes/GameSys.c ^
  source/codes/image.c ^
  source/codes/move.c ^
  source/codes/NextBlock.c ^
  source/codes/Paint.c ^
  source/codes/Point.c ^
  source/codes/StoreBlock.c ^
  source/codes/Options.c ^
  source/codes/Tool.c ^
  source/icon/resource.o ^
  -o bin/tetris.exe ^
  -Isource/sdl2/x86_64-w64-mingw32/include ^
  -Isource/sdl2/x86_64-w64-mingw32/include/SDL2 ^
  -Isource/sdl2/x86_64-w64-mingw32-mixer/include ^
  -Lsource/sdl2/x86_64-w64-mingw32/lib ^
  -Lsource/sdl2/x86_64-w64-mingw32-mixer/lib ^
  -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -mwindows -lgdi32 -lcomctl32 -g0 -O2 -s

IF %ERRORLEVEL% NEQ 0 (
  echo.
  echo Build failed.
) ELSE (
  echo.
  echo Build succeeded.
)

pause