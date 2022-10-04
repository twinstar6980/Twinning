param (
	$project,
	$system,
	$build,
	#$generator,
	$native_compiler_c,
	$native_compiler_cxx,
	$android_ndk,
	$android_api,
	$android_arch,
	$ios_toolchain,
	$ios_compiler_c,
	$ios_compiler_cxx,
	$ios_api,
	$ios_arch
)

$generator = "Ninja"

function prepare {
	switch ($system) {
		"native" {
			if ($IsWindows -or ($IsWindows -eq $null)) {
				$global:system_name = "windows"
				$global:system_name_cmake = "Windows"
				$global:compiler_name = "msvc"
				$native_arch = (Get-Process -Id $PID).StartInfo.EnvironmentVariables["PROCESSOR_ARCHITECTURE"]
			}
			if ($IsLinux) {
				$global:system_name = "linux"
				$global:system_name_cmake = "Linux"
				$global:compiler_name = "clang"
				$native_arch = (& "arch")
			}
			if ($IsMacOs) {
				$global:system_name = "macos"
				$global:system_name_cmake = "Darwin"
				$global:compiler_name = "clang"
				$native_arch = (& "uname" "-m")
			}
			switch ($native_arch) {
				"x86" {
					$global:arch_name = "x86_32"
				}
				"x86_64" {
					$global:arch_name = "x86_64"
				}
				"amd64" {
					$global:arch_name = "x86_64"
				}
				default {
					throw "unknown native_arch : $native_arch"
				}
			}
		}
		"android" {
			$global:system_name = "android"
			$global:system_name_cmake = "Android"
			$global:compiler_name = "clang"
			switch ($android_arch) {
				"x86_32" {
					$global:arch_name = "x86_32"
					$global:android_arch_cmake = "x86"
				}
				"x86_64" {
					$global:arch_name = "x86_64"
					$global:android_arch_cmake = "x86_64"
				}
				"arm_32" {
					$global:arch_name = "arm_32"
					$global:android_arch_cmake = "armeabi-v7a"
				}
				"arm_64" {
					$global:arch_name = "arm_64"
					$global:android_arch_cmake = "arm64-v8a"
				}
				default {
					throw "-android_arch should be 'x86_32' or 'x86_64' or 'arm_32' or 'arm_64' : $android_arch"
				}
			}
		}
		"ios" {
			$global:system_name = "ios"
			$global:system_name_cmake = "iOS"
			$global:compiler_name = "clang"
			switch ($ios_arch) {
				"arm_64" {
					$global:arch_name = "arm_64"
					$global:ios_arch_cmake = "arm64"
				}
				default {
					throw "-ios_arch should be 'arm_64' : $ios_arch"
				}
			}
		}
		default {
			throw "-system should be 'native' or 'android' or 'ios' : $system"
		}
	}
	switch ($build) {
		"debug" {
			$global:build_name = "debug"
			$global:build_name_cmake = "Debug"
		}
		"release" {
			$global:build_name = "release"
			$global:build_name_cmake = "Release"
		}
		"release_minimum" {
			$global:build_name = "release_minimum"
			$global:build_name_cmake = "MinSizeRel"
		}
		"release_debug" {
			$global:build_name = "release_debug"
			$global:build_name_cmake = "RelWithDebInfo"
		}
		default {
			throw "-build should be 'debug' or 'release' or 'release_minimum' or 'release_debug'"
		}
	}
	$global:project_source = "$project"
	$global:project_output = "$project/.build/$system_name.$arch_name.$compiler_name.$build_name"
}
function check_command_exist {
	param ($command)
	try {
		if (Get-Command "$command" -errorAction "SilentlyContinue") {
			return $true
		}
	} catch {
		return $false
	}
}
function check_required_tool {
	if (!(check_command_exist "cmake")) {
		throw "cmake is not found"
	}
	if ($system -eq "native") {
		if (!(check_command_exist "ninja")) {
			throw "ninja is not found"
		}
		if (!(check_command_exist "$native_compiler_c")) {
			throw "native_compiler_c is not found : $native_compiler_c"
		}
		if (!(check_command_exist "$native_compiler_cxx")) {
			throw "native_compiler_cxx is not found : $native_compiler_cxx"
		}
	}
	if ($system -eq "android") {
		if (!(Test-Path "$android_ndk")) {
			throw "android_ndk is not found : $android_ndk"
		}
		$global:android_toolchain = "$android_ndk/build/cmake/android.toolchain.cmake"
		if (!(Test-Path "$android_toolchain")) {
			throw "android_toolchain is not found : $android_toolchain"
		}
	}
	if ($system -eq "ios") {
		if (!(Test-Path "$ios_toolchain")) {
			throw "ios_toolchain is not found : $ios_toolchain"
		}
	}
}
function cmake_clear {
	if (Test-Path "$project_output") {
		Remove-Item "$project_output" -Recurse
	}
}
function cmake_generate {
	switch ($system) {
		"native" {
			& "cmake" `
				"-S" "$project_source" `
				"-B" "$project_output" `
				"-D" "CMAKE_SYSTEM_NAME=$system_name_cmake" `
				"-D" "CMAKE_BUILD_TYPE=$build_name_cmake" `
				"-D" "CMAKE_C_COMPILER=$native_compiler_c" `
				"-D" "CMAKE_CXX_COMPILER=$native_compiler_cxx" `
				"-G" "$generator"
		}
		"android" {
			& "cmake" `
				"-S" "$project_source" `
				"-B" "$project_output" `
				"-D" "CMAKE_SYSTEM_NAME=$system_name_cmake" `
				"-D" "CMAKE_BUILD_TYPE=$build_name_cmake" `
				"-D" "CMAKE_ANDROID_NDK=$android_ndk" `
				"-D" "CMAKE_ANDROID_API=$android_api" `
				"-D" "CMAKE_ANDROID_ARCH_ABI=$android_arch_cmake" `
				"-D" "CMAKE_ANDROID_STL_TYPE=c++_shared" `
				"-G" "$generator"
		}
		"ios" {
			& "cmake" `
				"-S" "$project_source" `
				"-B" "$project_output" `
				"-D" "CMAKE_SYSTEM_NAME=$system_name_cmake" `
				"-D" "CMAKE_BUILD_TYPE=$build_name_cmake" `
				"-D" "CMAKE_TOOLCHAIN_FILE=$ios_toolchain" `
				"-D" "CMAKE_C_COMPILER=$ios_compiler_c" `
				"-D" "CMAKE_CXX_COMPILER=$ios_compiler_cxx" `
				"-D" "PLATFORM=OS64" `
				"-D" "DEPLOYMENT_TARGET=$ios_api" `
				"-D" "ARCHS=$ios_arch_cmake" `
				"-G" "$generator"
		}
	}
}
function cmake_build {
	& "cmake" `
		"--build" "$project_output"
}

Write-Host "step : prepare"
prepare
Write-Host "step finish"

Write-Host "target : $system_name.$arch_name.$compiler_name.$build_name"

Write-Host "step : check"
check_required_tool
Write-Host "step finish"

Write-Host "step : clear"
cmake_clear
Write-Host "step finish"

Write-Host "step : generate"
cmake_generate
Write-Host "step finish"

Write-Host "step : build"
cmake_build
Write-Host "step finish"

Write-Host "done : $project_output"
