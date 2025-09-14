param (
	[Parameter(Mandatory)]
	[ValidateSet("windows.x86_64", "linux.x86_64", "macintosh.arm_64", "android.arm_64", "iphone.arm_64")]
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
	xmake "config" "--builddir=.build" "--mode=release" "--plat=windows" "--arch=x64" "--toolchain=mingw" "--runtimes=c++_shared"
	xmake "build"
	llvm-strip "--strip-all" "${ModuleDirectory}/.build/windows/x64/release/kernel.dll"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/windows/x64/release/kernel.dll" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "linux.x86_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=linux" "--arch=x86_64" "--toolchain=clang-20" "--runtimes=c++_shared"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/linux/x86_64/release/libkernel.so" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "macintosh.arm_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=macosx" "--arch=arm64" "--toolchain=xcode" "--cc=clang-20" "--cxx=clang-20" "--xcode_sdkver=15.5" "--target_minver=11.5"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/macosx/arm64/release/libkernel.dylib" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "android.arm_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=android" "--arch=arm64-v8a" "--toolchain=ndk" "--runtimes=c++_shared" "--ndk_sdkver=30"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/android/arm64-v8a/release/libkernel.so" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "iphone.arm_64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=iphoneos" "--arch=arm64" "--toolchain=xcode" "--cc=clang-20" "--cxx=clang-20" "--xcode_sdkver=18.5" "--target_minver=15.6"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/iphoneos/arm64/release/libkernel.dylib" -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
