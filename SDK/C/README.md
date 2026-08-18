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

Optional: 32-bit MinGW (`i686-w64-mingw32-gcc` / MSYS2 `mingw-w64-i686-gcc`) for `build_mingw_ci.bat` (compile gate; same ABI intent as Pelles). `build.bat` still needs `C:\thinBasic\SDK\GCC\Lib\thinCore.lib` and is **not** the Pelles ABI.

> Do **not** use a 64-bit compiler output. ThinBasic cannot load x64 modules.

## How to Build  

### Option A — Pelles C (supported)  

```bat
cd SDK\C
build_pelles.bat
```

Uses `C:\Program Files\PellesC\Bin` (32-bit: `/Tx86-coff`).  
Does **not** link `thinCore.lib`; `tb_thincore.c` resolves thinCore APIs at runtime
(`_thinBasic_LoadSymbol` cdecl + `thinBasic_ParseLong` stdcall). Keywords are registered
as `thinBasic_ReturnCodeLong`.

### Option B — MinGW CI mirror (no `thinCore.lib`)

```bat
cd SDK\C
build_mingw_ci.bat
```

Same sources as Pelles (`tb_thincore.c`, kernel32 only, i686). Used by
[`.github/workflows/ci.yml`](../../.github/workflows/ci.yml) as a compile gate.
CI does **not** run ThinBasic scripts; copy the artifact to `C:\thinBasic\Lib\`
and test locally. Requires MSYS2 MinGW32 or `i686-w64-mingw32-gcc`.

### Option C — MinGW + `thinCore.lib` (not the Pelles ABI path)

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

CURVE REQ/REP (single process):

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkCurve.tbasic"
```

PUSH/PULL (two consoles; start the sink first):

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkPullServer.tbasic"
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkPushClient.tbasic"
```

Single-process PUSH/PULL (also demos `ZmqUnbind` / `ZmqDisconnect`):

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkPushPull.tbasic"
```

Context options / Z85 / CurvePublic:

```bat
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkCtx.tbasic"
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkZ85.tbasic"
C:\thinBasic\thinBasicc.exe "SDK\C\examples\ZmqSdkCurvePublic.tbasic"
```

## ABI contract

Match [thinBasic_MSXML2](https://github.com/ThinBASIC/thinBasic_MSXML2) and official `thinCore.inc`.
Violating any row below has caused `0xC0000005` (ACCESS_VIOLATION) / a black console.

| Topic | Do | Do not |
|-------|----|--------|
| Keyword return | Numeric: cdecl `_thinBasic_LoadSymbol` + `thinBasic_ReturnCodeLong`, `LONG` in **EAX**. String: stdcall `thinBasic_LoadSymbol_FB` + `thinBasic_ReturnString`; `__stdcall` executor returns a byte-BSTR (`SysAllocStringByteLen`) in **EAX** (ThinBasic frees it). | `thinBasic_ReturnNumber`, C `double`, or `EXT` in ST(0). Do not return a C `char*` / libzmq static string. Do not register string keywords with cdecl `_thinBasic_LoadSymbol` (empty result + later AV). GCC `thinBasic_SetReturnString` is not in current `thinCore.dll`. |
| Integers | `thinBasic_ParseLong` (ByRef `LONG`, stdcall). Keep the value as `LONG`. | `thinBasic_ParseDouble`. Do not convert `LONG` → `double` (that writes ST(0); `Printl` / `FNINIT` can hide the crash). |
| Strings | `thinBasic_ParseString` is `Function (…) As Ext`. The C thunk must be typed as returning `double` so the compiler **pops ST(0)**. Length comes from `strlen` on the ByRef pointer. | Treat EAX or the `EXT` as success or as the string length. The `EXT` is the numeric value of the text (0 for `"quotes"`). Leaving ST(0) live will AV on the next string keyword (`ZmqSetsockoptStr` then `ZmqConnect`). |
| Equates | `"%ZMQ_REP"` via cdecl `_thinBasic_AddEquate(char*, char*, DWORD, DWORD)`. | Name without `%`. Do not call the stdcall `thinBasic_AddEquate` with a `DWORD` where PB passes `EXT` (stack misaligned; `USES "ZeroMQ"` crashes). |
| Registration vs parse | Numeric keywords: cdecl `_thinBasic_LoadSymbol`. String keywords: stdcall `thinBasic_LoadSymbol_FB`. Equates: cdecl `_thinBasic_AddEquate`. Parse stdcall `thinBasic_ParseLong` / `thinBasic_ParseString` / `thinBasic_Check*`. | Link Pelles C against GCC `thinCore.lib`. Register `ReturnString` via `_thinBasic_LoadSymbol`. |
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
| `ZmqCtxSet` / `ZmqCtxGet` | Context options (`%ZMQ_IO_THREADS`, `%ZMQ_MAX_SOCKETS`) |
| `ZmqSocket` / `ZmqClose` / `ZmqBind` / `ZmqUnbind` / `ZmqConnect` / `ZmqDisconnect` | Socket |
| `ZmqSend` / `ZmqRecv` | Buffer via `StrPtr` / `VarPtr` |
| `ZmqSendStr(hSock, msg$, flags)` | ThinBasic `String`; returns bytes sent or -1; max 4096 |
| `ZmqRecvStr(hSock, maxLen, flags)` | ThinBasic `String` (BSTR via `LoadSymbol_FB`); maxLen capped at 4096; empty on error/timeout/empty frame |
| `ZmqSetsockoptInt` / `ZmqSetsockoptStr` / `ZmqGetsockoptInt` | Options |
| `ZmqErrno` / `ZmqStrerror` | Error helpers (`ZmqStrerror` returns ASCIIZ pointer) |
| `ZmqStrerrorString(errnum)` | ThinBasic `String` (BSTR); prefer this for `Printl` |
| `ZmqVersionMajor` / `Minor` / `Patch` | Runtime version |
| `ZmqHas(capability)` | `zmq_has` |
| `ZmqCurveKeypair(pubPtr, secPtr)` | Fill two Z85 buffers (`StrPtr` / `VarPtr`, ≥41 bytes); 0 on success |
| `ZmqCurvePublic(pubPtr, secret$)` | Derive Z85 public from 40-char secret; 0 on success |
| `ZmqZ85Encode(destPtr, dataPtr, size)` | size multiple of 4; dest ≥ `size*5/4+1`; returns dest or 0 |
| `ZmqZ85Decode(destPtr, encoded$)` | encoded length multiple of 5; dest ≥ `len*4/5`; returns dest or 0 |

Equates registered by the module (use as `%ZMQ_REQ`, etc.):

- Socket types: `%ZMQ_PAIR` … `%ZMQ_STREAM` (incl. `%ZMQ_PUSH` / `%ZMQ_PULL`)
- Flags / common opts: `%ZMQ_DONTWAIT`, `%ZMQ_SNDMORE`, `%ZMQ_RCVMORE`, `%ZMQ_SUBSCRIBE`, `%ZMQ_UNSUBSCRIBE`,
  `%ZMQ_LINGER`, `%ZMQ_RCVTIMEO`, `%ZMQ_SNDTIMEO`
- Security: `%ZMQ_MECHANISM`, `%ZMQ_PLAIN_*`, `%ZMQ_CURVE_*`, `%ZMQ_ZAP_DOMAIN`,
  `%ZMQ_NULL` / `%ZMQ_PLAIN` / `%ZMQ_CURVE`, `%ZMQ_CURVE_KEYSIZE`, `%ZMQ_CURVE_KEYSIZE_Z85`
- Context: `%ZMQ_IO_THREADS`, `%ZMQ_MAX_SOCKETS`, `%ZMQ_IO_THREADS_DFLT`, `%ZMQ_MAX_SOCKETS_DFLT`

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
├── build_mingw_ci.bat  i686 MinGW CI mirror (no thinCore.lib)
├── verify_dll.ps1    PE i386 + export check (CI)
├── build.bat         MinGW + thinCore.lib (not the Pelles ABI path)
├── include/          thinCore.h (constants), tb_thincore.h, tb_parse.h, …
├── src/              tb_thincore.c, tb_zmq_exec.c, thinBasic_ZeroMQ.c, zmq_dynload.c
├── lib/              thinBasic_ZeroMQ.def
├── examples/         Smoke, REQ/REP, PUB/SUB, Curve, PUSH/PULL, Z85, Ctx
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
