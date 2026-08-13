# Verify thinBasic_ZeroMQ.dll is PE i386 and exports ThinBasic entry points.
param(
    [Parameter(Mandatory = $true)]
    [string] $DllPath,
    [string] $Objdump = ""
)

$ErrorActionPreference = "Stop"
$DllPath = [IO.Path]::GetFullPath($DllPath)

if (-not (Test-Path -LiteralPath $DllPath)) {
    throw "DLL not found: $DllPath"
}

$fs = [IO.File]::OpenRead($DllPath)
try {
    $br = New-Object IO.BinaryReader $fs
    $fs.Position = 0x3C
    $pe = $br.ReadInt32()
    $fs.Position = $pe
    $sig = [Text.Encoding]::ASCII.GetString($br.ReadBytes(4))
    if ($sig -ne "PE`0`0") {
        throw "Not a PE file (signature='$sig')"
    }

    $machine = $br.ReadUInt16()
    if ($machine -ne 0x14C) {
        throw "PE machine is 0x$($machine.ToString('X4')); expected i386 (0x14C)"
    }

    Write-Host "PE machine: i386 (0x14C)"
}
finally {
    $fs.Dispose()
}

$dumpExe = $null
if ($Objdump -and (Test-Path -LiteralPath $Objdump)) {
    $dumpExe = $Objdump
}
elseif (Get-Command objdump.exe -ErrorAction SilentlyContinue) {
    $dumpExe = (Get-Command objdump.exe).Source
}

if (-not $dumpExe) {
    Write-Host "objdump not found; skipped export name check"
    exit 0
}

$dump = & $dumpExe -p $DllPath 2>&1 | Out-String
$required = @("_LoadLocalSymbols", "_UnLoadLocalSymbols")
foreach ($name in $required) {
    if ($dump -notmatch [regex]::Escape($name)) {
        throw "Missing export: $name"
    }
    Write-Host "Export OK: $name"
}

Write-Host "DLL checks passed: $DllPath"
