param (
	$system,
	$build
)

$project = "$PSScriptRoot/.."

switch ($system) {
	"native" {
		if ($IsWindows -or ($IsWindows -eq $null)) {
			$native_compiler_c = "cl"
			$native_compiler_cxx = "cl"
		}
		if ($IsLinux) {
			$native_compiler_c = "clang"
			$native_compiler_cxx = "clang++"
		}
		if ($IsMacOs) {
			$native_compiler_c = "clang"
			$native_compiler_cxx = "clang++"
		}
		& "$PSScriptRoot/build.ps1" `
			-project "$project" `
			-system "$system" `
			-build "$build" `
			-native_compiler_c "$native_compiler_c" `
			-native_compiler_cxx "$native_compiler_cxx"
	}
	"android" {
		$android_ndk = "C:/Users/TwinKleS/AppData/Local/Android/Sdk/ndk/25.0.8775105"
		$android_arch = "arm_64_v8a"
		$android_platform = "28"
		& "$PSScriptRoot/build.ps1" `
			-project "$project" `
			-system "$system" `
			-build "$build" `
			-android_ndk "$android_ndk" `
			-android_arch "$android_arch" `
			-android_platform "$android_platform"
	}
	default {
		throw "-system should be 'native' or 'android'"
	}
}

