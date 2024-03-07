@ECHO OFF

REM Script for building the project in 'PRE-RELEASE' mode
REM PRE-RELEASE mode consists in having optimisations and symbols activated
REM Scripts inspired by Kohi Game Engine

if not exist .\bin\pre-release MKDIR .\bin\pre-release

SET files=src/*.c
SET outputDir=bin/pre-release/
SET output=%outputDir%app.exe

SET flags=-Wall -Werror -std=c99 -O3 -g
REM -Ipath
SET includeFlags=-Ilibs/gooeLog/ -Ivendors/build/sdl/include/ -Ivendors/build/sdl_image/include/ -Ivendors/build/sdl_mixer/include/
REM -lfile
SET linkerFlags=-Lvendors/build/sdl/lib -lSDL3 -Lvendors/build/sdl_image/lib -lSDL3_image -Lvendors/build/sdl_mixer/lib -lSDL3_mixer
SET defines=-DPRE_RELEASE

ECHO "Building application in PRE-RELEASE mode"
clang %files% %flags% %includeFlags% %linkerFlags% %defines% -o %output%

XCopy "vendors/build/sdl/bin/SDL3.dll" "%outputDir%" /S /F /Y
XCopy "vendors/build/sdl_image/bin/SDL3_image.dll" "%outputDir%" /S /F /Y
XCopy "vendors/build/sdl_mixer/bin/SDL3_mixer.dll" "%outputDir%" /S /F /Y
XCopy "assets" "%outputDir%/assets/" /S /F /Y
