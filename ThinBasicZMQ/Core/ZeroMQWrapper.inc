'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Type

Type LibZmqRuntime
  Public

  ' <summary>
  ' GetErrno
  ' </summary>
  ' <returns>Returns long.</returns>
  Function GetErrno() As Long
    Function = ZmqErrno()
  End Function

  ' <summary>
  ' GetStrerror
  ' </summary>
  ' <param name="errnum">Long</param>
  ' <returns>Returns string.</returns>
  Function GetStrerror(ByVal errnum As Long) As String
    Function = ZmqStrerrorString(errnum)
  End Function

  ' <summary>
  ' GetVersion
  ' </summary>
  ' <param name="major">Long</param>
  ' <param name="minor">Long</param>
  ' <param name="patch">Long</param>
  ' <returns>Returns void.</returns>
  Sub GetVersion(ByRef major As Long, ByRef minor As Long, ByRef patch As Long)
    ZmqVersion(major, minor, patch)
  End Sub

  ' <summary>
  ' HasCap
  ' </summary>
  ' <param name="capability">String</param>
  ' <returns>Returns long.</returns>
  Function HasCap(ByVal capability As String) As Long
    Function = ZmqHas(capability)
  End Function
End Type

Type LibZmqContext
  Public

  ' <summary>
  ' NewCtx
  ' </summary>
  ' <returns>Returns context handle.</returns>
  Function NewCtx() As Long
    Function = ZmqCtxNew()
  End Function

  ' <summary>
  ' CtxTerm
  ' </summary>
  ' <param name="context">Long</param>
  ' <returns>Returns long.</returns>
  Function CtxTerm(ByVal context As Long) As Long
    Function = ZmqCtxTerm(context)
  End Function

  ' <summary>
  ' Shutdown
  ' </summary>
  ' <param name="context">Long</param>
  ' <returns>Returns long.</returns>
  Function Shutdown(ByVal context As Long) As Long
    Function = ZmqCtxShutdown(context)
  End Function

  ' <summary>
  ' CtxSet
  ' </summary>
  ' <param name="context">Long</param>
  ' <param name="options">Long</param>
  ' <param name="optval">Long</param>
  ' <returns>Returns long.</returns>
  Function CtxSet(ByVal context As Long, ByVal options As Long, ByVal optval As Long) As Long
    Function = ZmqCtxSet(context, options, optval)
  End Function

  ' <summary>
  ' CtxGet
  ' </summary>
  ' <param name="context">Long</param>
  ' <param name="options">Long</param>
  ' <returns>Returns long.</returns>
  Function CtxGet(ByVal context As Long, ByVal options As Long) As Long
    Function = ZmqCtxGet(context, options)
  End Function
End Type

Type LibZmqSocket
  Public

  ' <summary>
  ' NewSocket
  ' </summary>
  ' <param name="context">Long</param>
  ' <param name="stype">Long</param>
  ' <returns>Returns socket handle.</returns>
  Function NewSocket(ByVal context As Long, ByVal stype As Long) As Long
    Function = ZmqSocket(context, stype)
  End Function

  ' <summary>
  ' SockMonitor
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="addr">String</param>
  ' <param name="events">Long</param>
  ' <returns>Returns long.</returns>
  Function SockMonitor(ByVal hSocket As Long, ByVal addr As String, ByVal events As Long) As Long
    Function = ZmqSocketMonitor(hSocket, addr, events)
  End Function

  ' <summary>
  ' SockBind
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="addr">String</param>
  ' <returns>Returns long.</returns>
  Function SockBind(ByVal hSocket As Long, ByVal addr As String) As Long
    Function = ZmqBind(hSocket, addr)
  End Function

  ' <summary>
  ' SockUnBind
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="addr">String</param>
  ' <returns>Returns long.</returns>
  Function SockUnBind(ByVal hSocket As Long, ByVal addr As String) As Long
    Function = ZmqUnBind(hSocket, addr)
  End Function

  ' <summary>
  ' SockRecv
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="buf">Long</param>
  ' <param name="buflen">DWord</param>
  ' <param name="flags">Long</param>
  ' <returns>Returns long.</returns>
  Function SockRecv(ByVal hSocket As Long, ByVal buf As Long, ByVal buflen As DWord, ByVal flags As Long) As Long
    Function = ZmqRecv(hSocket, buf, buflen, flags)
  End Function

  ' <summary>
  ' SockSend
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="buf">Long</param>
  ' <param name="buflen">DWord</param>
  ' <param name="flags">Long</param>
  ' <returns>Returns long.</returns>
  Function SockSend(ByVal hSocket As Long, ByVal buf As Long, ByVal buflen As DWord, ByVal flags As Long) As Long
    Function = ZmqSend(hSocket, buf, buflen, flags)
  End Function

  ' <summary>
  ' SockSendConst
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="buf">Long</param>
  ' <param name="buflen">DWord</param>
  ' <param name="flags">Long</param>
  ' <returns>Returns long.</returns>
  Function SockSendConst(ByVal hSocket As Long, ByVal buf As Long, ByVal buflen As DWord, ByVal flags As Long) As Long
    Function = ZmqSendConst(hSocket, buf, buflen, flags)
  End Function

  ' <summary>
  ' SockConnect
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="addr">String</param>
  ' <returns>Returns long.</returns>
  Function SockConnect(ByVal hSocket As Long, ByVal addr As String) As Long
    Function = ZmqConnect(hSocket, addr)
  End Function

  ' <summary>
  ' SockDisConnect
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="addr">String</param>
  ' <returns>Returns long.</returns>
  Function SockDisConnect(ByVal hSocket As Long, ByVal addr As String) As Long
    Function = ZmqDisConnect(hSocket, addr)
  End Function

  ' <summary>
  ' SockSetsockopt
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="options">Long</param>
  ' <param name="optval">Long</param>
  ' <param name="optvallen">DWord</param>
  ' <returns>Returns long.</returns>
  Function SockSetsockopt(ByVal hSocket As Long, ByVal options As Long, ByVal optval As Long, ByVal optvallen As DWord) As Long
    Function = ZmqSetsockopt(hSocket, options, optval, optvallen)
  End Function

  ' <summary>
  ' SockSetsockoptInt
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="options">Long</param>
  ' <param name="optval">Long</param>
  ' <returns>Returns long.</returns>
  Function SockSetsockoptInt(ByVal hSocket As Long, ByVal options As Long, ByVal optval As Long) As Long
    Function = ZmqSetsockoptInt(hSocket, options, optval)
  End Function

  ' <summary>
  ' SockGetsockopt
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="options">Long</param>
  ' <param name="optval">Long</param>
  ' <param name="optvallen">DWord</param>
  ' <returns>Returns long.</returns>
  Function SockGetsockopt(ByVal hSocket As Long, ByVal options As Long, ByVal optval As Long, ByRef optvallen As DWord) As Long
    Function = ZmqGetsockopt(hSocket, options, optval, optvallen)
  End Function

  ' <summary>
  ' SockGetsockoptInt
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <param name="options">Long</param>
  ' <param name="optval">Long</param>
  ' <returns>Returns long.</returns>
  Function SockGetsockoptInt(ByVal hSocket As Long, ByVal options As Long, ByRef optval As Long) As Long
    Function = ZmqGetsockoptInt(hSocket, options, optval)
  End Function

  ' <summary>
  ' SockClose
  ' </summary>
  ' <param name="hSocket">Long</param>
  ' <returns>Returns long.</returns>
  Function SockClose(ByVal hSocket As Long) As Long
    Function = ZmqClose(hSocket)
  End Function
End Type

Type LibZmqMsg
  Public

  ' <summary>
  ' MsgInit
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <returns>Returns long.</returns>
  Function MsgInit(ByRef msg As ZmqMsgT) As Long
    Function = ZmqMsgInit(msg)
  End Function

  ' <summary>
  ' MsgInitSize
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="msgsize">DWord</param>
  ' <returns>Returns long.</returns>
  Function MsgInitSize(ByRef msg As ZmqMsgT, ByVal msgsize As DWord) As Long
    Function = ZmqMsgInitSize(msg, msgsize)
  End Function

  ' <summary>
  ' MsgInitData
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="msgdata">Long</param>
  ' <param name="msgsize">DWord</param>
  ' <param name="ffn">Long</param>
  ' <param name="hint">Long</param>
  ' <returns>Returns long.</returns>
  Function MsgInitData(ByRef msg As ZmqMsgT, ByVal msgdata As Long, ByVal msgsize As DWord, ByVal ffn As Long, ByVal hint As Long) As Long
    Function = ZmqMsgInitData(msg, msgdata, msgsize, ffn, hint)
  End Function

  ' <summary>
  ' MsgSend
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="hSocket">Long</param>
  ' <param name="flags">Long</param>
  ' <returns>Returns long.</returns>
  Function MsgSend(ByRef msg As ZmqMsgT, ByVal hSocket As Long, ByVal flags As Long) As Long
    Function = ZmqMsgSend(msg, hSocket, flags)
  End Function

  ' <summary>
  ' MsgRecv
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="hSocket">Long</param>
  ' <param name="flags">Long</param>
  ' <returns>Returns long.</returns>
  Function MsgRecv(ByRef msg As ZmqMsgT, ByVal hSocket As Long, ByVal flags As Long) As Long
    Function = ZmqMsgRecv(msg, hSocket, flags)
  End Function

  ' <summary>
  ' MsgClose
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <returns>Returns long.</returns>
  Function MsgClose(ByRef msg As ZmqMsgT) As Long
    Function = ZmqMsgClose(msg)
  End Function

  ' <summary>
  ' MsgMove
  ' </summary>
  ' <param name="destmsg">ZmqMsgT</param>
  ' <param name="srcmsg">ZmqMsgT</param>
  ' <returns>Returns long.</returns>
  Function MsgMove(ByRef destmsg As ZmqMsgT, ByRef srcmsg As ZmqMsgT) As Long
    Function = ZmqMsgMove(destmsg, srcmsg)
  End Function

  ' <summary>
  ' MsgCopy
  ' </summary>
  ' <param name="destmsg">ZmqMsgT</param>
  ' <param name="srcmsg">ZmqMsgT</param>
  ' <returns>Returns long.</returns>
  Function MsgCopy(ByRef destmsg As ZmqMsgT, ByRef srcmsg As ZmqMsgT) As Long
    Function = ZmqMsgCopy(destmsg, srcmsg)
  End Function

  ' <summary>
  ' MsgData
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <returns>Returns pointer.</returns>
  Function MsgData(ByRef msg As ZmqMsgT) As Long
    Function = ZmqMsgData(msg)
  End Function

  ' <summary>
  ' MsgSize
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <returns>Returns dword.</returns>
  Function MsgSize(ByRef msg As ZmqMsgT) As DWord
    Function = ZmqMsgSize(msg)
  End Function

  ' <summary>
  ' MsgMore
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <returns>Returns long.</returns>
  Function MsgMore(ByRef msg As ZmqMsgT) As Long
    Function = ZmqMsgMore(msg)
  End Function

  ' <summary>
  ' MsgGet
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="property_">Long</param>
  ' <returns>Returns long.</returns>
  Function MsgGet(ByRef msg As ZmqMsgT, ByVal property_ As Long) As Long
    Function = ZmqMsgGet(msg, property_)
  End Function

  ' <summary>
  ' MsgSet
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="property_">Long</param>
  ' <param name="optval">Long</param>
  ' <returns>Returns long.</returns>
  Function MsgSet(ByRef msg As ZmqMsgT, ByVal property_ As Long, ByVal optval As Long) As Long
    Function = ZmqMsgSet(msg, property_, optval)
  End Function

  ' <summary>
  ' MsgGets
  ' </summary>
  ' <param name="msg">ZmqMsgT</param>
  ' <param name="property_">String</param>
  ' <returns>Returns pointer to ASCIIZ.</returns>
  Function MsgGets(ByRef msg As ZmqMsgT, ByVal property_ As String) As Long
    Function = ZmqMsgGets(msg, property_)
  End Function
End Type

Type LibZmqPoll
  Public

  ' <summary>
  ' DoPoll
  ' </summary>
  ' <param name="items">Long</param>
  ' <param name="nitems">Long</param>
  ' <param name="timeout">Long</param>
  ' <returns>Returns long.</returns>
  Function DoPoll(ByVal items As Long, ByVal nitems As Long, ByVal timeout As Long) As Long
    Function = ZmqPoll(items, nitems, timeout)
  End Function
End Type

Type LibZmqProxy
  Public

  ' <summary>
  ' DoProxy
  ' </summary>
  ' <param name="frontend">Long</param>
  ' <param name="backend">Long</param>
  ' <param name="capture">Long</param>
  ' <returns>Returns long.</returns>
  Function DoProxy(ByVal frontend As Long, ByVal backend As Long, ByVal capture As Long) As Long
    Function = ZmqProxy(frontend, backend, capture)
  End Function

  ' <summary>
  ' Steerable
  ' </summary>
  ' <param name="frontend">Long</param>
  ' <param name="backend">Long</param>
  ' <param name="capture">Long</param>
  ' <param name="control">Long</param>
  ' <returns>Returns long.</returns>
  Function Steerable(ByVal frontend As Long, ByVal backend As Long, ByVal capture As Long, ByVal control As Long) As Long
    Function = ZmqProxySteerable(frontend, backend, capture, control)
  End Function
End Type

Type LibZmqSecurity
  Public

  ' <summary>
  ' Z85Encode
  ' </summary>
  ' <param name="pDest">Long</param>
  ' <param name="pData">Long</param>
  ' <param name="nLen">DWord</param>
  ' <returns>Returns pointer.</returns>
  Function Z85Encode(ByVal pDest As Long, ByVal pData As Long, ByVal nLen As DWord) As Long
    Function = ZmqZ85Encode(pDest, pData, nLen)
  End Function

  ' <summary>
  ' Z85Decode
  ' </summary>
  ' <param name="pDest">Long</param>
  ' <param name="lpszString">String</param>
  ' <returns>Returns pointer.</returns>
  Function Z85Decode(ByVal pDest As Long, ByVal lpszString As String) As Long
    Function = ZmqZ85Decode(pDest, lpszString)
  End Function

  ' <summary>
  ' Z85EncodeStr
  ' </summary>
  ' <param name="pData">Long</param>
  ' <param name="nLen">DWord</param>
  ' <returns>Returns string.</returns>
  Function Z85EncodeStr(ByVal pData As Long, ByVal nLen As DWord) As String
    Function = ZmqZ85EncodeStr(pData, nLen)
  End Function

  ' <summary>
  ' Z85DecodeStr
  ' </summary>
  ' <param name="lpszString">String</param>
  ' <param name="pDest">Long</param>
  ' <param name="nDestSize">DWord</param>
  ' <returns>Returns pointer.</returns>
  Function Z85DecodeStr(ByVal lpszString As String, ByVal pDest As Long, ByVal nDestSize As DWord) As Long
    Function = ZmqZ85DecodeStr(lpszString, pDest, nDestSize)
  End Function

  ' <summary>
  ' CurveKeypair
  ' </summary>
  ' <param name="z85Public">Long</param>
  ' <param name="z85Secret">Long</param>
  ' <returns>Returns long.</returns>
  Function CurveKeypair(ByVal z85Public As Long, ByVal z85Secret As Long) As Long
    Function = ZmqCurveKeypair(z85Public, z85Secret)
  End Function

  ' <summary>
  ' CurvePublic
  ' </summary>
  ' <param name="z85Public">Long</param>
  ' <param name="z85Secret">String</param>
  ' <returns>Returns long.</returns>
  Function CurvePublic(ByVal z85Public As Long, ByVal z85Secret As String) As Long
    Function = ZmqCurvePublic(z85Public, z85Secret)
  End Function

  ' <summary>
  ' CurvePublicStr
  ' </summary>
  ' <param name="z85Secret">String</param>
  ' <returns>Returns string.</returns>
  Function CurvePublicStr(ByVal z85Secret As String) As String
    Function = ZmqCurvePublicStr(z85Secret)
  End Function
End Type

Type LibZmqHelper
  Public

  ' <summary>
  ' StopwatchStart
  ' </summary>
  ' <returns>Returns watch handle.</returns>
  Function StopwatchStart() As Long
    Function = ZmqStopwatchStart()
  End Function

  ' <summary>
  ' StopwatchIntermediate
  ' </summary>
  ' <param name="watch">Long</param>
  ' <returns>Returns dword microseconds.</returns>
  Function StopwatchIntermediate(ByVal watch As Long) As DWord
    Function = ZmqStopwatchIntermediate(watch)
  End Function

  ' <summary>
  ' StopwatchStop
  ' </summary>
  ' <param name="watch">Long</param>
  ' <returns>Returns dword microseconds.</returns>
  Function StopwatchStop(ByVal watch As Long) As DWord
    Function = ZmqStopwatchStop(watch)
  End Function

  ' <summary>
  ' DoSleep
  ' </summary>
  ' <param name="seconds">Long</param>
  ' <returns>Returns void.</returns>
  Sub DoSleep(ByVal seconds As Long)
    ZmqSleep(seconds)
  End Sub

  ' <summary>
  ' Threadstart
  ' </summary>
  ' <param name="func">Long</param>
  ' <param name="arg">Long</param>
  ' <returns>Returns thread handle.</returns>
  Function Threadstart(ByVal func As Long, ByVal arg As Long) As Long
    Function = ZmqThreadstart(func, arg)
  End Function

  ' <summary>
  ' Threadclose
  ' </summary>
  ' <param name="thread">Long</param>
  ' <returns>Returns void.</returns>
  Sub Threadclose(ByVal thread As Long)
    ZmqThreadclose(thread)
  End Sub
End Type