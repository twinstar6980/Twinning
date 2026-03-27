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
		utility.setup_project_library(
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
		utility.pack_windows_msix(
			f'{temporary}/artifact',
			f'{temporary}/artifact.msix',
			True,
		)
		utility.sign_windows_executable(
			f'{temporary}/artifact.msix',
			f'{temporary}/artifact.msix',
			'msix',
			keystore,
		)
		destination = ('.msix', f'{temporary}/artifact.msix')
	if utility.check_platform(platform, ['linux.amd64']):
		utility.setup_project_library(
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
		)
		utility.pack_zip(
			f'{temporary}/artifact',
			f'{temporary}/artifact.zip',
			'assistant',
		)
		destination = ('.zip', f'{temporary}/artifact.zip')
	if utility.check_platform(platform, ['macintosh.arm64']):
		utility.setup_project_library(
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
		utility.sign_macintosh_executable(
			f'{temporary}/artifact.app',
			f'{temporary}/artifact.app',
			keystore,
		)
		utility.pack_macintosh_dmg(
			f'{temporary}/artifact.app',
			f'{temporary}/artifact.dmg',
			'Twinning Assistant',
		)
		utility.sign_macintosh_executable(
			f'{temporary}/artifact.dmg',
			f'{temporary}/artifact.dmg',
			keystore,
		)
		destination = ('.dmg', f'{temporary}/artifact.dmg')
	if utility.check_platform(platform, ['android.arm64']):
		utility.setup_project_library(
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
		utility.sign_android_apk(
			f'{temporary}/artifact.apk',
			f'{temporary}/artifact.apk',
			keystore,
		)
		destination = ('.apk', f'{temporary}/artifact.apk')
	if utility.check_platform(platform, ['iphone.arm64']):
		utility.setup_project_library(
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
		utility.sign_iphone_executable(
			f'{temporary}/artifact.app',
			f'{temporary}/artifact.app',
			keystore,
		)
		utility.pack_iphone_ipa(
			f'{temporary}/artifact.app',
			f'{temporary}/artifact.ipa',
			'Twinning Assistant',
		)
		destination = ('.ipa', f'{temporary}/artifact.ipa')
	return destination

if __name__ == '__main__':
	utility.build_project_module(__file__, build, sys.argv[1])
