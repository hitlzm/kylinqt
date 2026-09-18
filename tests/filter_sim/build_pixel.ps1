# 编译并运行 PixelKalmanTracker 风险实测（链接工程真实实现 + OpenCV 4.2 MinGW）
$ErrorActionPreference = 'Stop'

$root = Resolve-Path (Join-Path $PSScriptRoot '..\..')
$ocv  = 'E:/opencv-4.2.0/DNN/install'

if (-not (Test-Path $ocv)) { throw "未找到 OpenCV: $ocv" }

$env:PATH = "$ocv/x64/mingw/bin;$env:PATH"

$gxx = (Get-Command g++).Source
$gxxArgs = @(
    '-std=c++14', '-O2', '-Wall',
    "-I$ocv/include",
    "-I$root",
    (Join-Path $root 'opencv\pixelkalmantracker.cpp'),
    (Join-Path $PSScriptRoot 'pixel_risk.cpp'),
    "-L$ocv/x64/mingw/lib",
    '-lopencv_core420', '-lopencv_video420',
    '-o', (Join-Path $PSScriptRoot 'pixel_risk.exe')
)

& $gxx @gxxArgs
if ($LASTEXITCODE -ne 0) { throw "编译失败" }

Set-Location $PSScriptRoot
& (Join-Path $PSScriptRoot 'pixel_risk.exe')
