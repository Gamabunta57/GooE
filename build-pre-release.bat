@ECHO OFF

REM Script for building the project in 'PRE-RELEASE' mode
REM PRE-RELEASE mode consists in having optimisations and symbols activated
REM Scripts inspired by Kohi Game Engine

if not exist .\bin\pre-release MKDIR .\bin\pre-release

SET files=src/*.c
SET output=bin/pre-release/app.exe

SET flags=-Wall -Werror -std=c99 -O3 -g
REM -Ipath
SET includeFlags=-Ilibs/GooELog/src/
REM -lfile
SET linkerFlags= 
SET defines=-DDEBUG

ECHO "Building application in PRE-RELEASE mode"
clang %files% %flags% %includeFlags% %linkerFlags% %defines% -o %output%
