'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqCtxNew
' </summary>
' <returns>Returns context handle.</returns>
Declare Function ZmqCtxNew Lib "libzmq.dll" Alias "zmq_ctx_new" () As Long

' <summary>
' ZmqCtxTerm
' </summary>
' <param name="context">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqCtxTerm Lib "libzmq.dll" Alias "zmq_ctx_term" (ByVal context As Long) As Long

' <summary>
' ZmqCtxShutdown
' </summary>
' <param name="context">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqCtxShutdown Lib "libzmq.dll" Alias "zmq_ctx_shutdown" (ByVal context As Long) As Long

' <summary>
' ZmqCtxSet
' </summary>
' <param name="context">Long</param>
' <param name="options">Long</param>
' <param name="optval">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqCtxSet Lib "libzmq.dll" Alias "zmq_ctx_set" (ByVal context As Long, ByVal options As Long, ByVal optval As Long) As Long

' <summary>
' ZmqCtxGet
' </summary>
' <param name="context">Long</param>
' <param name="options">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqCtxGet Lib "libzmq.dll" Alias "zmq_ctx_get" (ByVal context As Long, ByVal options As Long) As Long