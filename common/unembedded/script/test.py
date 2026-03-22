import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
from common.python.utility import *

# ----------------

def main(
	platform: str,
) -> None:
	ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64'])
	project_directory = get_project()
	local_directory = get_project_local()
	test_directory = get_project_local('test')
	fs_create_directory(
		f'{test_directory}/workspace',
	)
	fs_create_directory(
		f'{test_directory}/temporary',
	)
	if check_platform(platform, ['windows.amd64']):
		fs_create_link(
			f'{test_directory}/library',
			f'{local_directory}/library/{platform}',
			True,
		)
		fs_create_link(
			f'{test_directory}/launch.cmd',
			f'{project_directory}/common/unembedded/script/launch.cmd',
			False,
		)
		fs_create_link(
			f'{test_directory}/kernel',
			f'{project_directory}/Kernel/.build/windows/x64/release/kernel.dll',
			False,
		)
		fs_create_link(
			f'{test_directory}/shell.exe',
			f'{project_directory}/Shell/.build/windows/x64/release/shell.exe',
			False,
		)
	if check_platform(platform, ['linux.amd64']):
		fs_create_link(
			f'{test_directory}/library',
			f'{local_directory}/library/{platform}',
			True,
		)
		fs_create_link(
			f'{test_directory}/launch.sh',
			f'{project_directory}/common/unembedded/script/launch.sh',
			False,
		)
		fs_create_link(
			f'{test_directory}/kernel',
			f'{project_directory}/Kernel/.build/linux/x86_64/release/kernel.so',
			False,
		)
		fs_create_link(
			f'{test_directory}/shell',
			f'{project_directory}/Shell/.build/linux/x86_64/release/shell',
			False,
		)
	if check_platform(platform, ['macintosh.arm64']):
		fs_create_link(
			f'{test_directory}/library',
			f'{local_directory}/library/{platform}',
			True,
		)
		fs_create_link(
			f'{test_directory}/launch.sh',
			f'{project_directory}/common/unembedded/script/launch.sh',
			False,
		)
		fs_create_link(
			f'{test_directory}/kernel',
			f'{project_directory}/Kernel/.build/macosx/arm64/release/libkernel.dylib',
			False,
		)
		fs_create_link(
			f'{test_directory}/shell',
			f'{project_directory}/Shell/.build/macosx/arm64/release/shell',
			False,
		)
	fs_create_link(
		f'{test_directory}/script',
		f'{project_directory}/Script/dist',
		True,
	)
	fs_create_link(
		f'{test_directory}/assistant',
		f'{project_directory}/common/unembedded/configuration/assistant',
		True,
	)
	fs_create_link(
		f'{test_directory}/assistant_plus',
		f'{project_directory}/common/unembedded/configuration/assistant_plus',
		True,
	)
	print(f'>> TEST >> {test_directory}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
