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
Do **not** use the x64 builds from FreeBasicZMQ `Library\x64`.  
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
└── *.tbasic              # Examples
```

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
| Msg | `ZmqMsgInit` / `ZmqMsgSend` | `LibZmqMsg` | `MsgInit` / `MsgSend` |
| Poll | `ZmqPoll` | `LibZmqPoll` | `DoPoll` |
| Proxy | `ZmqProxy` | `LibZmqProxy` | `DoProxy` |
| Helper | `ZmqSleep` | `LibZmqHelper` | `DoSleep` |

Methods that are not reserved words (for example `CurveKeypair`, `StopwatchStart`, `Steerable`) keep the same short name on the Type.

## How to Build

Building requires ThinBasic Interpreter and test under Windows 10.  
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
Code released under the MIT license.

## TODO

- More examples

## Donation

If this application help you reduce time to coding, you can give me a cup of coffee :)

[![paypal](https://www.paypalobjects.com/en_US/TW/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=3RNMD6Q3B495N&source=url)

[Paypal Me](https://paypal.me/jiowcl?locale.x=zh_TW)
