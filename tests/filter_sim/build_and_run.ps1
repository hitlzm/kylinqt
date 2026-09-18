# 编译并运行滤波器实测程序。
# 要点：用 -include 抢先定义 serialport.h 的包含保护，使被测滤波器源码不必依赖 Qt 即可单独编译；
#       被测代码是工程里的原始文件，未做任何改动。
$ErrorActionPreference = 'Stop'

$root = Resolve-Path (Join-Path $PSScriptRoot '..\..')
Set-Location $root

New-Item -ItemType Directory -Force -Path (Join-Path $PSScriptRoot 'out') | Out-Null

$gxx = (Get-Command g++).Source
$gxxArgs = @(
    '-std=c++14', '-O2', '-Wall',
    '-I', (Join-Path $PSScriptRoot 'stub'),
    '-I', $root.Path,
    '-include', (Join-Path $PSScriptRoot 'stub\preserialport.h'),
    (Join-Path $root 'kalman\AlphaBetaTracker.cpp'),
    (Join-Path $root 'kalman\seekKalman.cpp'),
    (Join-Path $PSScriptRoot 'main.cpp'),
    '-o', (Join-Path $PSScriptRoot 'filter_sim.exe')
)

& $gxx @gxxArgs
if ($LASTEXITCODE -ne 0) { throw "编译失败" }

Set-Location $PSScriptRoot
& (Join-Path $PSScriptRoot 'filter_sim.exe')
