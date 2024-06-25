param (
	[Parameter(Mandatory)]
	[ValidateSet("any")]
	[String] $TargetPlatform
)
. "${PSScriptRoot}/../common/powershell/helper.ps1"
$ModuleDirectory = "${ProjectDirectory}/Script"
$ModuleDistributionFile = "${ProjectDistributionDirectory}/${TargetPlatform}.script"
Push-Location -Path $ModuleDirectory
New-Item -Force -ItemType "Directory" -Path $ProjectDistributionDirectory
if ($TargetPlatform -eq "any") {
	$ModuleDistributionFile += ".zip"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	tsc
	Get-ChildItem -Recurse -Filter "*.json" -Path "${ModuleDirectory}" | % {
		$_ = $_.FullName.Remove(0, $ModuleDirectory.Length + 1).Replace('\', '/')
		if (-not $_.Equals("tsconfig.json") -and -not $_.StartsWith(".build/")) {
			New-Item -Force -ItemType "SymbolicLink" -Path "${ModuleDirectory}/.build/$_" -Target "${ModuleDirectory}/$_"
		}
	}
	My-PackZip -Name "script" -Source "${ModuleDirectory}/.build" -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
