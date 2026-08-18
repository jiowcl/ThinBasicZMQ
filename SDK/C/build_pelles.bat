@echo off
setlocal EnableExtensions

rem -----------------------------------------------------------------------------
rem  Build thinBasic_ZeroMQ.dll with Pelles C (32-bit).
rem  Prerequisite: Pelles C installed (default: C:\Program Files\PellesC)
rem -----------------------------------------------------------------------------

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "INC=%ROOT%include"
set "DEFDIR=%ROOT%lib"
set "OUT=%ROOT%bin"
set "OBJ=%ROOT%build"
set "PELLESC=C:\Program Files\PellesC"

if not exist "%PELLESC%\Bin\pocc.exe" (
    echo ERROR: Pelles C not found at %PELLESC%
    exit /b 1
)

call "%PELLESC%\Bin\povars32.bat" >nul

set "CCFLAGS=/Tx86-coff /std:C11 /Ze /W2 /utf-8 /I"%INC%""

if not exist "%OBJ%" mkdir "%OBJ%"
if not exist "%OUT%" mkdir "%OUT%"

echo Using Pelles C (x86): %PELLESC%\Bin\pocc.exe
pocc %CCFLAGS% /Fo"%OBJ%\zmq_dynload.obj"      "%SRC%\zmq_dynload.c"
if errorlevel 1 exit /b 1
pocc %CCFLAGS% /Fo"%OBJ%\tb_thincore.obj"     "%SRC%\tb_thincore.c"
if errorlevel 1 exit /b 1
pocc %CCFLAGS% /Fo"%OBJ%\tb_zmq_exec.obj"     "%SRC%\tb_zmq_exec.c"
if errorlevel 1 exit /b 1
pocc %CCFLAGS% /Fo"%OBJ%\thinBasic_ZeroMQ.obj" "%SRC%\thinBasic_ZeroMQ.c"
if errorlevel 1 exit /b 1

porc /Fo"%OBJ%\thinBasic_ZeroMQ.res" /I"%INC%" "%DEFDIR%\thinBasic_ZeroMQ.rc"
if errorlevel 1 exit /b 1

polink /DLL /DEF:"%DEFDIR%\thinBasic_ZeroMQ.def" /OUT:"%OUT%\thinBasic_ZeroMQ.dll" ^
    "%OBJ%\zmq_dynload.obj" "%OBJ%\tb_thincore.obj" "%OBJ%\tb_zmq_exec.obj" "%OBJ%\thinBasic_ZeroMQ.obj" ^
    "%OBJ%\thinBasic_ZeroMQ.res" ^
    kernel32.lib
if errorlevel 1 (
    echo.
    echo Link failed.
    exit /b 1
)

echo.
echo Built: %OUT%\thinBasic_ZeroMQ.dll
echo Install: copy "%OUT%\thinBasic_ZeroMQ.dll" "C:\thinBasic\Lib\"
echo.
endlocal
