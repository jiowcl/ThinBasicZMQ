# ThinBasic SDK module  

C-language ThinBasic module that loads **libzmq.dll** at runtime and exposes a small
set of ZeroMQ keywords. This complements the native `#INCLUDE` API in `ThinBasicZMQ\Core\`.

![C](https://img.shields.io/badge/language-c-blue.svg)
![Status](https://img.shields.io/badge/Status-Experimental-orange)

## Output  

| Artifact | Purpose |
|----------|---------|
| `bin/thinBasic_ZeroMQ.dll` | ThinBasic module (install to `C:\thinBasic\Lib\`) |

## Prerequisites  

- **ThinBasic** 32-bit (x86)  
- **32-bit Pelles C** (supported build: `build_pelles.bat`)  
- Package **x86** `libzmq.dll` + `libsodium.dll` under `ThinBasicZMQ\Library\x86\`  

Optional: 32-bit MinGW (`i686-w64-mingw32-gcc`) and `C:\thinBasic\SDK\GCC\Lib\thinCore.lib` for `build.bat`. That path is not the ABI used by the Pelles module.

> Do **not** use a 64-bit compiler output. ThinBasic cannot load x64 modules.

## Build

### Option A — Pelles C (supported)

```bat
cd SDK\C
build_pelles.bat
```

Uses `C:\Program Files\PellesC\Bin` (32-bit: `/Tx86-coff`).  
Does **not** link `thinCore.lib`; `tb_thincore.c` resolves thinCore APIs at runtime
(`_thinBasic_LoadSymbol` cdecl + `thinBasic_ParseLong` stdcall). Keywords are registered
as `thinBasic_ReturnCodeLong`.

### Option B — MinGW (not the Pelles ABI path)

```bat
cd SDK\C
build.bat
```

Links `thinCore.lib` and does **not** compile `tb_thincore.c`. Set `THINBASIC_SDK`
inside `build.bat` if ThinBasic is not installed under `C:\thinBasic`.

## Install  

```bat
copy SDK\C\bin\thinBasic_ZeroMQ.dll C:\thinBasic\Lib\
```

## Script usage  

```vb
USES "CONSOLE"
USES "ZeroMQ"

ZmqLibraryInit(APP_SourcePath & "..\..\..\ThinBasicZMQ\Library\x86")

Dim hCtx As Long
hCtx = ZmqCtxNew()
' ...
ZmqLibraryShutdown()
```

Keywords require parentheses. Do not `#INCLUDE` `Core\ZeroMQ.inc` in the same script as `USES "ZeroMQ"`.

Run the smoke test:

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkSmoke.tbasic"
```

REQ/REP (two consoles; start the server first):

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkRepServer.tbasic"
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkReqClient.tbasic"
```

PUB/SUB (two consoles; start the publisher first):

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkPubServer.tbasic"
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkSubClient.tbasic"
```

## ABI contract

Match [thinBasic_MSXML2](https://github.com/ThinBASIC/thinBasic_MSXML2) and official `thinCore.inc`.
Violating any row below has caused `0xC0000005` (ACCESS_VIOLATION) / a black console.

| Topic | Do | Do not |
|-------|----|--------|
| Keyword return | Register `thinBasic_ReturnCodeLong`. Executor returns `LONG` in **EAX**. | `thinBasic_ReturnNumber`, C `double`, or 10-byte `EXT` in ST(0). Official PB modules return `EXT`; a C `double` is not the same size and leaves the FPU dirty. |
| Integers | `thinBasic_ParseLong` (ByRef `LONG`, stdcall). Keep the value as `LONG`. | `thinBasic_ParseDouble`. Do not convert `LONG` → `double` (that writes ST(0); `Printl` / `FNINIT` can hide the crash). |
| Strings | `thinBasic_ParseString` is `Function (…) As Ext`. The C thunk must be typed as returning `double` so the compiler **pops ST(0)**. Length comes from `strlen` on the ByRef pointer. | Treat EAX or the `EXT` as success or as the string length. The `EXT` is the numeric value of the text (0 for `"quotes"`). Leaving ST(0) live will AV on the next string keyword (`ZmqSetsockoptStr` then `ZmqConnect`). |
| Equates | `"%ZMQ_REP"` via cdecl `_thinBasic_AddEquate(char*, char*, DWORD, DWORD)`. | Name without `%`. Do not call the stdcall `thinBasic_AddEquate` with a `DWORD` where PB passes `EXT` (stack misaligned; `USES "ZeroMQ"` crashes). |
| Registration vs parse | Load cdecl `_thinBasic_LoadSymbol` / `_thinBasic_AddEquate`. Parse stdcall `thinBasic_ParseLong` / `thinBasic_ParseString` / `thinBasic_Check*`. | Link Pelles C against GCC `thinCore.lib` (calling-convention mismatch). |
| Module entry | Export `_LoadLocalSymbols` / `_UnLoadLocalSymbols` (cdecl). Both return **0**. `GetModuleHandle("thinCore.dll")` first; do not `FreeLibrary` the interpreter’s copy. | Return 1 from `LoadLocalSymbols`. |
| Handles | 32-bit `LONG` (x86 pointers fit). | |

Pelles C does **not** link `thinCore.lib`; `tb_thincore.c` resolves the exports at runtime.

## Keywords  

| Keyword | Notes |
|---------|--------|
| `ZmqLibraryInit(path)` | Load `libzmq.dll` from directory; returns 1/0 |
| `ZmqLibraryShutdown()` | Free libzmq |
| `ZmqLibraryLoaded()` | 1 if loaded |
| `ZmqCtxNew` / `ZmqCtxTerm` / `ZmqCtxShutdown` | Context |
| `ZmqSocket` / `ZmqClose` / `ZmqBind` / `ZmqConnect` | Socket |
| `ZmqSend` / `ZmqRecv` | Buffer via `StrPtr` / `VarPtr` |
| `ZmqSetsockoptInt` / `ZmqSetsockoptStr` / `ZmqGetsockoptInt` | Options |
| `ZmqErrno` / `ZmqStrerror` | Error helpers (`ZmqStrerror` returns ASCIIZ pointer) |
| `ZmqVersionMajor` / `Minor` / `Patch` | Runtime version |
| `ZmqHas(capability)` | `zmq_has` |

Equates registered by the module (use as `%ZMQ_REQ`, etc.): socket types, `%ZMQ_LINGER`,
`%ZMQ_RCVTIMEO`, `%ZMQ_SNDTIMEO`, `%ZMQ_DONTWAIT`, `%ZMQ_SNDMORE`, `%ZMQ_SUBSCRIBE`.

## Native vs SDK  

| | Native (`Core\ZeroMQ.inc`) | SDK (`USES "ZeroMQ"`) |
|--|---------------------------|------------------------|
| Coverage | Full API | Core subset |
| libzmq load | `LibPath.inc` (CWD) | `ZmqLibraryInit` |
| Deploy | Includes + DLLs | `thinBasic_ZeroMQ.dll` + DLLs |
| Best for | Examples, full features | Stable DLL path, future native helpers |

## Source layout  

```
SDK/C/
├── build_pelles.bat  supported (runtime thinCore bind, no thinCore.lib)
├── build.bat         MinGW + thinCore.lib (not the Pelles ABI path)
├── include/          thinCore.h (constants), tb_thincore.h, tb_parse.h, …
├── src/              tb_thincore.c, tb_zmq_exec.c, thinBasic_ZeroMQ.c, zmq_dynload.c
├── lib/              thinBasic_ZeroMQ.def
├── examples/         ZmqSdkSmoke.tbasic, REQ/REP, PUB/SUB
└── bin/              build output (gitignored)
```

## License  

Copyright (c) 2019-2026 Ji-Feng Tsai.  
ThinBasic Copyright (c) Eros Olmi [ThinBASIC Interpreter](https://github.com/ThinBASIC).  
Code released under the MIT license.  

## Donation

If this application help you reduce time to coding, you can give me a cup of coffee :)

[![paypal](https://www.paypalobjects.com/en_US/TW/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3RNMD6Q3B495N&source=url)

[Paypal Me](https://paypal.me/jiowcl?locale.x=zh_TW)
