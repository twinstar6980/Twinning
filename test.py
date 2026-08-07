import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
import common.script.utility as utility

# ----------------

def test(
	platform: str,
) -> None:
	utility.project_ensure_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64'])
	root = utility.project_locate_root()
	module = utility.project_locate_module()
	local = utility.project_locate_local()
	test = utility.project_locate_local('test')
	utility.fs_remove(
		f'{test}',
	)
	utility.fs_create_directory(
		f'{test}',
	)
	if utility.project_check_platform(platform, ['windows.amd64']):
		utility.fs_create_link(
			f'{test}/kernel',
			f'{module}/kernel/.build/windows/x64/release/kernel.dll',
			False,
		)
		utility.fs_create_link(
			f'{test}/script',
			f'{module}/script/build',
			True,
		)
		utility.fs_create_link(
			f'{test}/shell',
			f'{module}/shell/.build/windows/x64/release/shell.exe',
			False,
		)
		utility.fs_create_link(
			f'{test}/assistant',
			f'{root}/common/asset/assistant',
			True,
		)
		utility.fs_create_link(
			f'{test}/launch.sh',
			f'{root}/common/asset/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test}/launch.ps1',
			f'{root}/common/asset/launch.ps1',
			False,
		)
	if utility.project_check_platform(platform, ['linux.amd64']):
		utility.fs_create_link(
			f'{test}/kernel',
			f'{module}/kernel/.build/linux/x86_64/release/libkernel.so',
			False,
		)
		utility.fs_create_link(
			f'{test}/script',
			f'{module}/script/build',
			True,
		)
		utility.fs_create_link(
			f'{test}/shell',
			f'{module}/shell/.build/linux/x86_64/release/shell',
			False,
		)
		utility.fs_create_link(
			f'{test}/assistant',
			f'{root}/common/asset/assistant',
			True,
		)
		utility.fs_create_link(
			f'{test}/launch.sh',
			f'{root}/common/asset/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test}/launch.ps1',
			f'{root}/common/asset/launch.ps1',
			False,
		)
	if utility.project_check_platform(platform, ['macintosh.arm64']):
		utility.fs_create_link(
			f'{test}/kernel',
			f'{module}/kernel/.build/macosx/arm64/release/libkernel.dylib',
			False,
		)
		utility.fs_create_link(
			f'{test}/script',
			f'{module}/script/build',
			True,
		)
		utility.fs_create_link(
			f'{test}/shell',
			f'{module}/shell/.build/macosx/arm64/release/shell',
			False,
		)
		utility.fs_create_link(
			f'{test}/assistant',
			f'{root}/common/asset/assistant',
			True,
		)
		utility.fs_create_link(
			f'{test}/launch.sh',
			f'{root}/common/asset/launch.sh',
			False,
		)
		utility.fs_create_link(
			f'{test}/launch.ps1',
			f'{root}/common/asset/launch.ps1',
			False,
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
