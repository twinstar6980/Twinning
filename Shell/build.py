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
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.execute_command(module_directory, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=windows',
			'--arch=x64',
			'--toolchain=mingw[clang]',
			'--runtimes=c++_shared',
		])
		utility.execute_command(module_directory, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{module_directory}/.build/windows/x64/release/shell.exe',
			f'{module_distribution_file}',
		)
		utility.strip_windows_binary(
			f'{module_distribution_file}',
		)
		utility.apply_windows_manifest(
			f'{module_distribution_file}',
			f'{module_directory}/shell/resource/windows/application.manifest',
		)
		utility.sign_windows_executable(
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['linux.amd64']):
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.execute_command(module_directory, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=linux',
			'--arch=x86_64',
			'--toolchain=clang',
			'--runtimes=c++_shared',
		])
		utility.execute_command(module_directory, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{module_directory}/.build/linux/x86_64/release/shell',
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['macintosh.arm64']):
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.execute_command(module_directory, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=macosx',
			'--arch=arm64',
			'--toolchain=xcode',
			'--target_minver=13.5',
			'--cc=clang',
			'--cxx=clang',
		])
		utility.execute_command(module_directory, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{module_directory}/.build/macosx/arm64/release/shell',
			f'{module_distribution_file}',
		)
		utility.sign_macintosh_executable(
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['android.arm64']):
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.execute_command(module_directory, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=android',
			'--arch=arm64-v8a',
			'--toolchain=ndk',
			'--ndk_sdkver=30',
			'--runtimes=c++_shared',
		])
		utility.execute_command(module_directory, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{module_directory}/.build/android/arm64-v8a/release/shell',
			f'{module_distribution_file}',
		)
	if utility.check_platform(platform, ['iphone.arm64']):
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.execute_command(module_directory, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=iphoneos',
			'--arch=arm64',
			'--toolchain=xcode',
			'--target_minver=16.6',
			'--cc=clang',
			'--cxx=clang',
		])
		utility.execute_command(module_directory, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{module_directory}/.build/iphoneos/arm64/release/shell',
			f'{module_distribution_file}',
		)
		utility.sign_iphone_executable(
			f'{module_distribution_file}',
		)
	print(f'>> BUILD >> {module_distribution_file}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
