# ThinBasicZMQ

ZMQ Wrapper for ThinBasic Programming Language.

![GitHub](https://img.shields.io/github/license/jiowcl/ThinBasicZMQ.svg)
![ThinBasic](https://img.shields.io/badge/language-ThinBasic-blue.svg)
![Dependency](https://img.shields.io/badge/ZeroMQ-libzmq-FF6600?style=flat-square&logo=zeromq&logoColor=white)

## Environment

- Windows 10 above (recommend)
- ThinBasic 1.14 above (recommend) — **32-bit (x86) runtime only**
- [ZeroMQ](https://github.com/zeromq)

### Architecture note

ThinBasic itself is a **32-bit** process, so only **x86** `libzmq.dll` / `libsodium.dll` can be loaded.  
Do **not** use the x64 builds from FreeBasicZMQ/PureBasicZMQ `Library\x64`.  
You can still talk to x64 ZeroMQ peers over `tcp://` (wire protocol is architecture-independent).

## Package layout

```
ThinBasicZMQ/
├── Core/                 # Include library
│   ├── ZeroMQ.inc        # Umbrella: Enums + LibPath + all procedural Declares
│   ├── ZeroMQWrapper.inc # Includes ZeroMQ.inc + LibZmq* Type façade
│   ├── Enums.inc
│   ├── LibPath.inc       # Sets CWD to Library\x86 (loaded by ZeroMQ.inc)
│   ├── Runtime.inc
│   ├── Context.inc
│   ├── Socket.inc
│   ├── Msg.inc
│   ├── Poll.inc
│   ├── Proxy.inc
│   ├── Security.inc
│   └── Helper.inc
├── Library/
│   └── x86/              # libzmq.dll + libsodium.dll (x86)
├── SDK/
│   └── C/                # ThinBasic SDK module thinBasic_ZeroMQ.dll
├── Module/               # Type-wrapper examples (Pub/Sub, REQ/REP)
├── Weather/              # Weather-style PUB/SUB scenario
└── *.tbasic              # Procedural examples (incl. Monitor, Thread, Curve, Plain, …)
```

Samples:

| Script | Notes |
|--------|--------|
| `PubServer.tbasic` / `SubClient.tbasic` | PUB/SUB (procedural) |
| `RepServer.tbasic` / `ReqClient.tbasic` | REQ/REP (procedural) |
| `Module\*.tbasic` | Same patterns via `ZeroMQWrapper.inc` |
| `Monitor.tbasic` | `zmq_socket_monitor` (connect PAIR **before** Bind) |
| `Thread.tbasic` | `zmq_threadstart` / `zmq_threadclose` (`Function … CDECL`; join before other script work) |
| `Proxy.tbasic` / `Poll.tbasic` | Proxy and poll |
| `Curve.tbasic` | CURVE end-to-end REQ/REP |
| `Plain.tbasic` | PLAIN + in-process ZAP (single-threaded poll) |
| `Weather\WeatherPubServer.tbasic` / `WeatherSubClient.tbasic` | Weather-style JSON PUB/SUB |
| `CurveKeypair.tbasic` / `Z85.tbasic` | Security helpers |
| `SDK\C\examples\ZmqSdkSmoke.tbasic` | ThinBasic SDK module smoke test (`USES "ZeroMQ"`) |
| `SDK\C\examples\ZmqSdkRepServer.tbasic` / `ZmqSdkReqClient.tbasic` | SDK REQ/REP (`USES "ZeroMQ"`, port 1740) |
| `SDK\C\examples\ZmqSdkPubServer.tbasic` / `ZmqSdkSubClient.tbasic` | SDK PUB/SUB (`USES "ZeroMQ"`, port 1741, topic `quotes`) |
| `SDK\C\examples\ZmqSdkCurve.tbasic` | SDK CURVE REQ/REP (`USES "ZeroMQ"`, port 1742) |
| `SDK\C\examples\ZmqSdkPullServer.tbasic` / `ZmqSdkPushClient.tbasic` | SDK PUSH/PULL (`USES "ZeroMQ"`, port 1743) |
| `SDK\C\examples\ZmqSdkPushPull.tbasic` | SDK PUSH/PULL single process + `ZmqUnbind` / `ZmqDisconnect` |
| `SDK\C\examples\ZmqSdkZ85.tbasic` | SDK Z85 encode/decode (`USES "ZeroMQ"`) |
| `SDK\C\examples\ZmqSdkCurvePublic.tbasic` | SDK `ZmqCurvePublic` vs `ZmqCurveKeypair` |
| `SDK\C\examples\ZmqSdkCtx.tbasic` | SDK `ZmqCtxSet` / `ZmqCtxGet` |

## Native vs ThinBasic SDK  

| | Native (`Core\ZeroMQ.inc`) | SDK (`USES "ZeroMQ"`) |
|--|---------------------------|------------------------|
| Coverage | Full procedural + Type wrapper | Core subset + `ZmqLibraryInit` |
| libzmq load | `LibPath.inc` (process CWD) | `ZmqLibraryInit(path)` in C module |
| Build | None (includes only) | `SDK\C\build_pelles.bat` → `thinBasic_ZeroMQ.dll` |
| Constants | `$ZMQ_*` in `Enums.inc` | `%ZMQ_*` registered by the module |

See [SDK/C/README.md](SDK/C/README.md) for build, install, and the **ABI contract** (`LONG` in EAX, string BSTR via `LoadSymbol_FB`, `ParseLong`, `ParseString As Ext`). Do not mix `#INCLUDE` Core files with `USES "ZeroMQ"`.

Typical includes:

```vb
#INCLUDE Once ".\Core\ZeroMQ.inc"         ' procedural API
#INCLUDE Once ".\Core\ZeroMQWrapper.inc"  ' procedural + Type wrapper
```

## Procedural vs Type wrapper

Some ThinBasic reserved words cannot be Type method names, so the wrapper uses renamed methods:

| Area | Procedural | Type | Wrapper method |
|------|------------|------|----------------|
| Runtime | `ZmqErrno` | `LibZmqRuntime` | `GetErrno` |
| Runtime | `ZmqStrerrorString` | `LibZmqRuntime` | `GetStrerror` |
| Runtime | `ZmqVersion` | `LibZmqRuntime` | `GetVersion` |
| Runtime | `ZmqHas` | `LibZmqRuntime` | `HasCap` |
| Context | `ZmqCtxNew` | `LibZmqContext` | `NewCtx` |
| Context | `ZmqCtxTerm` | `LibZmqContext` | `CtxTerm` |
| Context | `ZmqCtxShutdown` | `LibZmqContext` | `Shutdown` |
| Context | `ZmqCtxSet` / `ZmqCtxGet` | `LibZmqContext` | `CtxSet` / `CtxGet` |
| Socket | `ZmqSocket` | `LibZmqSocket` | `NewSocket` |
| Socket | `ZmqBind` / `ZmqConnect` | `LibZmqSocket` | `SockBind` / `SockConnect` |
| Socket | `ZmqSend` / `ZmqRecv` | `LibZmqSocket` | `SockSend` / `SockRecv` |
| Socket | `ZmqClose` | `LibZmqSocket` | `SockClose` |
| Socket | `ZmqSetsockoptInt` | `LibZmqSocket` | `SockSetsockoptInt` |
| Socket | `ZmqSetsockoptStr` | `LibZmqSocket` | `SockSetsockoptStr` |
| Msg | `ZmqMsgInit` / `ZmqMsgSend` | `LibZmqMsg` | `MsgInit` / `MsgSend` |
| Poll | `ZmqPoll` | `LibZmqPoll` | `DoPoll` |
| Proxy | `ZmqProxy` | `LibZmqProxy` | `DoProxy` |
| Helper | `ZmqSleep` | `LibZmqHelper` | `DoSleep` |

Methods that are not reserved words (for example `CurveKeypair`, `StopwatchStart`, `Steerable`) keep the same short name on the Type.

## How to Build

Native examples need only the ThinBasic interpreter (32-bit) and `Library\x86`.  
The SDK module is built with Pelles C: `SDK\C\build_pelles.bat` (see [SDK/C/README.md](SDK/C/README.md)).  
CI compiles the same sources with i686 MinGW (`SDK\C\build_mingw_ci.bat`, no `thinCore.lib`) via [`.github/workflows/ci.yml`](.github/workflows/ci.yml); it does not run ThinBasic scripts.  
Type features require ThinBasic 1.4.0 and above.  

## Example

Publisher Server

```vb
Uses "CONSOLE"

#INCLUDE Once ".\Core\ZeroMQWrapper.inc"

Dim ZmqContextRec As LibZmqContext
Dim ZmqSocketRec As LibZmqSocket
Dim ZmqRuntimeRec As LibZmqRuntime

Function RndRange(ByVal first As Double, ByVal last As Double) As Double
  Function = Rnd * (last - first) + first
End Function

Global lpszServerAddr As String
Global hContext As Long
Global hSocket As Long
Global Rc As Long
Global lpszTopic As String
Global lpszSendMessage As String
Global nBid As Long
Global nAsk As Long

lpszServerAddr = "tcp://*:1689"

hContext = ZmqContextRec.NewCtx()
hSocket = ZmqSocketRec.NewSocket(hContext, $ZMQ_PUB)
Rc = ZmqSocketRec.SockBind(hSocket, lpszServerAddr)

If Rc <> 0 Then
  Printl("Bind failed: " & ZmqRuntimeRec.GetStrerror(ZmqRuntimeRec.GetErrno()))
Else
  Printl("Bind an IP address: " & lpszServerAddr)

  Sleep 200
  Randomize

  While %TRUE
    lpszTopic = "quotes"
    nBid = Int(RndRange(1000, 9000))
    nAsk = Int(RndRange(1000, 9000))
    lpszSendMessage = "Bid:" & Str$(nBid) & ",Ask:" & Str$(nAsk)

    If ZmqSocketRec.SockSend(hSocket, StrPtr(lpszTopic), Len(lpszTopic), $ZMQ_SNDMORE) = -1 Then
      Printl("Send topic failed: " & ZmqRuntimeRec.GetStrerror(ZmqRuntimeRec.GetErrno()))
    ElseIf ZmqSocketRec.SockSend(hSocket, StrPtr(lpszSendMessage), Len(lpszSendMessage), 0) = -1 Then
      Printl("Send message failed: " & ZmqRuntimeRec.GetStrerror(ZmqRuntimeRec.GetErrno()))
    Else
      Printl("Published: " & lpszSendMessage)
    End If

    Sleep 100
  Wend
End If

ZmqSocketRec.SockClose(hSocket)
ZmqContextRec.Shutdown(hContext)
```

Subscribe Client

```vb
Uses "CONSOLE"

#INCLUDE Once ".\Core\ZeroMQWrapper.inc"

Dim ZmqContextRec As LibZmqContext
Dim ZmqSocketRec As LibZmqSocket
Dim ZmqRuntimeRec As LibZmqRuntime

Global lpszServerAddr As String
Global hContext As Long
Global hSocket As Long
Global lpszSubscribe As String
Global Rc As Long
Global lpszTopicBuffer As Asciiz * 256
Global lpszRecvBuffer As Asciiz * 256
Global TopicBytes As Long
Global MessageBytes As Long

lpszServerAddr = "tcp://localhost:1689"
lpszSubscribe = "quotes"

hContext = ZmqContextRec.NewCtx()
hSocket = ZmqSocketRec.NewSocket(hContext, $ZMQ_SUB)

ZmqSocketRec.SockSetsockopt(hSocket, $ZMQ_SUBSCRIBE, StrPtr(lpszSubscribe), Len(lpszSubscribe))
Rc = ZmqSocketRec.SockConnect(hSocket, lpszServerAddr)

If Rc <> 0 Then
  Printl("Connect failed: " & ZmqRuntimeRec.GetStrerror(ZmqRuntimeRec.GetErrno()))
Else
  Printl("Connected: " & lpszServerAddr & " (subscribe=" & lpszSubscribe & ")")

  While %TRUE
    TopicBytes = ZmqSocketRec.SockRecv(hSocket, VarPtr(lpszTopicBuffer), SizeOf(lpszTopicBuffer), 0)

    If TopicBytes = -1 Then
      Printl("Recv topic failed: " & ZmqRuntimeRec.GetStrerror(ZmqRuntimeRec.GetErrno()))
    Else
      MessageBytes = ZmqSocketRec.SockRecv(hSocket, VarPtr(lpszRecvBuffer), SizeOf(lpszRecvBuffer), 0)

      If MessageBytes = -1 Then
        Printl("Recv message failed: " & ZmqRuntimeRec.GetStrerror(ZmqRuntimeRec.GetErrno()))
      Else
        Printl(Left$(lpszRecvBuffer, MessageBytes))
      End If
    End If
  Wend
End If

ZmqSocketRec.SockClose(hSocket)
ZmqContextRec.Shutdown(hContext)
```

## License

Copyright (c) 2019-2026 Ji-Feng Tsai.  
ThinBasic Copyright (c) Eros Olmi [ThinBASIC Interpreter](https://github.com/ThinBASIC).  
Code released under the MIT license.  

## Donation

If this application help you reduce time to coding, you can give me a cup of coffee :)

[![paypal](https://www.paypalobjects.com/en_US/TW/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3RNMD6Q3B495N&source=url)

[Paypal Me](https://paypal.me/jiowcl?locale.x=zh_TW)
