import os
import sys
sys.dont_write_bytecode = True
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from common.python.utility import *

# ----------------

def main(platform: str) -> None:
    if platform not in ['windows.amd64', 'linux.amd64', 'macintosh.arm64', 'android.arm64', 'iphone.arm64']:
        raise RuntimeError(f'invalid platform \'{platform}\'')
    module_directory, module_name = get_project_module(__file__)
    module_distribution_file = get_project_distribution(f'{platform}.{module_name}')
    if platform in ['windows.amd64']:
        module_distribution_file += '.msix'
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'flutter',
            'build',
            'windows',
            '--release',
            '--no-tree-shake-icons',
        ])
        pack_windows_msix(
            'assistant',
            f'{module_directory}/build/windows/x64/runner/Release',
            f'{module_distribution_file}',
        )
        sign_windows_msix(
            f'{module_distribution_file}',
        )
    if platform in ['linux.amd64']:
        module_distribution_file += '.zip'
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'flutter',
            'build',
            'linux',
            '--release',
            '--no-tree-shake-icons',
            '--target-platform', 'linux-x64',
        ], {
            'CXXFLAGS': '-stdlib=libc++',
        })
        pack_zip(
            'assistant',
            f'{module_directory}/build/linux/x64/runner/bundle',
            f'{module_distribution_file}',
        )
    if platform in ['macintosh.arm64']:
        module_distribution_file += '.dmg'
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'flutter',
            'build',
            'macos',
            '--release',
            '--no-tree-shake-icons',
        ])
        pack_macintosh_dmg(
            'Twinning Assistant',
            f'{module_directory}/build/macos/Build/Products/Release/Runner.app',
            f'{module_distribution_file}',
        )
    if platform in ['android.arm64']:
        module_distribution_file += '.apk'
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'flutter',
            'build',
            'apk',
            '--release',
            '--no-tree-shake-icons',
            '--target-platform', 'android-arm64',
            '--split-per-abi',
        ])
        fs_copy(
            f'{module_directory}/build/app/outputs/flutter-apk/app-arm64-v8a-release.apk',
            f'{module_distribution_file}',
        )
        sign_android_apk(
            f'{module_distribution_file}',
        )
    if platform in ['iphone.arm64']:
        module_distribution_file += '.ipa'
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'flutter',
            'build',
            'ios',
            '--release',
            '--no-tree-shake-icons',
        ])
        pack_iphone_ipa(
            'Twinning Assistant',
            f'{module_directory}/build/ios/iphoneos/Runner.app',
            f'{module_distribution_file}',
        )
    print(f'>> DISTRIBUTE >> {module_distribution_file}')

if __name__ == '__main__':
    main(sys.argv[1])
