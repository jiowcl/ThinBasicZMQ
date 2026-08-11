'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqPoll
' </summary>
' <param name="items">Long</param>
' <param name="nitems">Long</param>
' <param name="timeout">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqPoll Lib "libzmq.dll" Alias "zmq_poll" (ByVal items As Long, ByVal nitems As Long, ByVal timeout As Long) As Long