'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Declare Function

' <summary>
' ZmqZ85Encode
' </summary>
' <param name="dest">Long</param>
' <param name="data">Long</param>
' <param name="size">DWord</param>
' <returns>Returns pointer.</returns>
Declare Function ZmqZ85Encode Lib "libzmq.dll" Alias "zmq_z85_encode" (ByVal dest As Long, ByVal data As Long, ByVal size As DWord) As Long

' <summary>
' ZmqZ85Decode
' </summary>
' <param name="dest">Long</param>
' <param name="string">Asciiz</param>
' <returns>Returns pointer.</returns>
Declare Function ZmqZ85Decode Lib "libzmq.dll" Alias "zmq_z85_decode" (ByVal dest As Long, ByVal lpszString As Asciiz) As Long

' <summary>
' ZmqCurveKeypair
' </summary>
' <param name="z85Public">Long</param>
' <param name="z85Secret">Long</param>
' <returns>Returns long.</returns>
Declare Function ZmqCurveKeypair Lib "libzmq.dll" Alias "zmq_curve_keypair" (ByVal z85Public As Long, ByVal z85Secret As Long) As Long

' <summary>
' ZmqCurvePublic
' </summary>
' <param name="z85Public">Long</param>
' <param name="z85Secret">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqCurvePublic Lib "libzmq.dll" Alias "zmq_curve_public" (ByVal z85Public As Long, ByVal z85Secret As Asciiz) As Long

' <summary>
' ZmqZ85EncodeStr
' </summary>
' <param name="data">Long</param>
' <param name="size">DWord</param>
' <returns>Returns string.</returns>
Function ZmqZ85EncodeStr(ByVal pData As Long, ByVal nLen As DWord) As String
  Local nDestSize As DWord
  Local hDest As Long
  Local sOut As String

  If nLen <= 0 Then
    Function = ""
    Exit Function
  End If
  
  If (nLen And 3) <> 0 Then
    Function = ""
    Exit Function
  End If

  nDestSize = ((nLen * 5) \ 4) + 1
  hDest = HEAP_Alloc(nDestSize)

  If hDest Then
    If ZmqZ85Encode(hDest, pData, nLen) Then
      sOut = Peek$(ASCIIZ, hDest)
    End If

    HEAP_Free(hDest)
  End If

  Function = sOut
End Function

' <summary>
' ZmqZ85DecodeStr
' </summary>
' <param name="lpszString">String</param>
' <param name="pDest">Long</param>
' <param name="nDestSize">DWord</param>
' <returns>Returns pointer.</returns>
Function ZmqZ85DecodeStr(ByVal lpszString As String, ByVal pDest As Long, ByVal nDestSize As DWord) As Long
  Local nNeedSize As DWord

  nNeedSize = (Len(lpszString) * 4) \ 5

  If nNeedSize <= 0 Or nDestSize < nNeedSize Or pDest = 0 Then
    Function = 0
    Exit Function
  End If

  Function = ZmqZ85Decode(pDest, lpszString)
End Function

' <summary>
' ZmqCurvePublicStr
' </summary>
' <param name="z85Secret">String</param>
' <returns>Returns string.</returns>
Function ZmqCurvePublicStr(ByVal z85Secret As String) As String
  Local sOut As String
  Local hPublic As Long

  hPublic = HEAP_Alloc($ZMQ_CURVE_KEYSIZE_Z85 + 1)

  If hPublic Then
    If ZmqCurvePublic(hPublic, z85Secret) = 0 Then
      sOut = Peek$(ASCIIZ, hPublic)
    End If

    HEAP_Free(hPublic)
  End If

  Function = sOut
End Function