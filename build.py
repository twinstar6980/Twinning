import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
import common.script.utility as utility
import Kernel.build as build_kernel
import Shell.build as build_shell
import Script.build as build_script
import Assistant.build as build_assistant
import AssistantPlus.build as build_assistant_plus

# ----------------

def main(
	platform: str,
) -> None:
	utility.ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64'])
	# build
	build_kernel.main(platform)
	build_shell.main(platform)
	build_script.main('any.any')
	build_assistant.main(platform)
	if utility.check_platform(platform, ['windows.amd64']):
		build_assistant_plus.main(platform)
	# path
	project_directory = utility.get_project()
	local_directory = utility.get_project_local()
	distribution_directory = utility.get_project_distribution()
	bundle_directory = utility.get_project_distribution(f'{platform}.bundle')
	bundle_file = utility.get_project_distribution(f'{platform}.bundle.zip')
	# root
	utility.fs_remove(
		f'{bundle_directory}',
	)
	utility.fs_create_directory(
		f'{bundle_directory}',
	)
	# workspace
	utility.fs_create_directory(
		f'{bundle_directory}/workspace',
	)
	# temporary
	utility.fs_create_directory(
		f'{bundle_directory}/temporary',
	)
	# library
	utility.fs_copy(
		f'{local_directory}/library/{platform}',
		f'{bundle_directory}/library',
	)
	# launch
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_copy(
			f'{project_directory}/common/unembedded/script/launch.cmd',
			f'{bundle_directory}/launch.cmd',
		)
	if utility.check_platform(platform, ['linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.fs_copy(
			f'{project_directory}/common/unembedded/script/launch.sh',
			f'{bundle_directory}/launch.sh',
		)
	# kernel
	if utility.check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.kernel',
			f'{bundle_directory}/kernel',
		)
	# shell
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.shell',
			f'{bundle_directory}/shell.exe',
		)
	if utility.check_platform(platform, ['linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.shell',
			f'{bundle_directory}/shell',
		)
	# script
	if True:
		utility.unpack_zip(
			f'{distribution_directory}/any.any.script.zip',
			f'{bundle_directory}',
		)
	# assistant
	if True:
		utility.fs_copy(
			f'{project_directory}/common/unembedded/configuration/assistant',
			f'{bundle_directory}/assistant',
		)
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.assistant.msix',
			f'{bundle_directory}/assistant.msix',
		)
	if utility.check_platform(platform, ['linux.amd64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.assistant.zip',
			f'{bundle_directory}/assistant.zip',
		)
	if utility.check_platform(platform, ['macintosh.arm64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.assistant.dmg',
			f'{bundle_directory}/assistant.dmg',
		)
	if utility.check_platform(platform, ['android.arm64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.assistant.apk',
			f'{bundle_directory}/assistant.apk',
		)
	if utility.check_platform(platform, ['iphone.arm64']):
		utility.fs_copy(
			f'{distribution_directory}/{platform}.assistant.ipa',
			f'{bundle_directory}/assistant.ipa',
		)
	# assistant_plus
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_copy(
			f'{project_directory}/common/unembedded/configuration/assistant_plus',
			f'{bundle_directory}/assistant_plus',
		)
		utility.fs_copy(
			f'{distribution_directory}/{platform}.assistant_plus.msix',
			f'{bundle_directory}/assistant_plus.msix',
		)
	# done
	utility.pack_zip(
		'Twinning',
		f'{bundle_directory}',
		f'{bundle_file}',
	)
	print(f'>> BUILD >> {bundle_file}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
