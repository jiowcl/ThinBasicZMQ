'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqMsgInit
' </summary>
' <param name="msg">ZmqMsgT</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgInit Lib "libzmq.dll" Alias "zmq_msg_init" (ByRef msg As ZmqMsgT) As Long

' <summary>
' ZmqMsgInitSize
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="msgsize">DWord</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgInitSize Lib "libzmq.dll" Alias "zmq_msg_init_size" (ByRef msg As ZmqMsgT, ByVal msgsize As DWord) As Long

' <summary>
' ZmqMsgInitData
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="msgdata">Long</param>
' <param name="msgsize">DWord</param>
' <param name="ffn">Long</param>
' <param name="hint">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgInitData Lib "libzmq.dll" Alias "zmq_msg_init_data" (ByRef msg As ZmqMsgT, ByVal msgdata As Long, ByVal msgsize As DWord, ByVal ffn As Long, ByVal hint As Long) As Long

' <summary>
' ZmqMsgSend
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="hSocket">Long</param>
' <param name="flags">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgSend Lib "libzmq.dll" Alias "zmq_msg_send" (ByRef msg As ZmqMsgT, ByVal hSocket As Long, ByVal flags As Long) As Long

' <summary>
' ZmqMsgRecv
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="hSocket">Long</param>
' <param name="flags">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgRecv Lib "libzmq.dll" Alias "zmq_msg_recv" (ByRef msg As ZmqMsgT, ByVal hSocket As Long, ByVal flags As Long) As Long

' <summary>
' ZmqMsgClose
' </summary>
' <param name="msg">ZmqMsgT</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgClose Lib "libzmq.dll" Alias "zmq_msg_close" (ByRef msg As ZmqMsgT) As Long

' <summary>
' ZmqMsgMove
' </summary>
' <param name="destmsg">ZmqMsgT</param>
' <param name="srcmsg">ZmqMsgT</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgMove Lib "libzmq.dll" Alias "zmq_msg_move" (ByRef destmsg As ZmqMsgT, ByRef srcmsg As ZmqMsgT) As Long

' <summary>
' ZmqMsgCopy
' </summary>
' <param name="destmsg">ZmqMsgT</param>
' <param name="srcmsg">ZmqMsgT</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgCopy Lib "libzmq.dll" Alias "zmq_msg_copy" (ByRef destmsg As ZmqMsgT, ByRef srcmsg As ZmqMsgT) As Long

' <summary>
' ZmqMsgData
' </summary>
' <param name="msg">ZmqMsgT</param>
' <returns>Returns pointer.</returns>
Declare Function ZmqMsgData Lib "libzmq.dll" Alias "zmq_msg_data" (ByRef msg As ZmqMsgT) As Long

' <summary>
' ZmqMsgSize
' </summary>
' <param name="msg">ZmqMsgT</param>
' <returns>Returns dword.</returns>
Declare Function ZmqMsgSize Lib "libzmq.dll" Alias "zmq_msg_size" (ByRef msg As ZmqMsgT) As DWord

' <summary>
' ZmqMsgMore
' </summary>
' <param name="msg">ZmqMsgT</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgMore Lib "libzmq.dll" Alias "zmq_msg_more" (ByRef msg As ZmqMsgT) As Long

' <summary>
' ZmqMsgGet
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="property">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgGet Lib "libzmq.dll" Alias "zmq_msg_get" (ByRef msg As ZmqMsgT, ByVal property_ As Long) As Long

' <summary>
' ZmqMsgSet
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="property">Long</param>
' <param name="optval">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqMsgSet Lib "libzmq.dll" Alias "zmq_msg_set" (ByRef msg As ZmqMsgT, ByVal property_ As Long, ByVal optval As Long) As Long

' <summary>
' ZmqMsgGets
' </summary>
' <param name="msg">ZmqMsgT</param>
' <param name="property">Asciiz</param>
' <returns>Returns pointer to ASCIIZ.</returns>
Declare Function ZmqMsgGets Lib "libzmq.dll" Alias "zmq_msg_gets" (ByRef msg As ZmqMsgT, ByVal property_ As Asciiz) As Long