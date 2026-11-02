[CmdletBinding()]
param([int]$JavaProcessId,
      [int]$ReaServerProcessId,
      [Parameter(Mandatory)][int]$ExportProcessId,
      [long]$MaximumBytes = 1073741824,
      [int]$MaximumStallSeconds = 90)
$ErrorActionPreference = 'Stop'
$log = Join-Path $PSScriptRoot '../build/decompiler-watchdog.log'
while (Get-Process -Id $ExportProcessId -ErrorAction SilentlyContinue) {
    $allProcesses = @(Get-CimInstance Win32_Process)
    $byId = @{}
    foreach ($item in $allProcesses) { $byId[[int]$item.ProcessId] = $item }
    $children = $allProcesses | Where-Object { $_.Name -eq 'decompile.exe' } | Where-Object {
        if ($JavaProcessId) { $_.ParentProcessId -eq $JavaProcessId }
        elseif ($ReaServerProcessId) {
            $ancestor = $_
            $owned = $false
            for ($depth=0; $depth -lt 12 -and $ancestor; $depth++) {
                if ($ancestor.ProcessId -eq $ReaServerProcessId) { $owned=$true; break }
                $ancestor = $byId[[int]$ancestor.ParentProcessId]
            }
            $owned
        }
    }
    foreach ($child in $children) {
        $process = Get-Process -Id $child.ProcessId -ErrorAction SilentlyContinue
        $lastProgress = Get-ChildItem (Join-Path $PSScriptRoot '../raw/rea') -File |
            Where-Object { $_.Name -eq 'procedures.json' -or $_.Name -like 'batch-*.json' } |
            Sort-Object LastWriteTime -Descending | Select-Object -First 1
        $stalled = $false
        if ($process -and $lastProgress) {
            $since = $lastProgress.LastWriteTime
            if ($process.StartTime -gt $since) { $since = $process.StartTime }
            $stalled = ((Get-Date)-$since).TotalSeconds -gt $MaximumStallSeconds
        }
        if ($process -and ($process.WorkingSet64 -ge $MaximumBytes -or $stalled)) {
            [ordered]@{ time = (Get-Date).ToUniversalTime().ToString('o');
                process = $child.ProcessId; parent = $child.ParentProcessId;
                workingSetBytes = $process.WorkingSet64;
                stalled = $stalled;
                action = 'Stop runaway decompiler; retain explicit REA error' } |
                ConvertTo-Json -Compress | Add-Content -LiteralPath $log
            Stop-Process -Id $child.ProcessId -ErrorAction SilentlyContinue
        }
    }
    Start-Sleep -Seconds 2
}
