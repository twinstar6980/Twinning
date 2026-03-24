import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common.script.utility as utility

# ----------------

def main(
	platform: str,
) -> None:
	utility.ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64'])
	module_directory, module_name = utility.get_project_module(__file__)
	module_distribution_file = utility.get_project_distribution(f'{platform}.{module_name}')
	if utility.check_platform(platform, ['windows.amd64']):
		module_distribution_file += '.msix'
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.setup_common_cpp_library(
			platform,
		)
		utility.execute_command(module_directory, [
			'flutter',
			'build',
			'windows',
			'--release',
			'--no-tree-shake-icons',
		])
		utility.pack_windows_msix(
			'assistant',
			f'{module_directory}/build/windows/x64/runner/Release',
			f'{module_distribution_file}',
		)
		utility.sign_windows_executable(
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['linux.amd64']):
		module_distribution_file += '.zip'
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.setup_common_cpp_library(
			platform,
		)
		utility.execute_command(module_directory, [
			'flutter',
			'build',
			'linux',
			'--release',
			'--no-tree-shake-icons',
			'--target-platform', 'linux-x64',
		], {
			'CXXFLAGS': '-stdlib=libc++',
		})
		utility.pack_zip(
			'assistant',
			f'{module_directory}/build/linux/x64/release/bundle',
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['macintosh.arm64']):
		module_distribution_file += '.dmg'
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.setup_common_cpp_library(
			platform,
		)
		utility.execute_command(module_directory, [
			'flutter',
			'build',
			'macos',
			'--release',
			'--no-tree-shake-icons',
		])
		utility.sign_macintosh_executable(
			f'{module_directory}/build/macos/Build/Products/Release/Runner.app',
		)
		utility.pack_macintosh_dmg(
			'Twinning Assistant',
			f'{module_directory}/build/macos/Build/Products/Release/Runner.app',
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['android.arm64']):
		module_distribution_file += '.apk'
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.setup_common_cpp_library(
			platform,
		)
		utility.execute_command(module_directory, [
			'flutter',
			'build',
			'apk',
			'--release',
			'--no-tree-shake-icons',
			'--target-platform', 'android-arm64',
			'--split-per-abi',
		])
		utility.fs_copy(
			f'{module_directory}/build/app/outputs/flutter-apk/app-arm64-v8a-release.apk',
			f'{module_distribution_file}',
		)
		utility.sign_android_apk(
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['iphone.arm64']):
		module_distribution_file += '.ipa'
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.setup_common_cpp_library(
			platform,
		)
		utility.execute_command(module_directory, [
			'flutter',
			'build',
			'ios',
			'--release',
			'--no-tree-shake-icons',
			'--no-codesign',
		])
		utility.sign_iphone_executable(
			f'{module_directory}/build/ios/iphoneos/Runner.app',
		)
		utility.pack_iphone_ipa(
			'Twinning Assistant',
			f'{module_directory}/build/ios/iphoneos/Runner.app',
			f'{module_distribution_file}',
		)
	print(f'>> BUILD >> {module_distribution_file}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
