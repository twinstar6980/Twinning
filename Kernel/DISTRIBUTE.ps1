param (
	[Parameter(Mandatory)]
	[ValidateSet("windows.x86_64", "linux.x86_64", "macintosh.x86_64", "android.arm_64", "iphone.arm_64")]
	[String] $TargetPlatform
)
. "${PSScriptRoot}/../common/powershell/helper.ps1"
$ModuleDirectory = "${ProjectDirectory}/Kernel"
$ModuleDistributionFile = "${ProjectDistributionDirectory}/${TargetPlatform}.kernel"
Push-Location -Path $ModuleDirectory
New-Item -Force -ItemType "Directory" -Path $ProjectDistributionDirectory
if ($TargetPlatform -eq "windows.x86_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--buildir=.build" "--mode=release" "--plat=windows" "--arch=x64" "--toolchain=clang-cl" "--runtimes=MD"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/windows/x64/release/kernel.dll" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "linux.x86_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--buildir=.build" "--mode=release" "--plat=linux" "--arch=x86_64" "--toolchain=clang-20" "--runtimes=c++_shared"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/linux/x86_64/release/libkernel.so" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "macintosh.x86_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--buildir=.build" "--mode=release" "--plat=macosx" "--arch=x86_64" "--toolchain=xcode" "--cc=clang-20" "--cxx=clang-20" "--xcode_sdkver=13.3" "--target_minver=13.3"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/macosx/x86_64/release/libkernel.dylib" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "android.arm_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--buildir=.build" "--mode=release" "--plat=android" "--arch=arm64-v8a" "--toolchain=ndk" "--runtimes=c++_shared" "--ndk_sdkver=28"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/android/arm64-v8a/release/libkernel.so" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "iphone.arm_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--buildir=.build" "--mode=release" "--plat=iphoneos" "--arch=arm64" "--toolchain=xcode" "--cc=clang-20" "--cxx=clang-20" "--xcode_sdkver=16.4" "--target_minver=16.4"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/iphoneos/arm64/release/libkernel.dylib" -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
