import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
import common.script.utility as utility
import module.kernel.build as build_kernel
import module.script.build as build_script
import module.shell.build as build_shell
import module.assistant.build as build_assistant

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
	if not utility.project_check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		return destination
	# build
	utility.project_build_module(build_kernel.__file__, build_kernel.build, platform)
	utility.project_build_module(build_script.__file__, build_script.build, platform)
	utility.project_build_module(build_shell.__file__, build_shell.build, platform)
	utility.project_build_module(build_assistant.__file__, build_assistant.build, platform)
	# root
	utility.fs_create_directory(
		f'{temporary}/artifact',
	)
	# module
	module_distribution_list: dict[str, str | dict[str, str]] = {
		'kernel': '',
		'script': '.zip!',
		'shell': '',
		'assistant': {
			'windows.amd64': '.msix',
			'linux.amd64': '.AppImage',
			'macintosh.arm64': '.dmg',
			'android.arm64': '.apk',
			'iphone.arm64': '.ipa',
		},
	}
	for module_name, module_distribution in module_distribution_list.items():
		module_distribution_extension = module_distribution if isinstance(module_distribution, str) else module_distribution[platform]
		if module_distribution_extension == '.zip!':
			utility.ex_archive_unpack_zip(
				f'{distribution}/{platform}.{module_name}{module_distribution_extension[:-1]}',
				f'{temporary}/artifact/{module_name}',
				f'{module_name}',
			)
		else:
			utility.fs_copy(
				f'{distribution}/{platform}.{module_name}{module_distribution_extension}',
				f'{temporary}/artifact/{module_name}{module_distribution_extension}',
			)
	# asset
	utility.fs_copy(
		f'{source}/common/asset/assistant',
		f'{temporary}/artifact/assistant',
	)
	utility.fs_copy(
		f'{source}/common/asset/launch.sh',
		f'{temporary}/artifact/launch.sh',
	)
	utility.fs_copy(
		f'{source}/common/asset/launch.ps1',
		f'{temporary}/artifact/launch.ps1',
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
	utility.ex_archive_pack_zip(
		f'{temporary}/artifact.zip',
		f'{temporary}/artifact',
		f'Twinning',
	)
	destination = ('.zip', f'{temporary}/artifact.zip')
	return destination

if __name__ == '__main__':
	utility.project_build_bundle(__file__, build, sys.argv[1])
