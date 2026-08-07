import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
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
		utility.project_setup_library(
			platform,
		)
		utility.sh_execute_command(source, [
			'flutter',
			'build',
			'windows',
			'--release',
			'--no-tree-shake-icons',
		])
		utility.fs_copy(
			f'{source}/build/windows/x64/runner/Release',
			f'{temporary}/artifact',
		)
		utility.ex_windows_create_pri_resource(
			f'{temporary}/artifact',
		)
		utility.ex_windows_pack_msix(
			f'{temporary}/artifact.msix',
			f'{temporary}/artifact',
		)
		utility.ex_windows_sign(
			f'{temporary}/artifact.msix',
			keystore,
			'msix',
		)
		destination = ('.msix', f'{temporary}/artifact.msix')
	if utility.project_check_platform(platform, ['linux.amd64']):
		utility.project_setup_library(
			platform,
		)
		utility.sh_execute_command(source, [
			'flutter',
			'build',
			'linux',
			'--release',
			'--no-tree-shake-icons',
			'--target-platform', 'linux-x64',
		], {
			'CXXFLAGS': '-stdlib=libc++',
		})
		utility.fs_copy(
			f'{source}/build/linux/x64/release/bundle',
			f'{temporary}/artifact',
			follow_link=True,
		)
		utility.ex_linux_pack_appimage(
			f'{temporary}/artifact.AppImage',
			f'{temporary}/artifact',
		)
		destination = ('.AppImage', f'{temporary}/artifact.AppImage')
	if utility.project_check_platform(platform, ['macintosh.arm64']):
		utility.project_setup_library(
			platform,
		)
		utility.sh_execute_command(source, [
			'flutter',
			'build',
			'macos',
			'--release',
			'--no-tree-shake-icons',
		])
		utility.fs_copy(
			f'{source}/build/macos/Build/Products/Release/Runner.app',
			f'{temporary}/artifact.app',
			follow_link=True,
		)
		utility.ex_macintosh_sign(
			f'{temporary}/artifact.app',
			keystore,
			'app',
		)
		utility.ex_macintosh_pack_dmg(
			f'{temporary}/artifact.dmg',
			f'{temporary}/artifact.app',
			'Twinning Assistant',
		)
		destination = ('.dmg', f'{temporary}/artifact.dmg')
	if utility.project_check_platform(platform, ['android.arm64']):
		utility.project_setup_library(
			platform,
		)
		utility.sh_execute_command(source, [
			'flutter',
			'build',
			'apk',
			'--release',
			'--no-tree-shake-icons',
			'--target-platform', 'android-arm64',
			'--split-per-abi',
		])
		utility.fs_copy(
			f'{source}/build/app/outputs/flutter-apk/app-arm64-v8a-release.apk',
			f'{temporary}/artifact.apk',
		)
		utility.ex_android_sign(
			f'{temporary}/artifact.apk',
			keystore,
			'apk',
			(False, False, True),
		)
		destination = ('.apk', f'{temporary}/artifact.apk')
	if utility.project_check_platform(platform, ['iphone.arm64']):
		utility.project_setup_library(
			platform,
		)
		utility.sh_execute_command(source, [
			'flutter',
			'build',
			'ios',
			'--release',
			'--no-tree-shake-icons',
			'--no-codesign',
		])
		utility.fs_copy(
			f'{source}/build/ios/iphoneos/Runner.app',
			f'{temporary}/artifact.app',
			follow_link=True,
		)
		utility.ex_iphone_sign(
			f'{temporary}/artifact.app',
			keystore,
			'app',
		)
		utility.ex_iphone_pack_ipa(
			f'{temporary}/artifact.ipa',
			f'{temporary}/artifact.app',
			'Twinning Assistant',
		)
		destination = ('.ipa', f'{temporary}/artifact.ipa')
	return destination

if __name__ == '__main__':
	utility.project_build_module(__file__, build, sys.argv[1])
