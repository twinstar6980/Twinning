import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from common.python.utility import *

# ----------------

def main(
	platform: str,
) -> None:
	ensure_platform(platform, ['any.any'])
	module_directory, module_name = get_project_module(__file__)
	module_distribution_file = get_project_distribution(f'{platform}.{module_name}')
	if check_platform(platform, ['any.any']):
		module_distribution_file += '.zip'
		fs_remove(
			f'{module_distribution_file}',
		)
		execute_command(module_directory, [
			'npm',
			'install',
		])
		execute_command(module_directory, [
			'npm',
			'run',
			'build',
		])
		fs_create_link(
			f'{module_directory}/dist/configuration',
			f'{module_directory}/src/configuration',
			False,
		)
		pack_zip(
			'script',
			f'{module_directory}/dist',
			f'{module_distribution_file}',
		)
	print(f'>> BUILD >> {module_distribution_file}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
