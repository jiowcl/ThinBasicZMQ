@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem -----------------------------------------------------------------------------
rem  CI / MinGW 32-bit build of thinBasic_ZeroMQ.dll (Pelles ABI mirror).
rem
rem  Compiles tb_thincore.c and links kernel32 only (no thinCore.lib).
rem  Requires i686 MinGW (MSYS2 MINGW32 or i686-w64-mingw32-gcc).
rem -----------------------------------------------------------------------------

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "INC=%ROOT%include"
set "DEFDIR=%ROOT%lib"
set "OUT=%ROOT%bin"
set "OBJ=%ROOT%build"

set "OBJDUMP="

if not "%CC%"=="" (
    if exist "%CC%" goto :cc_ok
    where "%CC%" >nul 2>&1
    if not errorlevel 1 goto :cc_ok
)

if exist "C:\msys64\mingw32\bin\gcc.exe" (
    set "CC=C:\msys64\mingw32\bin\gcc.exe"
    goto :cc_ok
)

where i686-w64-mingw32-gcc.exe >nul 2>&1
if not errorlevel 1 (
    set "CC=i686-w64-mingw32-gcc.exe"
    goto :cc_ok
)

where gcc.exe >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%p in ('where gcc.exe') do (
        set "CC=%%p"
        goto :cc_ok
    )
)

echo ERROR: 32-bit MinGW gcc not found.
echo Install MSYS2 mingw-w64-i686-gcc or i686-w64-mingw32-gcc, or set CC to gcc.exe.
exit /b 1

:cc_ok
for /f "delims=" %%m in ('"%CC%" -dumpmachine 2^>nul') do set "MACHINE=%%m"
echo Using compiler: %CC%
echo dumpmachine: %MACHINE%

echo %MACHINE% | findstr /i "x86_64" >nul
if not errorlevel 1 (
    echo ERROR: compiler targets x86_64. ThinBasic modules must be i686.
    exit /b 1
)

echo %MACHINE% | findstr /i "i686 i386" >nul
if errorlevel 1 (
    echo ERROR: compiler is not 32-bit MinGW: %MACHINE%
    exit /b 1
)

for %%d in ("%CC%") do set "CCDIR=%%~dpd"
if exist "%CCDIR%objdump.exe" set "OBJDUMP=%CCDIR%objdump.exe"

if not exist "%OBJ%" mkdir "%OBJ%"
if not exist "%OUT%" mkdir "%OUT%"

set "CFLAGS=-std=c11 -Wall -Wextra -O2 -DWIN32_LEAN_AND_MEAN"

"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\zmq_dynload.c"       -o "%OBJ%\zmq_dynload.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\tb_thincore.c"       -o "%OBJ%\tb_thincore.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\tb_zmq_exec.c"       -o "%OBJ%\tb_zmq_exec.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\thinBasic_ZeroMQ.c"  -o "%OBJ%\thinBasic_ZeroMQ.o"
if errorlevel 1 exit /b 1

set "WINDRES="
if exist "%CCDIR%windres.exe" set "WINDRES=%CCDIR%windres.exe"
if not defined WINDRES where windres.exe >nul 2>&1
if not errorlevel 1 set "WINDRES=windres.exe"
if not defined WINDRES (
    echo ERROR: windres.exe not found (needed for VERSIONINFO).
    exit /b 1
)

"%WINDRES%" -I "%INC%" -i "%DEFDIR%\thinBasic_ZeroMQ.rc" -o "%OBJ%\thinBasic_ZeroMQ_res.o"
if errorlevel 1 exit /b 1

"%CC%" -shared -Wl,--enable-stdcall-fixup ^
    "%OBJ%\zmq_dynload.o" "%OBJ%\tb_thincore.o" "%OBJ%\tb_zmq_exec.o" "%OBJ%\thinBasic_ZeroMQ.o" ^
    "%OBJ%\thinBasic_ZeroMQ_res.o" ^
    "%DEFDIR%\thinBasic_ZeroMQ.def" -lkernel32 ^
    -o "%OUT%\thinBasic_ZeroMQ.dll"
if errorlevel 1 (
    echo.
    echo Link failed. Use a 32-bit MinGW toolchain ^(i686^).
    exit /b 1
)

echo.
echo Built: %OUT%\thinBasic_ZeroMQ.dll
if defined OBJDUMP (
    echo.
    "%OBJDUMP%" -f "%OUT%\thinBasic_ZeroMQ.dll"
)

endlocal
exit /b 0
