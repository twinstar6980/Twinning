import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common.script.utility as utility

# ----------------

def main(
	platform: str,
) -> None:
	utility.ensure_platform(platform, ['any.any'])
	module_directory, module_name = utility.get_project_module(__file__)
	module_distribution_file = utility.get_project_distribution(f'{platform}.{module_name}')
	if utility.check_platform(platform, ['any.any']):
		module_distribution_file += '.zip'
		utility.fs_remove(
			f'{module_distribution_file}',
		)
		utility.execute_command(module_directory, [
			'npm',
			'install',
		])
		utility.execute_command(module_directory, [
			'npm',
			'run',
			'build',
		])
		utility.fs_create_link(
			f'{module_directory}/dist/configuration',
			f'{module_directory}/src/configuration',
			False,
		)
		utility.pack_zip(
			'script',
			f'{module_directory}/dist',
			f'{module_distribution_file}',
		)
	print(f'>> BUILD >> {module_distribution_file}')
	return

if __name__ == '__main__':
	main(sys.argv[1])
