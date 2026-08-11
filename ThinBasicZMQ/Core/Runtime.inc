'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqErrno
' </summary>
' <returns>Returns long.</returns>
Declare Function ZmqErrno Lib "libzmq.dll" Alias "zmq_errno" () As Long

' <summary>
' ZmqStrerror
' </summary>
' <param name="errnum">Long</param>
' <returns>Returns pointer to ASCIIZ error string.</returns>
Declare Function ZmqStrerror Lib "libzmq.dll" Alias "zmq_strerror" (ByVal errnum As Long) As Long

' <summary>
' ZmqVersion
' </summary>
' <param name="major">Long</param>
' <param name="minor">Long</param>
' <param name="patch">Long</param>
' <returns>Returns void.</returns>
Declare Sub ZmqVersion Lib "libzmq.dll" Alias "zmq_version" (ByRef major As Long, ByRef minor As Long, ByRef patch As Long)

' <summary>
' ZmqHas
' </summary>
' <param name="capability">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqHas Lib "libzmq.dll" Alias "zmq_has" (ByVal capability As Asciiz) As Long

' <summary>
' ZmqStrerrorString
' </summary>
' <param name="errnum">Long</param>
' <returns>Returns string.</returns>
Function ZmqStrerrorString(ByVal errnum As Long) As String
  Local pErr As Long

  pErr = ZmqStrerror(errnum)
  If pErr Then
    Function = Peek$(ASCIIZ, pErr)
  Else
    Function = ""
  End If
End Function