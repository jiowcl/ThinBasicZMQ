@echo off
setlocal EnableExtensions

rem -----------------------------------------------------------------------------
rem  CI / MinGW 32-bit build of thinBasic_ZeroMQ.dll (Pelles ABI mirror).
rem
rem  Compiles tb_thincore.c and links kernel32 only (no thinCore.lib).
rem  VERSIONINFO comes from lib\thinBasic_ZeroMQ.rc via windres.
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
set "OBJDUMP="

if exist "C:\msys64\mingw32\bin\gcc.exe" set "CC=C:\msys64\mingw32\bin\gcc.exe"

if not "%CC%"=="" goto :have_cc
where i686-w64-mingw32-gcc.exe >nul 2>&1
if errorlevel 1 goto :try_gcc_noext
for /f "delims=" %%p in ('where i686-w64-mingw32-gcc.exe') do set "CC=%%p"
goto :have_cc

:try_gcc_noext
where i686-w64-mingw32-gcc >nul 2>&1
if errorlevel 1 goto :no_cc
for /f "delims=" %%p in ('where i686-w64-mingw32-gcc') do set "CC=%%p"
goto :have_cc

:no_cc
echo ERROR: 32-bit MinGW gcc not found.
echo Install MSYS2 mingw-w64-i686-gcc or i686-w64-mingw32-gcc, or set CC explicitly.
exit /b 1

:have_cc
for %%d in ("%CC%") do set "CCDIR=%%~dpd"
if exist "%CCDIR%objdump.exe" set "OBJDUMP=%CCDIR%objdump.exe"

for /f "delims=" %%m in ('"%CC%" -dumpmachine 2^>nul') do set "MACHINE=%%m"
echo Using compiler: %CC%
echo dumpmachine: %MACHINE%

echo %MACHINE% | findstr /i "x86_64" >nul
if errorlevel 1 goto :machine_ok
echo ERROR: compiler targets x86_64. ThinBasic modules must be i686.
exit /b 1

:machine_ok
echo %MACHINE% | findstr /i "i686 i386" >nul
if not errorlevel 1 goto :arch_ok
echo ERROR: compiler is not 32-bit MinGW: %MACHINE%
exit /b 1

:arch_ok
if not exist "%OBJ%" mkdir "%OBJ%"
if not exist "%OUT%" mkdir "%OUT%"

rem Sources already define WIN32_LEAN_AND_MEAN; do not pass it on the command line.
set "CFLAGS=-std=c11 -Wall -Wextra -O2"

"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\zmq_dynload.c" -o "%OBJ%\zmq_dynload.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\tb_thincore.c" -o "%OBJ%\tb_thincore.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\tb_zmq_exec.c" -o "%OBJ%\tb_zmq_exec.o"
if errorlevel 1 exit /b 1
"%CC%" %CFLAGS% -I"%INC%" -c "%SRC%\thinBasic_ZeroMQ.c" -o "%OBJ%\thinBasic_ZeroMQ.o"
if errorlevel 1 exit /b 1

if exist "%CCDIR%windres.exe" set "WINDRES=%CCDIR%windres.exe"
if "%WINDRES%"=="" if exist "%CCDIR%i686-w64-mingw32-windres.exe" set "WINDRES=%CCDIR%i686-w64-mingw32-windres.exe"
if not "%WINDRES%"=="" goto :have_windres
echo ERROR: windres.exe not found. Install mingw-w64-i686-binutils.
exit /b 1

:have_windres
"%WINDRES%" -I "%INC%" -i "%DEFDIR%\thinBasic_ZeroMQ.rc" -o "%OBJ%\thinBasic_ZeroMQ_res.o"
if errorlevel 1 exit /b 1

"%CC%" -shared -Wl,--enable-stdcall-fixup "%OBJ%\zmq_dynload.o" "%OBJ%\tb_thincore.o" "%OBJ%\tb_zmq_exec.o" "%OBJ%\thinBasic_ZeroMQ.o" "%OBJ%\thinBasic_ZeroMQ_res.o" "%DEFDIR%\thinBasic_ZeroMQ.def" -lkernel32 -o "%OUT%\thinBasic_ZeroMQ.dll"
if errorlevel 1 goto :link_fail
goto :link_ok

:link_fail
echo.
echo Link failed. Use a 32-bit MinGW toolchain.
exit /b 1

:link_ok
echo.
echo Built: %OUT%\thinBasic_ZeroMQ.dll
if not defined OBJDUMP goto :done
echo.
"%OBJDUMP%" -f "%OUT%\thinBasic_ZeroMQ.dll"

:done
endlocal
exit /b 0
