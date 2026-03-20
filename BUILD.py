import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
from common.python.utility import *
import Kernel.BUILD
import Shell.BUILD
import Script.BUILD
import Assistant.BUILD
import AssistantPlus.BUILD

# ----------------

def main(
	platform: str,
) -> None:
	ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64'])
	# build
	Kernel.BUILD.main(platform)
	Shell.BUILD.main(platform)
	Script.BUILD.main('any')
	Assistant.BUILD.main(platform)
	if check_platform(platform, ['windows.amd64']):
		AssistantPlus.BUILD.main(platform)
	# path
	project_directory = get_project()
	distribution_directory = get_project_distribution(None)
	bundle_directory = get_project_distribution(f'{platform}.bundle')
	bundle_file = get_project_distribution(f'{platform}.bundle.zip')
	# root
	fs_remove(
		f'{bundle_directory}',
	)
	fs_create_directory(
		f'{bundle_directory}',
	)
	# workspace
	fs_create_directory(
		f'{bundle_directory}/workspace',
	)
	# temporary
	fs_create_directory(
		f'{bundle_directory}/temporary',
	)
	# library
	if True:
		fs_create_directory(
			f'{bundle_directory}/library',
		)
	if check_platform(platform, ['windows.amd64']):
		fs_copy(
			f'{project_directory}/Assistant/windows/package/Library/libc++.dll',
			f'{bundle_directory}/library/libc++.dll',
		)
		fs_copy(
			f'{project_directory}/Assistant/windows/package/Library/libunwind.dll',
			f'{bundle_directory}/library/libunwind.dll',
		)
	if check_platform(platform, ['android.arm64']):
		fs_copy(
			f'{project_directory}/Assistant/android/app/src/main/jniLibs/arm64-v8a/libc++_shared.so',
			f'{bundle_directory}/library/libc++_shared.so',
		)
	# launch
	if check_platform(platform, ['windows.amd64']):
		fs_copy(
			f'{project_directory}/common/unembedded/launch.cmd',
			f'{bundle_directory}/launch.cmd',
		)
	if check_platform(platform, ['linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		fs_copy(
			f'{project_directory}/common/unembedded/launch.sh',
			f'{bundle_directory}/launch.sh',
		)
	# kernel
	if check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		fs_copy(
			f'{distribution_directory}/{platform}.kernel',
			f'{bundle_directory}/kernel',
		)
	# shell
	if check_platform(platform, ['windows.amd64']):
		fs_copy(
			f'{distribution_directory}/{platform}.shell',
			f'{bundle_directory}/shell.exe',
		)
	if check_platform(platform, ['linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		fs_copy(
			f'{distribution_directory}/{platform}.shell',
			f'{bundle_directory}/shell',
		)
	# script
	if True:
		unpack_zip(
			f'{distribution_directory}/any.script.zip',
			f'{bundle_directory}',
		)
	# assistant
	if True:
		fs_copy(
			f'{project_directory}/common/unembedded/assistant',
			f'{bundle_directory}/assistant',
		)
	if check_platform(platform, ['windows.amd64']):
		fs_copy(
			f'{distribution_directory}/{platform}.assistant.msix',
			f'{bundle_directory}/assistant.msix',
		)
	if check_platform(platform, ['linux.amd64']):
		fs_copy(
			f'{distribution_directory}/{platform}.assistant.zip',
			f'{bundle_directory}/assistant.zip',
		)
	if check_platform(platform, ['macintosh.arm64']):
		fs_copy(
			f'{distribution_directory}/{platform}.assistant.dmg',
			f'{bundle_directory}/assistant.dmg',
		)
	if check_platform(platform, ['android.arm64']):
		fs_copy(
			f'{distribution_directory}/{platform}.assistant.apk',
			f'{bundle_directory}/assistant.apk',
		)
	if check_platform(platform, ['iphone.arm64']):
		fs_copy(
			f'{distribution_directory}/{platform}.assistant.ipa',
			f'{bundle_directory}/assistant.ipa',
		)
	# assistant_plus
	if check_platform(platform, ['windows.amd64']):
		fs_copy(
			f'{project_directory}/common/unembedded/assistant_plus',
			f'{bundle_directory}/assistant_plus',
		)
		fs_copy(
			f'{distribution_directory}/{platform}.assistant_plus.msix',
			f'{bundle_directory}/assistant_plus.msix',
		)
	# done
	pack_zip(
		'Twinning',
		f'{bundle_directory}',
		f'{bundle_file}',
	)
	print(f'>> BUILD >> {bundle_file}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
