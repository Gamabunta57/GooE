@ECHO OFF

REM Script for building the project in 'RELEASE' mode
REM RELEASE mode consists in having optimisations activated and no symbols
REM Scripts inspired by Kohi Game Engine

if not exist .\bin\release MKDIR .\bin\release

SET files=src/*.c
SET outputDir=bin/release/
SET output=%outputDir%app.exe

SET flags=-Wall -Werror -std=c99 -O3
REM -Ipath
SET includeFlags=-Ilibs/GooELog/src/ -Ivendors/sdl/include/ -Ivendors/sdl_image/include/
REM -lfile
SET linkerFlags=-Lvendors/build/sdl/lib -lSDL3 -Lvendors/build/sdl_image/lib -lSDL3_image
SET defines=-DRELEASE -DGOOE_LOG_NONE

ECHO "Building application in RELEASE mode"
clang %files% %flags% %includeFlags% %linkerFlags% %defines% -o %output%

XCopy "vendors/build/sdl/bin/SDL3.dll" "%outputDir%" /S /F /Y
XCopy "vendors/build/sdl_image/bin/SDL3_image.dll" "%outputDir%" /S /F /Y
XCopy "assets" "%outputDir%/assets/" /S /F /Y
