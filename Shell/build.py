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
	if utility.project_check_platform(platform, ['windows.amd64']):
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
			f'{source}/.build/windows/x64/release/shell.exe',
			f'{temporary}/artifact.exe',
		)
		utility.ex_windows_strip_executable(
			f'{temporary}/artifact.exe',
		)
		utility.ex_windows_import_manifest(
			f'{temporary}/artifact.exe',
			f'{source}/shell/resource/windows/application.manifest',
			1,
		)
		utility.ex_windows_sign(
			f'{temporary}/artifact.exe',
			keystore,
			'pe',
		)
		destination = ('.exe', f'{temporary}/artifact.exe')
	if utility.project_check_platform(platform, ['linux.amd64']):
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
			f'{source}/.build/linux/x86_64/release/shell',
			f'{temporary}/artifact',
		)
		destination = ('', f'{temporary}/artifact')
	if utility.project_check_platform(platform, ['macintosh.arm64']):
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
			f'{source}/.build/macosx/arm64/release/shell',
			f'{temporary}/artifact',
		)
		utility.ex_macintosh_sign(
			f'{temporary}/artifact',
			keystore,
			'macho',
		)
		destination = ('', f'{temporary}/artifact')
	if utility.project_check_platform(platform, ['android.arm64']):
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
			f'{source}/.build/android/arm64-v8a/release/shell',
			f'{temporary}/artifact',
		)
		destination = ('', f'{temporary}/artifact')
	if utility.project_check_platform(platform, ['iphone.arm64']):
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
			f'{source}/.build/iphoneos/arm64/release/shell',
			f'{temporary}/artifact',
		)
		utility.ex_iphone_sign(
			f'{temporary}/artifact',
			keystore,
			'macho',
		)
		destination = ('', f'{temporary}/artifact')
	return destination

if __name__ == '__main__':
	utility.project_build_module(__file__, build, sys.argv[1])
