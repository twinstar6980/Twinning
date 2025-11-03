import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
from common.python.utility import *

# ----------------

def main(platform: str) -> None:
    if platform not in ['windows.amd64', 'linux.amd64', 'macintosh.arm64']:
        raise RuntimeError(f'invalid platform \'{platform}\'')
    project_directory = f'{get_project()}'
    test_directory = f'{get_project_local()}/test'
    fs_create_directory(
        f'{test_directory}/workspace',
    )
    fs_create_directory(
        f'{test_directory}/temporary',
    )
    if platform in ['windows.amd64']:
        fs_create_link(
            f'{test_directory}/library',
            f'{project_directory}/Assistant/windows/package/Library',
            True,
        )
        fs_create_link(
            f'{test_directory}/launch.cmd',
            f'{project_directory}/common/unembedded/launch.cmd',
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
    if platform in ['linux.amd64']:
        fs_create_directory(
            f'{test_directory}/library',
        )
        fs_create_link(
            f'{test_directory}/launch.sh',
            f'{project_directory}/common/unembedded/launch.sh',
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
    if platform in ['macintosh.arm64']:
        fs_create_directory(
            f'{test_directory}/library',
        )
        fs_create_link(
            f'{test_directory}/launch.sh',
            f'{project_directory}/common/unembedded/launch.sh',
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
        f'{project_directory}/Script/.build',
        True,
    )
    fs_create_link(
        f'{test_directory}/assistant',
        f'{project_directory}/common/unembedded/assistant',
        True,
    )
    fs_create_link(
        f'{test_directory}/assistant_plus',
        f'{project_directory}/common/unembedded/assistant_plus',
        True,
    )
    print(f'>> TEST >> {test_directory}')

if __name__ == '__main__':
    main(sys.argv[1])
