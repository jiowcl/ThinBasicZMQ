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
  ZmqSetCurrentDirectory(APP_SourcePath & "Library\x86")
End Sub

' Switch CWD so subsequent Declare Lib "libzmq.dll" resolves package DLLs.
ZmqLibraryPathSet()
