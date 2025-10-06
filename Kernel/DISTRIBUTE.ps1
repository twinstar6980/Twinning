param (
	[Parameter(Mandatory)]
	[ValidateSet("windows.amd64", "linux.amd64", "macintosh.arm64", "android.arm64", "iphone.arm64")]
	[String] $TargetPlatform
)
. "${PSScriptRoot}/../common/powershell/helper.ps1"
$ModuleDirectory = "${ProjectDirectory}/Kernel"
$ModuleDistributionFile = "${ProjectDistributionDirectory}/${TargetPlatform}.kernel"
Push-Location -Path $ModuleDirectory
New-Item -Force -ItemType "Directory" -Path $ProjectDistributionDirectory
if ($TargetPlatform -eq "windows.amd64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=windows" "--arch=x64" "--toolchain=mingw" "--runtimes=c++_shared"
	xmake "build"
	llvm-strip "--strip-all" "${ModuleDirectory}/.build/windows/x64/release/kernel.dll"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/windows/x64/release/kernel.dll" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "linux.amd64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=linux" "--arch=x86_64" "--toolchain=clang-20" "--runtimes=c++_shared"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/linux/x86_64/release/libkernel.so" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "macintosh.arm64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=macosx" "--arch=arm64" "--toolchain=xcode" "--cc=clang-20" "--cxx=clang-20" "--target_minver=11.5"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/macosx/arm64/release/libkernel.dylib" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "android.arm64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=android" "--arch=arm64-v8a" "--toolchain=ndk" "--runtimes=c++_shared" "--ndk_sdkver=30"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/android/arm64-v8a/release/libkernel.so" -Destination "${ModuleDistributionFile}"
}
if ($TargetPlatform -eq "iphone.arm64") {
	if (Test-Path -Path $ModuleDistributionFile) {
		Remove-Item -Force -Recurse -Path $ModuleDistributionFile
	}
	xmake "config" "--builddir=.build" "--mode=release" "--plat=iphoneos" "--arch=arm64" "--toolchain=xcode" "--cc=clang-20" "--cxx=clang-20" "--target_minver=15.6"
	xmake "build"
	Copy-Item -Force -Recurse -Path "${ModuleDirectory}/.build/iphoneos/arm64/release/libkernel.dylib" -Destination "${ModuleDistributionFile}"
}
Pop-Location
Write-Host -Object "!! DONE >> ${ModuleDistributionFile}"
