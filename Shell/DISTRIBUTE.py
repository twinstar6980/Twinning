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
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'xmake',
            'config',
            '--builddir=.build',
            '--mode=release',
            '--plat=windows',
            '--arch=x64',
            '--toolchain=mingw[clang]',
            '--runtimes=c++_shared',
        ])
        execute_command(module_directory, [
            'xmake',
            'build',
        ])
        fs_copy(
            f'{module_directory}/.build/windows/x64/release/shell.exe',
            f'{module_distribution_file}',
        )
        strip_windows_binary(
            f'{module_distribution_file}',
        )
        apply_windows_manifest(
            f'{module_distribution_file}',
            f'{module_directory}/shell/resource/windows/application.manifest',
        )
    if platform in ['linux.amd64']:
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'xmake',
            'config',
            '--builddir=.build',
            '--mode=release',
            '--plat=linux',
            '--arch=x86_64',
            '--toolchain=clang',
            '--runtimes=c++_shared',
        ])
        execute_command(module_directory, [
            'xmake',
            'build',
        ])
        fs_copy(
            f'{module_directory}/.build/linux/x86_64/release/shell',
            f'{module_distribution_file}',
        )
    if platform in ['macintosh.arm64']:
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'xmake',
            'config',
            '--builddir=.build',
            '--mode=release',
            '--plat=macosx',
            '--arch=arm64',
            '--toolchain=xcode',
            '--target_minver=11.5',
            '--cc=clang-20',
            '--cxx=clang-20',
        ])
        execute_command(module_directory, [
            'xmake',
            'build',
        ])
        fs_copy(
            f'{module_directory}/.build/macosx/arm64/release/shell',
            f'{module_distribution_file}',
        )
    if platform in ['android.arm64']:
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'xmake',
            'config',
            '--builddir=.build',
            '--mode=release',
            '--plat=android',
            '--arch=arm64-v8a',
            '--toolchain=ndk',
            '--ndk_sdkver=30',
            '--runtimes=c++_shared',
        ])
        execute_command(module_directory, [
            'xmake',
            'build',
        ])
        fs_copy(
            f'{module_directory}/.build/android/arm64-v8a/release/shell',
            f'{module_distribution_file}',
        )
    if platform in ['iphone.arm64']:
        fs_remove(
            f'{module_distribution_file}',
        )
        execute_command(module_directory, [
            'xmake',
            'config',
            '--builddir=.build',
            '--mode=release',
            '--plat=iphoneos',
            '--arch=arm64',
            '--toolchain=xcode',
            '--target_minver=15.6',
            '--cc=clang-20',
            '--cxx=clang-20',
        ])
        execute_command(module_directory, [
            'xmake',
            'build',
        ])
        fs_copy(
            f'{module_directory}/.build/iphoneos/arm64/release/shell',
            f'{module_distribution_file}',
        )
        sign_iphone_binary(
            f'{module_distribution_file}',
        )
    print(f'>> DISTRIBUTE >> {module_distribution_file}')

if __name__ == '__main__':
    main(sys.argv[1])
