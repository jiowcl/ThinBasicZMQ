'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqSocket
' </summary>
' <param name="context">Long</param>
' <param name="stype">Long</param>
' <returns>Returns socket handle.</returns>
Declare Function ZmqSocket Lib "libzmq.dll" Alias "zmq_socket" (ByVal context As Long, ByVal stype As Long) As Long

' <summary>
' ZmqSocketMonitor
' </summary>
' <param name="hSocket">Long</param>
' <param name="addr">Asciiz</param>
' <param name="events">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqSocketMonitor Lib "libzmq.dll" Alias "zmq_socket_monitor" (ByVal hSocket As Long, ByVal addr As Asciiz, ByVal events As Long) As Long

' <summary>
' ZmqBind
' </summary>
' <param name="hSocket">Long</param>
' <param name="addr">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqBind Lib "libzmq.dll" Alias "zmq_bind" (ByVal hSocket As Long, ByVal addr As Asciiz) As Long

' <summary>
' ZmqUnBind
' </summary>
' <param name="hSocket">Long</param>
' <param name="addr">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqUnBind Lib "libzmq.dll" Alias "zmq_unbind" (ByVal hSocket As Long, ByVal addr As Asciiz) As Long

' <summary>
' ZmqRecv
' </summary>
' <param name="hSocket">Long</param>
' <param name="buf">Long</param>
' <param name="buflen">DWord</param>
' <param name="flags">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqRecv Lib "libzmq.dll" Alias "zmq_recv" (ByVal hSocket As Long, ByVal buf As Long, ByVal buflen As DWord, ByVal flags As Long) As Long

' <summary>
' ZmqSend
' </summary>
' <param name="hSocket">Long</param>
' <param name="buf">Long</param>
' <param name="buflen">DWord</param>
' <param name="flags">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqSend Lib "libzmq.dll" Alias "zmq_send" (ByVal hSocket As Long, ByVal buf As Long, ByVal buflen As DWord, ByVal flags As Long) As Long

' <summary>
' ZmqSendConst
' </summary>
' <param name="hSocket">Long</param>
' <param name="buf">Long</param>
' <param name="buflen">DWord</param>
' <param name="flags">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqSendConst Lib "libzmq.dll" Alias "zmq_send_const" (ByVal hSocket As Long, ByVal buf As Long, ByVal buflen As DWord, ByVal flags As Long) As Long

' <summary>
' ZmqConnect
' </summary>
' <param name="hSocket">Long</param>
' <param name="addr">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqConnect Lib "libzmq.dll" Alias "zmq_connect" (ByVal hSocket As Long, ByVal addr As Asciiz) As Long

' <summary>
' ZmqDisConnect
' </summary>
' <param name="hSocket">Long</param>
' <param name="addr">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqDisConnect Lib "libzmq.dll" Alias "zmq_disconnect" (ByVal hSocket As Long, ByVal addr As Asciiz) As Long

' <summary>
' ZmqSetsockopt
' </summary>
' <param name="hSocket">Long</param>
' <param name="options">Long</param>
' <param name="optval">Long</param>
' <param name="optvallen">DWord</param>
' <returns>Returns long.</returns>
Declare Function ZmqSetsockopt Lib "libzmq.dll" Alias "zmq_setsockopt" (ByVal hSocket As Long, ByVal options As Long, ByVal optval As Long, ByVal optvallen As DWord) As Long

' <summary>
' ZmqGetsockopt
' </summary>
' <param name="hSocket">Long</param>
' <param name="options">Long</param>
' <param name="optval">Long</param>
' <param name="optvallen">DWord</param>
' <returns>Returns long.</returns>
Declare Function ZmqGetsockopt Lib "libzmq.dll" Alias "zmq_getsockopt" (ByVal hSocket As Long, ByVal options As Long, ByVal optval As Long, ByRef optvallen As DWord) As Long

' <summary>
' ZmqClose
' </summary>
' <param name="hSocket">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqClose Lib "libzmq.dll" Alias "zmq_close" (ByVal hSocket As Long) As Long

' <summary>
' ZmqSetsockoptInt
' </summary>
' <param name="hSocket">Long</param>
' <param name="options">Long</param>
' <param name="optval">Long</param>
' <returns>Returns long.</returns>
Function ZmqSetsockoptInt(ByVal hSocket As Long, ByVal options As Long, ByVal optval As Long) As Long
  Local lOptVal As Long

  lOptVal = optval
  Function = ZmqSetsockopt(hSocket, options, VarPtr(lOptVal), SizeOf(Long))
End Function

' <summary>
' ZmqSetsockoptStr
' </summary>
' <param name="hSocket">Long</param>
' <param name="options">Long</param>
' <param name="optval">String</param>
' <returns>Returns long.</returns>
Function ZmqSetsockoptStr(ByVal hSocket As Long, ByVal options As Long, ByVal optval As String) As Long
  Function = ZmqSetsockopt(hSocket, options, StrPtr(optval), Len(optval))
End Function

' <summary>
' ZmqGetsockoptInt
' </summary>
' <param name="hSocket">Long</param>
' <param name="options">Long</param>
' <param name="optval">Long</param>
' <returns>Returns long.</returns>
Function ZmqGetsockoptInt(ByVal hSocket As Long, ByVal options As Long, ByRef optval As Long) As Long
  Local lOptVal As Long
  Local optvallen As DWord
  Local lResult As Long

  lOptVal = 0
  optvallen = SizeOf(Long)
  lResult = ZmqGetsockopt(hSocket, options, VarPtr(lOptVal), optvallen)

  If lResult = 0 Then
    optval = lOptVal
  End If

  Function = lResult
End Function