import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
import common.script.utility as utility

# ----------------

def test(
	platform: str,
) -> None:
	utility.ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64'])
	source = utility.locate_project()
	local = utility.locate_project_local()
	test = utility.locate_project_local('test')
	utility.fs_create_directory(
		f'{test}',
	)
	if utility.check_platform(platform, ['windows.amd64']):
		utility.fs_create_link(
			f'{test}/kernel',
			f'{source}/Kernel/.build/windows/x64/release/kernel.dll',
			False,
		)
		utility.fs_create_link(
			f'{test}/shell.exe',
			f'{source}/Shell/.build/windows/x64/release/shell.exe',
			False,
		)
		utility.fs_create_link(
			f'{test}/script',
			f'{source}/Script/dist',
			True,
		)
		utility.fs_create_link(
			f'{test}/launch.cmd',
			f'{source}/common/unembedded/launch.cmd',
			False,
		)
		utility.fs_create_link(
			f'{test}/assistant',
			f'{source}/common/unembedded/assistant',
			True,
		)
		utility.fs_create_link(
			f'{test}/assistant_plus',
			f'{source}/common/unembedded/assistant_plus',
			True,
		)
	if utility.check_platform(platform, ['linux.amd64']):
		utility.fs_create_link(
			f'{test}/kernel',
			f'{source}/Kernel/.build/linux/x86_64/release/kernel.so',
			False,
		)
		utility.fs_create_link(
			f'{test}/shell',
			f'{source}/Shell/.build/linux/x86_64/release/shell',
			False,
		)
		utility.fs_create_link(
			f'{test}/script',
			f'{source}/Script/dist',
			True,
		)
		utility.fs_create_link(
			f'{test}/launch.sh',
			f'{source}/common/unembedded/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test}/assistant',
			f'{source}/common/unembedded/assistant',
			True,
		)
	if utility.check_platform(platform, ['macintosh.arm64']):
		utility.fs_create_link(
			f'{test}/kernel',
			f'{source}/Kernel/.build/macosx/arm64/release/libkernel.dylib',
			False,
		)
		utility.fs_create_link(
			f'{test}/shell',
			f'{source}/Shell/.build/macosx/arm64/release/shell',
			False,
		)
		utility.fs_create_link(
			f'{test}/script',
			f'{source}/Script/dist',
			True,
		)
		utility.fs_create_link(
			f'{test}/launch.sh',
			f'{source}/common/unembedded/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test}/assistant',
			f'{source}/common/unembedded/assistant',
			True,
		)
	if True:
		utility.fs_create_link(
			f'{test}/library',
			f'{local}/library/{platform}',
			True,
		)
		utility.fs_create_directory(
			f'{test}/workspace',
		)
		utility.fs_create_directory(
			f'{test}/temporary',
		)
	print(f'>> TEST >> {test}')
	return

if __name__ == '__main__':
	test(sys.argv[1])
