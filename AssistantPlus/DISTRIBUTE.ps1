param (
	[Parameter(Mandatory)]
	[ValidateSet("windows.x86_64")]
	[String] $TargetPlatform
)
. "${PSScriptRoot}/../common/powershell/helper.ps1"
$ModuleDirectory = "${ProjectDirectory}/AssistantPlus"
$ModuleDistributionFile = "${ProjectDistributionDirectory}/${TargetPlatform}.assistant_plus"
Push-Location -Path $ModuleDirectory
New-Item -Force -ItemType "Directory" -Path $ProjectDistributionDirectory
if ($TargetPlatform -eq "windows.x86_64") {
	$ModuleDistributionFile += ".msix"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	MSBuild "-restore" "-verbosity:minimal" "-property:Platform=x64" "-property:Configuration=Release" "-property:GenerateAppxPackageOnBuild=true"
	My-SignMsix -Source (Get-Item -Path "${ModuleDirectory}/.build/bin/Application/x64.Release/AppPackages/Application_*.0.0.0_x64_Test/Application_*.0.0.0_x64.msix" | % { $_.FullName }) -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
