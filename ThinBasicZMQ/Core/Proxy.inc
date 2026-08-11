'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqProxy
' </summary>
' <param name="frontend">Long</param>
' <param name="backend">Long</param>
' <param name="capture">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqProxy Lib "libzmq.dll" Alias "zmq_proxy" (ByVal frontend As Long, ByVal backend As Long, ByVal capture As Long) As Long

' <summary>
' ZmqProxySteerable
' </summary>
' <param name="frontend">Long</param>
' <param name="backend">Long</param>
' <param name="capture">Long</param>
' <param name="control">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqProxySteerable Lib "libzmq.dll" Alias "zmq_proxy_steerable" (ByVal frontend As Long, ByVal backend As Long, ByVal capture As Long, ByVal control As Long) As Long