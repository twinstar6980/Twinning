import os
import sys
import re
import contextlib
import pathlib
import glob
import shutil
import tempfile
import subprocess
import json
import typing

# ----------------

def rs_create_finalizer_scope(
) -> contextlib.ExitStack:
	return contextlib.ExitStack()

# ----------------

def fs_path_parent(
	target: str,
) -> str:
	return pathlib.Path(target).parent.__str__()

def fs_path_name(
	target: str,
) -> str:
	return pathlib.Path(target).name

def fs_exist(
	target: str,
) -> bool:
	return pathlib.Path(target).exists(follow_symlinks=False)

def fs_exist_link(
	target: str,
) -> bool:
	return pathlib.Path(target).is_symlink(follow_symlinks=False)

def fs_exist_file(
	target: str,
) -> bool:
	return pathlib.Path(target).is_file(follow_symlinks=False)

def fs_exist_directory(
	target: str,
) -> bool:
	return pathlib.Path(target).is_dir(follow_symlinks=False)

def fs_copy(
	target: str,
	placement: str,
	follow_link: bool = False,
) -> None:
	if not fs_exist(target):
		raise RuntimeError(f'invalid target \'{target}\'')
	if fs_exist(placement):
		fs_remove(placement)
	placement_parent = fs_path_parent(placement)
	if not fs_exist_directory(placement_parent):
		fs_create_directory(placement_parent)
	if pathlib.Path(target).is_file():
		shutil.copy(target, placement, follow_symlinks=follow_link)
	if pathlib.Path(target).is_dir():
		shutil.copytree(target, placement, symlinks=not follow_link, dirs_exist_ok=True)
	return

def fs_remove(
	target: str,
) -> None:
	if pathlib.Path(target).is_symlink():
		os.remove(target)
	if pathlib.Path(target).is_file():
		os.remove(target)
	if pathlib.Path(target).is_dir():
		shutil.rmtree(target)
	return

def fs_create_link(
	target: str,
	referent: str,
	is_directory: bool,
) -> None:
	if fs_exist(target):
		fs_remove(target)
	os.symlink(referent, target, is_directory)
	return

def fs_read_file(
	target: str,
) -> str:
	return pathlib.Path(target).read_text('utf-8')

def fs_write_file(
	target: str,
	data: str,
) -> None:
	pathlib.Path(target).write_text(data, encoding='utf-8')
	return

def fs_create_directory(
	target: str,
) -> None:
	os.makedirs(target, exist_ok=True)
	return

def fs_find(
	pattern: str,
) -> list[str]:
	return sorted(glob.glob(pattern))

def fs_temporary(
) -> tempfile.TemporaryDirectory[str]:
	parent = project_locate_temporary()
	if not fs_exist_directory(parent):
		fs_create_directory(parent)
	return tempfile.TemporaryDirectory(dir=parent)

# ----------------

def sh_execute_command(
	location: str,
	command: list[str],
	environment: dict[str, str] = {},
	ensure_ok: bool = True,
	want_output: bool = False,
) -> tuple[int, str, str]:
	actual_environment = os.environ.copy()
	for environment_name, environment_value in environment.items():
		actual_environment[environment_name] = environment_value
	result = subprocess.run(
		command,
		shell=sys.platform == 'win32',
		cwd=location,
		env=actual_environment,
		check=ensure_ok,
		capture_output=want_output,
		text=True,
		encoding='utf-8',
	)
	return (result.returncode, result.stdout, result.stderr)

# ----------------

def ex_keystore_verify(
	target: str,
	password: str,
) -> bool:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		openssl_result, _, _ = sh_execute_command(temporary, [
			'openssl',
			'pkcs12',
			'-in', f'{target}',
			'-passin', f'pass:{password}',
			'-noout',
		], ensure_ok=False)
	return openssl_result == 0

def ex_keystore_query(
	target: str,
	password: str,
) -> tuple[str]:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		sh_execute_command(temporary, [
			'openssl',
			'pkcs12',
			'-in', f'{target}',
			'-passin', f'pass:{password}',
			'-out', f'{temporary}/file.crt',
			'-nokeys',
		])
		sh_execute_command(temporary, [
			'openssl',
			'x509',
			'-in', f'{temporary}/file.crt',
			'-out', f'{temporary}/result.txt',
			'-noout',
			'-subject',
		])
		result_content = fs_read_file(
			f'{temporary}/result.txt',
		)
		result_subject = re.search(
			r'^subject=(.*)$',
			result_content,
			re.MULTILINE,
		)
		if result_subject == None:
			raise RuntimeError(f'unable to get subject')
	return (result_subject.group(1),)

# ----------------

def ex_archive_pack_zip(
	package: str,
	content: str,
	root: str | None,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{content}',
			f'{temporary}/content{'' if root == None else f'/{root}'}',
		)
		shutil.make_archive(
			f'{temporary}/package',
			'zip',
			f'{temporary}/content',
			f'{'.' if root == None else f'{root}'}',
		)
		fs_copy(
			f'{temporary}/package.zip',
			f'{package}',
		)
	return

def ex_archive_unpack_zip(
	package: str,
	content: str,
	root: str | None,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{package}',
			f'{temporary}/package.zip',
		)
		shutil.unpack_archive(
			f'{temporary}/package.zip',
			f'{temporary}/content',
			'zip',
		)
		fs_copy(
			f'{temporary}/content{'' if root == None else f'/{root}'}',
			f'{content}',
		)
	return

# ----------------

def ex_windows_sign(
	target: str,
	keystore: tuple[str, str] | None,
	type: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		if keystore == None:
			return
		keystore_file, keystore_password = keystore
		fs_copy(
			f'{target}',
			f'{temporary}/target.{type}',
		)
		if type == 'msix':
			keystore_subject = ex_keystore_query(keystore_file, keystore_password)[0]
			ex_windows_unpack_msix(
				f'{temporary}/target.{type}',
				f'{temporary}/content',
			)
			manifest_content = fs_read_file(
				f'{temporary}/content/AppxManifest.xml',
			)
			manifest_content = re.sub(
				r'(<Identity\s.*Publisher\s*=\s*")([^"]*)("\s.*/>)',
				rf'\1{keystore_subject.replace('\\', '\\\\')}\3',
				manifest_content,
				re.RegexFlag.MULTILINE,
			)
			fs_write_file(
				f'{temporary}/content/AppxManifest.xml',
				manifest_content,
			)
			ex_windows_pack_msix(
				f'{temporary}/target.{type}',
				f'{temporary}/content',
			)
		sh_execute_command(temporary, [
			'signtool',
			'sign',
			'/fd', f'SHA256',
			'/f', f'{keystore_file}',
			'/p', f'{keystore_password}',
			f'{temporary}/target.{type}',
		])
		fs_copy(
			f'{temporary}/target.{type}',
			f'{target}',
		)
	return

def ex_windows_pack_msix(
	package: str,
	content: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{content}',
			f'{temporary}/content',
		)
		sh_execute_command(temporary, [
			'makeappx',
			'pack',
			'/o',
			'/h', f'SHA256',
			'/d', f'{temporary}/content',
			'/p', f'{temporary}/package.msix',
		])
		fs_copy(
			f'{temporary}/package.msix',
			f'{package}',
		)
	return

def ex_windows_unpack_msix(
	package: str,
	content: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{package}',
			f'{temporary}/package.msix',
		)
		sh_execute_command(temporary, [
			'makeappx',
			'unpack',
			'/o',
			'/p', f'{temporary}/package.msix',
			'/d', f'{temporary}/content',
		])
		fs_copy(
			f'{temporary}/content',
			f'{content}',
		)
	return

def ex_windows_strip_executable(
	executable: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{executable}',
			f'{temporary}/executable.bin',
		)
		sh_execute_command(temporary, [
			'llvm-strip',
			'--strip-all',
			f'{temporary}/executable.bin',
		])
		fs_copy(
			f'{temporary}/executable.bin',
			f'{executable}',
		)
	return

def ex_windows_import_manifest(
	executable: str,
	manifest: str,
	resource_identifier: int,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{executable}',
			f'{temporary}/executable.bin',
		)
		fs_copy(
			f'{manifest}',
			f'{temporary}/manifest.manifest',
		)
		sh_execute_command(temporary, [
			'mt',
			'-manifest', f'{temporary}/manifest.manifest',
			f'-outputresource:{temporary}/executable.bin;#{resource_identifier}',
			'-verbose',
		])
		fs_copy(
			f'{temporary}/executable.bin',
			f'{executable}',
		)
	return

def ex_windows_create_pri_resource(
	package: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		sh_execute_command(temporary, [
			'makepri',
			'createconfig',
			'/o',
			'/cf', f'{temporary}/priconfig.xml',
			'/dq', f'en-US',
		])
		sh_execute_command(temporary, [
			'makepri',
			'new',
			'/o',
			'/cf', f'{temporary}/priconfig.xml',
			'/pr', f'{package}',
			'/of', f'{package}/resources.pri',
		])
	return

# ----------------

def ex_linux_pack_appimage(
	package: str,
	content: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{content}',
			f'{temporary}/content.AppDir',
		)
		sh_execute_command(temporary, [
			'appimagetool',
			'--no-appstream',
			f'{temporary}/content.AppDir',
			f'{temporary}/package.AppImage',
		])
		fs_copy(
			f'{temporary}/package.AppImage',
			f'{package}',
		)
	return

# ----------------

def ex_macintosh_sign(
	target: str,
	keystore: tuple[str, str] | None,
	type: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		keystore_name = '-'
		keychain_file = f'{temporary}/temporary.keychain'
		keychain_password = 'temporary'
		if keystore != None:
			keystore_file, keystore_password = keystore
			sh_execute_command(temporary, [
				'security',
				'create-keychain',
				'-p', f'{keychain_password}',
				f'{keychain_file}',
			])
			finalizer.callback(lambda:
				sh_execute_command(temporary, [
					'security',
					'delete-keychain',
					f'{keychain_file}',
				]
			))
			sh_execute_command(temporary, [
				'security',
				'import',
				f'{keystore_file}',
				'-k', f'{keychain_file}',
				'-P', f'{keystore_password}',
				'-T', f'/usr/bin/codesign',
			])
			sh_execute_command(temporary, [
				'security',
				'unlock-keychain',
				'-p', f'{keychain_password}',
				f'{keychain_file}',
			])
			sh_execute_command(temporary, [
				'security',
				'set-key-partition-list',
				'-S', f'apple-tool:,apple:',
				'-s',
				'-k', f'{keychain_password}',
				f'{keychain_file}',
			])
			_, list_keychains_output, _ = sh_execute_command(temporary, [
				'security',
				'list-keychains',
			], want_output=True)
			sh_execute_command(temporary, [
				'security',
				'list-keychains',
				'-s', *[item.strip(' "') for item in list_keychains_output.splitlines()], keychain_file,
			])
			_, find_identity_output, _ = sh_execute_command(temporary, [
				'security',
				'find-identity',
				f'{keychain_file}',
			], want_output=True)
			find_identity_match = re.search(r" ([0-9A-F]{40}) ", find_identity_output, flags=re.RegexFlag.MULTILINE)
			if find_identity_match == None:
				raise RuntimeError(f'could not import keystore')
			keystore_name = find_identity_match.group(1)
		default_entitlement_file = f'{temporary}/default.entitlements'
		fs_write_file(
			default_entitlement_file,
			'<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"><plist version="1.0"><dict></dict></plist>',
		)
		fs_copy(
			f'{target}',
			f'{temporary}/target',
		)
		content_list: list[str] = []
		if type == 'macho':
			content_list += [f'{temporary}/target']
		if type == 'app':
			content_list += fs_find(f'{temporary}/target/Contents/Frameworks/*.framework')
			content_list += fs_find(f'{temporary}/target/Contents/PlugIns/*.appex')
			content_list += fs_find(f'{temporary}/target/Frameworks/*.framework')
			content_list += fs_find(f'{temporary}/target/PlugIns/*.appex')
			content_list += [f'{temporary}/target']
		for content_item in content_list:
			actual_entitlement_file = f'{temporary}/original.{str(pathlib.Path(content_item).relative_to(f'{temporary}/target')).replace('/', '_')}.entitlements'
			sh_execute_command(temporary, [
				'codesign',
				'-d',
				'--entitlements', f':{actual_entitlement_file}',
				f'{content_item}',
			], ensure_ok=False)
			if not fs_exist_file(actual_entitlement_file):
				actual_entitlement_file = default_entitlement_file
			sh_execute_command(temporary, [
				'codesign',
				'-s', f'{keystore_name}',
				'--keychain', f'{keychain_file}',
				'--entitlements', f'{actual_entitlement_file}',
				'--force',
				f'{content_item}',
			])
		fs_copy(
			f'{temporary}/target',
			f'{target}',
		)
	return

def ex_macintosh_pack_dmg(
	package: str,
	content: str,
	name: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		fs_copy(
			f'{content}',
			f'{temporary}/content/{name}.app',
		)
		sh_execute_command(temporary, [
			'create-dmg',
			'--volname', f'{name}',
			'--window-pos', f'0', f'0',
			'--window-size', f'500', f'300',
			'--text-size', f'16',
			'--icon-size', f'128',
			'--icon', f'{name}.app', f'150', f'100',
			'--app-drop-link', f'350', f'100',
			'--format', f'UDZO',
			'--filesystem', f'APFS',
			f'{temporary}/package.dmg',
			f'{temporary}/content/{name}.app',
		])
		fs_copy(
			f'{temporary}/package.dmg',
			f'{package}',
		)
	return

# ----------------

def ex_android_sign(
	target: str,
	keystore: tuple[str, str] | None,
	type: str,
	signature_version: tuple[bool, bool, bool],
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		if keystore == None:
			return
		keystore_file, keystore_password = keystore
		fs_copy(
			f'{target}',
			f'{temporary}/target.apk',
		)
		sh_execute_command(temporary, [
			'apksigner',
			'sign',
			'--v1-signing-enabled', f'{str(signature_version[0]).lower()}',
			'--v2-signing-enabled', f'{str(signature_version[1]).lower()}',
			'--v3-signing-enabled', f'{str(signature_version[2]).lower()}',
			'--v4-signing-enabled', f'false',
			'--ks', f'{keystore_file}',
			'--ks-pass', f'pass:{keystore_password}',
			f'{temporary}/target.apk',
		])
		fs_copy(
			f'{temporary}/target.apk',
			f'{target}',
		)
	return

# ----------------

def ex_iphone_sign(
	target: str,
	keystore: tuple[str, str] | None,
	type: str,
) -> None:
	return ex_macintosh_sign(target, keystore, type)

def ex_iphone_pack_ipa(
	package: str,
	content: str,
	name: str,
) -> None:
	return ex_archive_pack_zip(package, content, f'Payload/{name}.app')

# ----------------

def project_check_platform(
	value: str,
	expect: list[str],
) -> bool:
	return value in expect

def project_ensure_platform(
	value: str,
	expect: list[str],
) -> None:
	if not project_check_platform(value, expect):
		raise RuntimeError(f'unsupported platform \'{value}\'')
	return

# ----------------

def project_locate_root(
	name: str | None = None,
) -> str:
	return f'{pathlib.Path(__file__).absolute().parent.parent.parent.as_posix()}{'' if name is None else f'/{name}'}'

def project_locate_module(
	name: str | None = None,
) -> str:
	return f'{project_locate_root('module')}{'' if name is None else f'/{name}'}'

def project_locate_local(
	name: str | None = None,
) -> str:
	return f'{project_locate_root('.local')}{'' if name is None else f'/{name}'}'

def project_locate_temporary(
	name: str | None = None,
) -> str:
	return f'{project_locate_local('temporary')}{'' if name is None else f'/{name}'}'

def project_locate_distribution(
	name: str | None = None,
) -> str:
	return f'{project_locate_local('distribution')}{'' if name is None else f'/{name}'}'

def project_locate_keystore(
) -> tuple[str, str] | None:
	file = f'{project_locate_local('keystore')}/file.p12'
	if not pathlib.Path(file).is_file():
		return None
	password = fs_read_file(f'{project_locate_local('keystore')}/password.txt')
	if not ex_keystore_verify(file, password):
		raise RuntimeError('invaild keystore password')
	return (file, password)

# ----------------

def project_fetch_cpp_dependency(
	location: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		print(f'fetch \'{fs_path_name(location)}\' ...')
		origin_directory = f'{location}/origin'
		manifest_file = f'{location}/manifest.json'
		patch_file = f'{location}/patch.diff'
		if fs_exist_directory(origin_directory):
			print(f'pass')
			return
		if not fs_exist_file(manifest_file):
			raise RuntimeError(f'could not found package manifest')
		if not fs_exist_file(patch_file):
			raise RuntimeError(f'could not found package patch')
		manifest = json.loads(fs_read_file(manifest_file))
		repository = manifest.get('repository')
		reference = manifest.get('reference')
		if repository == None:
			raise RuntimeError(f'invalid package repository')
		if reference == None:
			raise RuntimeError(f'invalid package manifest')
		sh_execute_command(temporary, [
			'git',
			'clone',
			f'{repository}',
			'.',
		])
		sh_execute_command(temporary, [
			'git',
			'checkout',
			f'{reference}',
		])
		sh_execute_command(temporary, [
			'git',
			'apply',
			'--allow-empty',
			f'{patch_file}',
		])
		fs_copy(temporary, origin_directory)
		print(f'done')
	return

def project_fetch_cpp_dependency_directory(
	location: str,
) -> None:
	for item in fs_find(f'{location}/*'):
		if not fs_exist_directory(item):
			continue
		project_fetch_cpp_dependency(item)
	return

# ----------------

def project_setup_library(
	platform: str,
) -> None:
	library_directory = None
	library_file_list = None
	if project_check_platform(platform, ['windows.amd64']):
		clang_file = shutil.which('clang')
		if clang_file == None:
			raise RuntimeError(f'could not found clang path')
		library_directory_list = fs_find(str(pathlib.Path(clang_file).parent.parent / 'x86_64-w64-mingw32/bin'))
		if len(library_directory_list) == 0:
			raise RuntimeError(f'could not found library directory')
		library_directory = library_directory_list[0]
		library_file_list = ['libc++.dll', 'libunwind.dll']
	if project_check_platform(platform, ['android.arm64']):
		ndk_home = os.environ.get('ANDROID_NDK_HOME')
		if ndk_home == None:
			raise RuntimeError(f'could not found ndk path')
		library_directory_list = fs_find(str(pathlib.Path(ndk_home) / 'toolchains/llvm/prebuilt/*/sysroot/usr/lib/aarch64-linux-android'))
		if len(library_directory_list) == 0:
			raise RuntimeError(f'could not found library directory')
		library_directory = library_directory_list[0]
		library_file_list = ['libc++_shared.so']
	destination = f'{project_locate_local('library')}/{platform}'
	fs_create_directory(
		destination,
	)
	if library_directory != None and library_file_list != None:
		for library_file in library_file_list:
			fs_copy(
				f'{library_directory}/{library_file}',
				f'{destination}/{library_file}',
			)
	return

def project_build_module(
	path: str,
	builder: typing.Callable[[str, tuple[str, str] | None, str, str], tuple[str, str] | None],
	platform: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		name = fs_path_name(fs_path_parent(path))
		distribution = builder(
			project_locate_module(name),
			project_locate_keystore(),
			temporary,
			platform,
		)
		if distribution == None:
			raise RuntimeError(f'unsupported platform \'{platform}\'')
		distribution_file = project_locate_distribution(f'{platform}.{name}{distribution[0]}')
		fs_copy(
			distribution[1],
			distribution_file,
		)
		print(f'>> BUILD MODULE >> {distribution_file}')
	return

def project_build_bundle(
	path: str,
	builder: typing.Callable[[str, str, str, tuple[str, str] | None, str, str], tuple[str, str] | None],
	platform: str,
) -> None:
	with rs_create_finalizer_scope() as finalizer:
		temporary = finalizer.enter_context(fs_temporary())
		distribution = builder(
			project_locate_root(),
			project_locate_local(),
			project_locate_distribution(),
			project_locate_keystore(),
			temporary,
			platform,
		)
		if distribution == None:
			raise RuntimeError(f'unsupported platform \'{platform}\'')
		distribution_file = project_locate_distribution(f'{platform}.bundle{distribution[0]}')
		fs_copy(
			distribution[1],
			distribution_file,
		)
		print(f'>> BUILD BUNDLE >> {distribution_file}')
	return
