param (
	[Parameter(Mandatory)]
	[ValidateSet("windows.amd64", "linux.amd64", "macintosh.amd64+arm64", "android.arm64", "iphone.arm64")]
	[String] $TargetPlatform
)
. "${PSScriptRoot}/../common/powershell/helper.ps1"
$ModuleDirectory = "${ProjectDirectory}/Assistant"
$ModuleDistributionFile = "${ProjectDistributionDirectory}/${TargetPlatform}.assistant"
Push-Location -Path $ModuleDirectory
New-Item -Force -ItemType "Directory" -Path $ProjectDistributionDirectory
if ($TargetPlatform -eq "windows.amd64") {
	$ModuleDistributionFile += ".msix"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "windows" "--release" "--no-tree-shake-icons"
	My-PackMsix -Name "assistant" -Source "${ModuleDirectory}/build/windows/x64/runner/Release" -Destination "${ModuleDirectory}/build/windows/x64/runner/assistant.msix"
	My-SignMsix -Source "${ModuleDirectory}/build/windows/x64/runner/assistant.msix" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "linux.amd64") {
	$ModuleDistributionFile += ".zip"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	$Env:CXXFLAGS = "-stdlib=libc++"
	flutter "build" "linux" "--release" "--no-tree-shake-icons" "--target-platform" "linux-x64"
	My-PackZip -Name "assistant" -Source "${ModuleDirectory}/build/linux/x64/release/bundle" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "macintosh.amd64+arm64") {
	$ModuleDistributionFile += ".dmg"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "macos" "--release" "--no-tree-shake-icons"
	My-PackDmg -Name "Twinning Assistant" -Source "${ModuleDirectory}/build/macos/Build/Products/Release/Runner.app" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "android.arm64") {
	$ModuleDistributionFile += ".apk"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "apk" "--release" "--no-tree-shake-icons" "--target-platform" "android-arm64" "--split-per-abi"
	My-SignApk -Source "${ModuleDirectory}/build/app/outputs/flutter-apk/app-arm64-v8a-release.apk" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "iphone.arm64") {
	$ModuleDistributionFile += ".ipa"
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	flutter "build" "ios" "--release" "--no-tree-shake-icons"
	My-PackIpa -Name "Twinning Assistant" -Source "${ModuleDirectory}/build/ios/iphoneos/Runner.app" -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
