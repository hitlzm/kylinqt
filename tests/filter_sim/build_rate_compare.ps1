# Build the focused ex-guide frequency/trajectory comparison.
# The production filter sources are compiled directly; no production file is changed.
$ErrorActionPreference = 'Stop'

$root = Resolve-Path (Join-Path $PSScriptRoot '..\..')
$ocv = 'E:/opencv-4.2.0/DNN/install'
if (-not (Test-Path $ocv)) {
    throw "OpenCV not found: $ocv"
}

$env:PATH = "$ocv/x64/mingw/bin;$env:PATH"
Set-Location $PSScriptRoot
New-Item -ItemType Directory -Force -Path .\out | Out-Null

$gxx = (Get-Command g++).Source
$args = @(
    '-std=c++14', '-O2', '-Wall',
    "-I$ocv/include",
    '-I', (Join-Path $PSScriptRoot 'stub'),
    '-I', $root.Path,
    '-include', (Join-Path $PSScriptRoot 'stub\preserialport.h'),
    (Join-Path $root 'kalman\AlphaBetaTracker.cpp'),
    (Join-Path $root 'opencv\pixelkalmantracker.cpp'),
    (Join-Path $PSScriptRoot 'exguide_rate_compare.cpp'),
    "-L$ocv/x64/mingw/lib",
    '-lopencv_core420', '-lopencv_video420',
    '-o', (Join-Path $PSScriptRoot 'exguide_rate_compare.exe')
)

& $gxx @args
if ($LASTEXITCODE -ne 0) {
    throw "build failed"
}

& (Join-Path $PSScriptRoot 'exguide_rate_compare.exe')
