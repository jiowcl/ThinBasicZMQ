'--------------------------------------------------------------------------------------------
'  Copyright (c) Ji-Feng Tsai. All rights reserved.
'  Code released under the MIT license.
'--------------------------------------------------------------------------------------------

' Package DLLs live under Library\x86 (x86 only; ThinBasic cannot load x64 libzmq).
' ThinBasic #INCLUDE is script-relative, but Declare Lib uses the process CWD.
' Include this file before other Core modules that Declare Lib "libzmq.dll".

' <summary>
' ZmqSetCurrentDirectory
' </summary>
' <param name="lpPathName">Asciiz</param>
' <returns>Returns long.</returns>
Declare Function ZmqSetCurrentDirectory Lib "kernel32.dll" Alias "SetCurrentDirectoryA" (ByVal lpPathName As Asciiz) As Long

' <summary>
' ZmqLibraryPathSet
' </summary>
' <returns>Returns void.</returns>
Sub ZmqLibraryPathSet()
  Local sPath As String

  ' Scripts in ThinBasicZMQ\
  sPath = APP_SourcePath & "Library\x86"
  If ZmqSetCurrentDirectory(sPath) = 0 Then
    ' Scripts in ThinBasicZMQ\Module\
    sPath = APP_SourcePath & "..\Library\x86"
    ZmqSetCurrentDirectory(sPath)
  End If
End Sub

' Switch CWD so subsequent Declare Lib "libzmq.dll" resolves package DLLs.
ZmqLibraryPathSet()
