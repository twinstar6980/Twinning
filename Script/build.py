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
	if utility.check_platform(platform, ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']):
		utility.sh_execute_command(source, [
			'npm',
			'install',
		])
		utility.sh_execute_command(source, [
			'npm',
			'run',
			'build',
		])
		utility.fs_create_link(
			f'{source}/dist/configuration',
			f'{source}/src/configuration',
			True,
		)
		utility.fs_copy(
			f'{source}/dist',
			f'{temporary}/artifact',
		)
		utility.pack_zip(
			f'{temporary}/artifact',
			f'{temporary}/artifact.zip',
			f'script',
		)
		destination = ('.zip', f'{temporary}/artifact.zip')
	return destination

if __name__ == '__main__':
	utility.build_project_module(__file__, build, sys.argv[1])
