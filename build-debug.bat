@ECHO OFF

REM Script for building the project in 'DEBUG' mode
REM DEBUG mode consists in having optimisations deactivated and symbols activated
REM Scripts inspired by Kohi Game Engine

if not exist .\bin\debug MKDIR .\bin\debug

SET files=src/*.c
SET outputDir=bin/debug/
SET output=%outputDir%app.exe

SET flags=-Wall -Werror -std=c99 -O0 -g
REM -Ipath
SET includeFlags=-Ilibs/GooELog/src/ -Ivendors/sdl/include/
REM -lfile
SET linkerFlags=-Lvendors/build/sdl/lib -lSDL3
SET defines=-DDEBUG

ECHO "Building application in DEBUG mode"
clang %files% %flags% %includeFlags% %linkerFlags% %defines% -o %output%

XCopy "vendors/build/sdl/bin/SDL3.dll" "%outputDir%" /S /F /Y