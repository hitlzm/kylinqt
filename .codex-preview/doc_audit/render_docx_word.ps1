param(
    [Parameter(Mandatory = $true)]
    [string]$InputDir,
    [Parameter(Mandatory = $true)]
    [string]$OutputDir,
    [string]$FileName = ""
)

$ErrorActionPreference = "Stop"
$resolvedInput = (Resolve-Path -LiteralPath $InputDir).Path
$fullOutput = [System.IO.Path]::GetFullPath($OutputDir)
New-Item -ItemType Directory -Force -Path $fullOutput | Out-Null

$word = $null
try {
    $word = New-Object -ComObject Word.Application
    $word.Visible = $false
    $word.DisplayAlerts = 0
    $word.Options.UpdateLinksAtOpen = $false

    $filter = if ([string]::IsNullOrWhiteSpace($FileName)) { "*.docx" } else { $FileName }
    Get-ChildItem -LiteralPath $resolvedInput -Filter $filter -File |
        Sort-Object Name |
        ForEach-Object {
            $pdfPath = Join-Path $fullOutput ($_.BaseName + ".pdf")
            $document = $null
            try {
                $document = $word.Documents.Open(
                    $_.FullName,
                    $false,
                    $true,
                    $false
                )
                $document.ExportAsFixedFormat($pdfPath, 17)
                Write-Output ("rendered: " + $_.Name)
            }
            finally {
                if ($null -ne $document) {
                    $document.Close($false)
                    [System.Runtime.InteropServices.Marshal]::ReleaseComObject($document) | Out-Null
                }
            }
        }
}
finally {
    if ($null -ne $word) {
        $word.Quit()
        [System.Runtime.InteropServices.Marshal]::ReleaseComObject($word) | Out-Null
    }
    [GC]::Collect()
    [GC]::WaitForPendingFinalizers()
}
