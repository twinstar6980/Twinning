import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
import common.script.utility as utility

# ----------------

def main(
	platform: str,
) -> None:
	utility.ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64'])
	project_directory = utility.get_project()
	local_directory = utility.get_project_local()
	test_directory = utility.get_project_local('test')
	utility.fs_create_directory(
		f'{test_directory}/workspace',
	)
	utility.fs_create_directory(
		f'{test_directory}/temporary',
	)
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_create_link(
			f'{test_directory}/library',
			f'{local_directory}/library/{platform}',
			True,
		)
		utility.fs_create_link(
			f'{test_directory}/launch.cmd',
			f'{project_directory}/common/unembedded/script/launch.cmd',
			False,
		)
		utility.fs_create_link(
			f'{test_directory}/kernel',
			f'{project_directory}/Kernel/.build/windows/x64/release/kernel.dll',
			False,
		)
		utility.fs_create_link(
			f'{test_directory}/shell.exe',
			f'{project_directory}/Shell/.build/windows/x64/release/shell.exe',
			False,
		)
	if utility.check_platform(platform, ['linux.amd64']):
		utility.fs_create_link(
			f'{test_directory}/library',
			f'{local_directory}/library/{platform}',
			True,
		)
		utility.fs_create_link(
			f'{test_directory}/launch.sh',
			f'{project_directory}/common/unembedded/script/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test_directory}/kernel',
			f'{project_directory}/Kernel/.build/linux/x86_64/release/kernel.so',
			False,
		)
		utility.fs_create_link(
			f'{test_directory}/shell',
			f'{project_directory}/Shell/.build/linux/x86_64/release/shell',
			False,
		)
	if utility.check_platform(platform, ['macintosh.arm64']):
		utility.fs_create_link(
			f'{test_directory}/library',
			f'{local_directory}/library/{platform}',
			True,
		)
		utility.fs_create_link(
			f'{test_directory}/launch.sh',
			f'{project_directory}/common/unembedded/script/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test_directory}/kernel',
			f'{project_directory}/Kernel/.build/macosx/arm64/release/libkernel.dylib',
			False,
		)
		utility.fs_create_link(
			f'{test_directory}/shell',
			f'{project_directory}/Shell/.build/macosx/arm64/release/shell',
			False,
		)
	utility.fs_create_link(
		f'{test_directory}/script',
		f'{project_directory}/Script/dist',
		True,
	)
	utility.fs_create_link(
		f'{test_directory}/assistant',
		f'{project_directory}/common/unembedded/configuration/assistant',
		True,
	)
	utility.fs_create_link(
		f'{test_directory}/assistant_plus',
		f'{project_directory}/common/unembedded/configuration/assistant_plus',
		True,
	)
	print(f'>> TEST >> {test_directory}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
