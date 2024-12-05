param (
	[Parameter(Mandatory)]
	[ValidateSet("windows.x86_64", "linux.x86_64", "macintosh.x86_64+arm_64", "android.arm_64", "iphone.arm_64")]
	[String] $TargetPlatform
)
. "${PSScriptRoot}/../common/powershell/helper.ps1"
$ModuleDirectory = "${ProjectDirectory}/Assistant"
$ModuleDistributionFile = "${ProjectDistributionDirectory}/${TargetPlatform}.assistant"
Push-Location -Path $ModuleDirectory
New-Item -Force -ItemType "Directory" -Path $ProjectDistributionDirectory
if ($TargetPlatform -eq "windows.x86_64") {
	$ModuleDistributionFile += ".msix"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "windows" "--release" "--no-tree-shake-icons"
	dart "run" "msix:create"
	My-SignMsix -Source "${ModuleDirectory}/build/windows/x64/runner/Release/assistant.msix" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "linux.x86_64") {
	$ModuleDistributionFile += ".zip"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "linux" "--release" "--no-tree-shake-icons" "--target-platform" "linux-x64"
	My-PackZip -Name "assistant" -Source "${ModuleDirectory}/build/linux/x64/release/bundle" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "macintosh.x86_64+arm_64") {
	$ModuleDistributionFile += ".dmg"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "macos" "--release" "--no-tree-shake-icons"
	My-PackDmg -Name "Twinning Assistant" -Source "${ModuleDirectory}/build/macos/Build/Products/Release/Runner.app" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "android.arm_64") {
	$ModuleDistributionFile += ".apk"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "apk" "--release" "--no-tree-shake-icons" "--target-platform" "android-arm64" "--split-per-abi"
	My-SignApk -Source "${ModuleDirectory}/build/app/outputs/flutter-apk/app-arm64-v8a-release.apk" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "iphone.arm_64") {
	$ModuleDistributionFile += ".ipa"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "ios" "--release" "--no-tree-shake-icons"
	My-PackIpa -Name "Twinning Assistant" -Source "${ModuleDirectory}/build/ios/iphoneos/Runner.app" -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
