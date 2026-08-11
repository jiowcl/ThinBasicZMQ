'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqStopwatchStart
' </summary>
' <returns>Returns watch handle.</returns>
Declare Function ZmqStopwatchStart Lib "libzmq.dll" Alias "zmq_stopwatch_start" () As Long

' <summary>
' ZmqStopwatchIntermediate
' </summary>
' <param name="watch">Long</param>
' <returns>Returns dword microseconds.</returns>
Declare Function ZmqStopwatchIntermediate Lib "libzmq.dll" Alias "zmq_stopwatch_intermediate" (ByVal watch As Long) As DWord

' <summary>
' ZmqStopwatchStop
' </summary>
' <param name="watch">Long</param>
' <returns>Returns dword microseconds.</returns>
Declare Function ZmqStopwatchStop Lib "libzmq.dll" Alias "zmq_stopwatch_stop" (ByVal watch As Long) As DWord

' <summary>
' ZmqSleep
' </summary>
' <param name="seconds">Long</param>
' <returns>Returns void.</returns>
Declare Sub ZmqSleep Lib "libzmq.dll" Alias "zmq_sleep" (ByVal seconds As Long)

' <summary>
' ZmqThreadstart
' </summary>
' <param name="func">Long</param>
' <param name="arg">Long</param>
' <returns>Returns thread handle.</returns>
Declare Function ZmqThreadstart Lib "libzmq.dll" Alias "zmq_threadstart" (ByVal func As Long, ByVal arg As Long) As Long

' <summary>
' ZmqThreadclose
' </summary>
' <param name="thread">Long</param>
' <returns>Returns void.</returns>
Declare Sub ZmqThreadclose Lib "libzmq.dll" Alias "zmq_threadclose" (ByVal thread As Long)

' <summary>
' ZmqMemCopy
' </summary>
' <param name="dest">Long</param>
' <param name="src">Long</param>
' <param name="length">Long</param>
' <returns>Returns void.</returns>
Declare Sub ZmqMemCopy Lib "kernel32.dll" Alias "RtlMoveMemory" (ByVal dest As Long, ByVal src As Long, ByVal length As Long)
