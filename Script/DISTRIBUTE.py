import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from common.python.utility import *

# ----------------

def main(platform: str) -> None:
    if platform not in ['any']:
        raise RuntimeError(f'invalid platform \'{platform}\'')
    module_directory, module_name = get_project_module(__file__)
    module_distribution_file = get_project_distribution(f'{platform}.{module_name}')
    if platform in ['any']:
        module_distribution_file += '.zip'
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'tsc',
        ])
        configuration_list = filter(
            lambda it: it != 'tsconfig.json' and not it.startswith('.build'),
            map(
                lambda it: f'{it.relative_to(module_directory)}',
                list(pathlib.Path(f'{module_directory}').glob('**/*.json')),
            ),
        )
        for configuration_file in configuration_list:
            fs_create_link(
                f'{module_directory}/.build/{configuration_file}',
                f'{module_directory}/{configuration_file}',
                False,
            )
        pack_zip(
            'script',
            f'{module_directory}/.build',
            f'{module_distribution_file}',
        )
    print(f'>> DISTRIBUTE >> {module_distribution_file}')

if __name__ == '__main__':
    main(sys.argv[1])
