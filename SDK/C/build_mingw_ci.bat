@echo off
setlocal EnableExtensions

rem -----------------------------------------------------------------------------
rem  CI / MinGW 32-bit build of thinBasic_ZeroMQ.dll (Pelles ABI mirror).
rem
rem  Compiles:
rem    src\zmq_dynload.c
rem    src\tb_thincore.c
rem    src\tb_zmq_exec.c
rem    src\thinBasic_ZeroMQ.c
rem
rem  Links:
rem    kernel32 only (no thinCore.lib)
rem  and includes VERSIONINFO from:
rem    lib\thinBasic_ZeroMQ.rc
rem -----------------------------------------------------------------------------

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "INC=%ROOT%include"
set "DEFDIR=%ROOT%lib"
set "OUT=%ROOT%bin"
set "OBJ=%ROOT%build"

set "CC="
set "CCDIR="
set "WINDRES="

rem Prefer i686 toolchain explicitly.
if exist "C:\msys64\mingw32\bin\gcc.exe" (
  set "CC=C:\msys64\mingw32\bin\gcc.exe"
)

if "%CC%"=="" (
  where i686-w64-mingw32-gcc >nul 2>&1
  if not errorlevel 1 (
    for /f "delims=" %%p in ('where i686-w64-mingw32-gcc') do set "CC=%%p"
  )
)

if "%CC%"=="" (
  where i686-w64-mingw32-gcc.exe >nul 2>&1
  if not errorlevel 1 (
    for /f "delims=" %%p in ('where i686-w64-mingw32-gcc.exe') do set "CC=%%p"
  )
)

if "%CC%"=="" (
  echo ERROR: 32-bit MinGW gcc not found.
  echo Install MSYS2 mingw-w64-i686-gcc or i686-w64-mingw32-gcc, or set CC explicitly.
  exit /b 1
)

for %%d in ("%CC%") do set "CCDIR=%%~dpd"
if exist "%CCDIR%objdump.exe" set "OBJDUMP=%CCDIR%objdump.exe"

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

if not exist "%OBJ%" mkdir "%OBJ%"
if not exist "%OUT%" mkdir "%OUT%"

set "CFLAGS=-std=c11 -Wall -Wextra -O2 -DWIN32_LEAN_AND_MEAN"

"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\zmq_dynload.c" -o "%OBJ%\zmq_dynload.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\tb_thincore.c" -o "%OBJ%\tb_thincore.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\tb_zmq_exec.c" -o "%OBJ%\tb_zmq_exec.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\thinBasic_ZeroMQ.c" -o "%OBJ%\thinBasic_ZeroMQ.o"
if errorlevel 1 exit /b 1

rem VERSIONINFO resource (windres).
if exist "%CCDIR%windres.exe" (
  set "WINDRES=%CCDIR%windres.exe"
)

if "%WINDRES%"=="" (
  where windres.exe >nul 2>&1
  if not errorlevel 1 set "WINDRES=windres.exe"
)

if "%WINDRES%"=="" (
  echo ERROR: windres.exe not found (needed for VERSIONINFO).
  exit /b 1
)

"%WINDRES%" -I "%INC%" -i "%DEFDIR%\thinBasic_ZeroMQ.rc" -o "%OBJ%\thinBasic_ZeroMQ_res.o"
if errorlevel 1 exit /b 1

rem Link. Single-line to minimize cmd/batch parsing issues.
"%CC%" -shared -Wl,--enable-stdcall-fixup ^
  "%OBJ%\zmq_dynload.o" "%OBJ%\tb_thincore.o" "%OBJ%\tb_zmq_exec.o" "%OBJ%\thinBasic_ZeroMQ.o" "%OBJ%\thinBasic_ZeroMQ_res.o" "%DEFDIR%\thinBasic_ZeroMQ.def" -lkernel32 -o "%OUT%\thinBasic_ZeroMQ.dll"

if errorlevel 1 (
  echo.
  echo Link failed. Use a 32-bit MinGW toolchain.
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

