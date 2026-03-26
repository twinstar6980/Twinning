import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common.script.utility as utility

# ----------------

def build(
	source: str,
	keystore: tuple[str, str] | None,
	temporary: str,
	platform: str,
) -> tuple[str, str] | None:
	destination = None
	if utility.check_platform(platform, ['windows.amd64']):
		utility.sh_execute_command(source, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=windows',
			'--arch=x64',
			'--toolchain=mingw[clang]',
			'--runtimes=c++_shared',
		])
		utility.sh_execute_command(source, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{source}/.build/windows/x64/release/kernel.dll',
			f'{temporary}/artifact.dll',
		)
		utility.strip_windows_binary(
			f'{temporary}/artifact.dll',
			f'{temporary}/artifact.dll',
		)
		utility.sign_windows_executable(
			f'{temporary}/artifact.dll',
			f'{temporary}/artifact.dll',
			'dll',
			keystore,
		)
		destination = ('', f'{temporary}/artifact.dll')
	if utility.check_platform(platform, ['linux.amd64']):
		utility.sh_execute_command(source, [
			'xmake',
			'config',
			'--builddir=.build',
			'--mode=release',
			'--plat=linux',
			'--arch=x86_64',
			'--toolchain=clang',
			'--runtimes=c++_shared',
		])
		utility.sh_execute_command(source, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{source}/.build/linux/x86_64/release/libkernel.so',
			f'{temporary}/artifact.so',
		)
		destination = ('', f'{temporary}/artifact.so')
	if utility.check_platform(platform, ['macintosh.arm64']):
		utility.sh_execute_command(source, [
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
		utility.sh_execute_command(source, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{source}/.build/macosx/arm64/release/libkernel.dylib',
			f'{temporary}/artifact.dylib',
		)
		utility.sign_macintosh_executable(
			f'{temporary}/artifact.dylib',
			f'{temporary}/artifact.dylib',
			keystore,
		)
		utility.fs_copy(
			f'{temporary}/artifact.dylib',
			f'{destination}',
		)
		destination = ('', f'{temporary}/artifact.dylib')
	if utility.check_platform(platform, ['android.arm64']):
		utility.sh_execute_command(source, [
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
		utility.sh_execute_command(source, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{source}/.build/android/arm64-v8a/release/libkernel.so',
			f'{temporary}/artifact.so',
		)
		destination = ('', f'{temporary}/artifact.so')
	if utility.check_platform(platform, ['iphone.arm64']):
		utility.sh_execute_command(source, [
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
		utility.sh_execute_command(source, [
			'xmake',
			'build',
		])
		utility.fs_copy(
			f'{source}/.build/iphoneos/arm64/release/libkernel.dylib',
			f'{temporary}/artifact.dylib',
		)
		utility.sign_iphone_executable(
			f'{temporary}/artifact.dylib',
			f'{temporary}/artifact.dylib',
			keystore,
		)
		destination = ('', f'{temporary}/artifact.dylib')
	return destination

if __name__ == '__main__':
	utility.build_project_module(__file__, build, sys.argv[1])
