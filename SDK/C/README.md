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
- **ThinBasic SDK** (`C:\thinBasic\SDK\GCC\Lib\thinCore.lib`)  
- **32-bit MinGW toolchain** (`i686-w64-mingw32-gcc` recommended)  
- **32-bit Pelles C toolchain**  
- Package **x86** `libzmq.dll` + `libsodium.dll` under `ThinBasicZMQ\Library\x86\`  

> Do **not** use a 64-bit compiler output. ThinBasic cannot load x64 modules.

## Build

### Option A — Pelles C (recommended if installed)

```bat
cd SDK\C
build_pelles.bat
```

Uses `C:\Program Files\PellesC\Bin` (32-bit: `/Tx86-coff`).  
Does **not** link `thinCore.lib`; `tb_thincore.c` resolves thinCore APIs at runtime
(`_thinBasic_LoadSymbol` cdecl + `thinBasic_ParseLong` stdcall). Keywords are registered
as `thinBasic_ReturnCodeLong`.

### Option B — MinGW (i686-w64-mingw32-gcc)

```bat
cd SDK\C
build.bat
```

Set `THINBASIC_SDK` inside `build.bat` if ThinBasic is not installed under `C:\thinBasic`.

## Install  

```bat
copy SDK\C\bin\thinBasic_ZeroMQ.dll C:\thinBasic\Lib\
```

## Script usage  

```vb
USES "CONSOLE"
USES "ZeroMQ"

ZmqLibraryInit APP_SourcePath & "..\..\..\ThinBasicZMQ\Library\x86"

Dim hCtx As Long
hCtx = ZmqCtxNew()
' ...
ZmqLibraryShutdown
```

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

## Notes

- Keywords return **Long** via `thinBasic_ReturnCodeLong` (EAX). Official PowerBASIC
  modules use `EXT` / `thinBasic_ReturnNumber`; returning C `double` left values on
  the FPU stack and caused ACCESS_VIOLATION after several consecutive keyword calls.
- Numeric parameters use `thinBasic_ParseLong` (ByRef stdcall), matching
  [thinBasic_MSXML2](https://github.com/ThinBASIC/thinBasic_MSXML2).
- `thinBasic_ParseString` is `As Ext` in the official SDK: the C thunk must
  consume ST(0) or a later keyword call can ACCESS_VIOLATION.
- Numeric equates use the `%` prefix (`%ZMQ_REP`), same as official `thinBasic_AddEquate`.
- `LoadLocalSymbols` returns 0, as in the official module and SDK anatomy docs.  

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
├── build.bat
├── include/          thinCore.h, zmq_dynload.h, tb_parse.h, zmq_enums.h
├── src/              thinBasic_ZeroMQ.c, zmq_dynload.c, tb_zmq_exec.c
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
