# launch script for pwsh

$twinning = Split-Path -Parent -Path $MyInvocation.MyCommand.Path
$twinning = $twinning -replace "\\", "/"

if (($PSVersionTable.Platform -eq 'Win32NT') -or ($PSVersionTable.Platform -eq $null)) {
	$env:PATH = "$twinning/library;$env:PATH"
	$env:PATHEXT = ";$env:PATHEXT"
}
else {
	$env:LD_LIBRARY_PATH = "$twinning/library:$env:LD_LIBRARY_PATH"
}

& "$twinning/shell" "$twinning/kernel" "$twinning/script/main.js" "$twinning" $args

Write-Host "press <ENTER> to exit ..."
Read-Host
