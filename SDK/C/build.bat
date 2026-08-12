@echo off
setlocal EnableExtensions

rem -----------------------------------------------------------------------------
rem  Build thinBasic_ZeroMQ.dll (32-bit ThinBasic module, SDK).
rem  Requires: i686-w64-mingw32-gcc (or gcc -m32 with 32-bit MinGW libraries)
rem -----------------------------------------------------------------------------

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "INC=%ROOT%include"
set "LIB=%ROOT%lib"
set "OUT=%ROOT%bin"
set "OBJ=%ROOT%build"

set "THINBASIC_SDK=C:\thinBasic\SDK\GCC\Lib"
if not exist "%THINBASIC_SDK%\thinCore.lib" (
    echo ERROR: thinCore.lib not found at %THINBASIC_SDK%
    echo Set THINBASIC_SDK to your ThinBasic SDK\GCC\Lib folder.
    exit /b 1
)

if exist "%ProgramFiles%\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\i686-w64-mingw32-gcc.exe" (
    set "CC=%ProgramFiles%\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32\bin\i686-w64-mingw32-gcc.exe"
) else (
    where i686-w64-mingw32-gcc >nul 2>&1
    if %ERRORLEVEL%==0 (
        set "CC=i686-w64-mingw32-gcc"
    ) else (
        set "CC=gcc"
        set "CFLAGS_EXTRA=-m32"
    )
)

set "CFLAGS=-std=c11 -Wall -Wextra -Wpedantic -O2 -DWIN32_LEAN_AND_MEAN -I"%INC%" %CFLAGS_EXTRA%"
set "LDFLAGS=-shared -Wl,--enable-stdcall-fixup"
set "LIBS=%THINBASIC_SDK%\thinCore.lib -lkernel32"

if not exist "%OBJ%" mkdir "%OBJ%"
if not exist "%OUT%" mkdir "%OUT%"

echo Using compiler: %CC%
"%CC%" %CFLAGS% -c "%SRC%\zmq_dynload.c"      -o "%OBJ%\zmq_dynload.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -c "%SRC%\tb_zmq_exec.c"     -o "%OBJ%\tb_zmq_exec.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -c "%SRC%\thinBasic_ZeroMQ.c" -o "%OBJ%\thinBasic_ZeroMQ.o"
if errorlevel 1 exit /b 1

"%CC%" %LDFLAGS% ^
    "%OBJ%\zmq_dynload.o" "%OBJ%\tb_zmq_exec.o" "%OBJ%\thinBasic_ZeroMQ.o" ^
    "%LIB%\thinBasic_ZeroMQ.def" %LIBS% ^
    -o "%OUT%\thinBasic_ZeroMQ.dll"
if errorlevel 1 (
    echo.
    echo Build failed. ThinBasic modules must be 32-bit (x86).
    echo Install i686-w64-mingw32-gcc or a 32-bit MinGW toolchain.
    exit /b 1
)

echo.
echo Built: %OUT%\thinBasic_ZeroMQ.dll
echo Install: copy "%OUT%\thinBasic_ZeroMQ.dll" "C:\thinBasic\Lib\"
echo.
endlocal
