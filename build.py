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

def build(
	source: str,
	local: str,
	distribution: str,
	keystore: tuple[str, str] | None,
	temporary: str,
	platform: str,
) -> tuple[str, str] | None:
	destination = None
	if not utility.check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		return destination
	# build
	if utility.check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.build_project_module(build_kernel.__file__, build_kernel.build, platform)
		utility.build_project_module(build_shell.__file__, build_shell.build, platform)
		utility.build_project_module(build_script.__file__, build_script.build, platform)
		utility.build_project_module(build_assistant.__file__, build_assistant.build, platform)
	if utility.check_platform(platform, ['windows.amd64']):
		utility.build_project_module(build_assistant_plus.__file__, build_assistant_plus.build, platform)
	# root
	utility.fs_create_directory(
		f'{temporary}/artifact',
	)
	# module
	module_distribution_list = {
		'kernel': {
			'windows.amd64': '',
			'linux.amd64': '',
			'macintosh.arm64': '',
			'android.arm64': '',
			'iphone.arm64': '',
		},
		'shell': {
			'windows.amd64': '.exe',
			'linux.amd64': '',
			'macintosh.arm64': '',
			'android.arm64': '',
			'iphone.arm64': '',
		},
		'script': {
			'windows.amd64': '!.zip',
			'linux.amd64': '!.zip',
			'macintosh.arm64': '!.zip',
			'android.arm64': '!.zip',
			'iphone.arm64': '!.zip',
		},
		'assistant': {
			'windows.amd64': '.msix',
			'linux.amd64': '.AppImage',
			'macintosh.arm64': '.dmg',
			'android.arm64': '.apk',
			'iphone.arm64': '.ipa',
		},
		'assistant_plus': {
			'windows.amd64': '.msix',
		},
	}
	for module_name, module_distribution in module_distribution_list.items():
		module_distribution_extension = module_distribution.get(platform)
		if module_distribution_extension == None:
			continue
		if module_distribution_extension == '!.zip':
			utility.unpack_zip(
				f'{distribution}/{platform}.{module_name}{'.zip'}',
				f'{temporary}/artifact/{module_name}',
				f'{module_name}',
			)
		else:
			utility.fs_copy(
				f'{distribution}/{platform}.{module_name}{module_distribution.get(platform)}',
				f'{temporary}/artifact/{module_name}{module_distribution.get(platform)}',
			)
	# unembedded
	if utility.check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.fs_copy(
			f'{source}/common/unembedded/assistant',
			f'{temporary}/artifact/assistant',
		)
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_copy(
			f'{source}/common/unembedded/assistant_plus',
			f'{temporary}/artifact/assistant_plus',
		)
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_copy(
			f'{source}/common/unembedded/launch.cmd',
			f'{temporary}/artifact/launch.cmd',
		)
	if utility.check_platform(platform, ['linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.fs_copy(
			f'{source}/common/unembedded/launch.sh',
			f'{temporary}/artifact/launch.sh',
		)
	# miscellaneous
	utility.fs_copy(
		f'{local}/library/{platform}',
		f'{temporary}/artifact/library',
	)
	utility.fs_create_directory(
		f'{temporary}/artifact/workspace',
	)
	utility.fs_create_directory(
		f'{temporary}/artifact/temporary',
	)
	# bundle
	utility.pack_zip(
		f'{temporary}/artifact',
		f'{temporary}/artifact.zip',
		'Twinning',
	)
	destination = ('.zip', f'{temporary}/artifact.zip')
	return destination

if __name__ == '__main__':
	utility.build_project_bundle(__file__, build, sys.argv[1])
