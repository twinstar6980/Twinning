namespace Twinning.Script.DeveloperHelper {

	// #region archive

	export type ArchiveContentDescriptor = Array<{
		location: StoragePath;
		placement: StoragePath;
	}>;

	// ----------------

	export function archive_list(
		package_file: StoragePath,
	): Array<StoragePath> {
		return ExternalHelper.run_7z_list_content(package_file);
	}

	export function archive_extract(
		package_file: StoragePath,
		content_descriptor: ArchiveContentDescriptor,
	): void {
		return ExternalHelper.run_7z_extract_content(package_file, content_descriptor);
	}

	export function archive_modify(
		package_file: StoragePath,
		content_descriptor: ArchiveContentDescriptor,
	): void {
		return ExternalHelper.run_7z_modify_content(package_file, content_descriptor);
	}

	// ----------------

	export function archive_pack(
		package_file: StoragePath,
		content_directory: StoragePath,
		root_directory: StoragePath,
	): void {
		archive_modify(package_file, [{
			location: root_directory,
			placement: content_directory,
		}]);
		return;
	}

	export function archive_unpack(
		package_file: StoragePath,
		content_directory: StoragePath,
		root_directory: StoragePath,
	): void {
		archive_extract(package_file, [{
			location: root_directory,
			placement: content_directory,
		}]);
		return;
	}

	// #endregion

	// #region keystore

	export type Keystore = [StoragePath, string];

	// ----------------

	export function keystore_generate(
		target_file: StoragePath,
		password: string,
		subject: string,
		time: [Date, Date],
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_key_file = temporary_directory.join(`key.key`);
		let temporary_certificate_file = temporary_directory.join(`certificate.crt`);
		ExternalHelper.run_openssl_generate_key(
			temporary_key_file,
		);
		ExternalHelper.run_openssl_generate_certificate(
			temporary_certificate_file,
			temporary_key_file,
			subject,
			time,
		);
		ExternalHelper.run_openssl_generate_keystore(
			target_file,
			temporary_key_file,
			temporary_certificate_file,
			password,
		);
		return;
	}

	export function keystore_query(
		target_file: StoragePath,
		password: string,
	): {subject: string} {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_certificate_file = temporary_directory.join(`certificate.crt`);
		ExternalHelper.run_openssl_export_certificate(
			target_file,
			temporary_certificate_file,
			password,
		);
		let result = ExternalHelper.run_openssl_query_certificate(
			temporary_certificate_file,
		);
		return result;
	}

	// #endregion

	// #region windows

	const WindowsSignatureTypeX = ['pe', 'msix'] as const;

	export type WindowsSignatureType = typeof WindowsSignatureTypeX[number];

	export const WindowsSignatureTypeE = [...WindowsSignatureTypeX];

	const WindowsPackageTypeX = ['msix'] as const;

	export type WindowsPackageType = typeof WindowsPackageTypeX[number];

	export const WindowsPackageTypeE = [...WindowsPackageTypeX];

	// ----------------

	export function windows_sign_program(
		target: StoragePath,
		keystore: Keystore,
		type: WindowsSignatureType,
	): void {
		let keystore_information = keystore_query(keystore[0], keystore[1]);
		if (type === 'pe') {
			ExternalHelper.run_signtool_sign(
				target,
				'exe',
				keystore[0],
				keystore[1],
			);
		}
		if (type === 'msix') {
			let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
			using temporary_directory_using = temporary_directory_finalizer;
			let temporary_target = temporary_directory.join(`target`);
			let temporary_package_directory = temporary_directory.join(`package`);
			StorageHelper.copy(target, temporary_target, true);
			ExternalHelper.run_makeappx_unpack(
				temporary_target,
				temporary_package_directory,
			);
			let temporary_manifest_file = temporary_package_directory.join(`AppxManifest.xml`);
			let manifest_content = StorageHelper.read_file_text(temporary_manifest_file);
			manifest_content = manifest_content.replace(
				/(<Identity\s*.*\s*Publisher\s*=\s*")([^"]*)("\s.*\/>)/m,
				`$1${keystore_information.subject.replaceAll('$', '$$')}$3`,
			);
			StorageHelper.write_file_text(temporary_manifest_file, manifest_content);
			StorageHelper.remove(temporary_target);
			ExternalHelper.run_makeappx_pack(
				temporary_target,
				temporary_package_directory,
			);
			ExternalHelper.run_signtool_sign(
				temporary_target,
				'msix',
				keystore[0],
				keystore[1],
			);
			StorageHelper.remove(target);
			StorageHelper.copy(temporary_target, target, false);
		}
		return;
	}

	// ----------------

	export function windows_pack_program(
		package_file: StoragePath,
		content_directory: StoragePath,
		type: WindowsPackageType,
	): void {
		if (type === 'msix') {
			ExternalHelper.run_makeappx_pack(
				package_file,
				content_directory,
			);
		}
		return;
	}

	// ----------------

	export function windows_import_manifest(
		executable_file: StoragePath,
		manifest_file: StoragePath,
		resource_identifier: bigint,
	): void {
		ExternalHelper.run_mt_import(
			executable_file,
			manifest_file,
			resource_identifier,
		);
		return;
	}

	export function windows_export_manifest(
		executable_file: StoragePath,
		manifest_file: StoragePath,
		resource_identifier: bigint,
	): void {
		ExternalHelper.run_mt_export(
			executable_file,
			manifest_file,
			resource_identifier,
		);
		return;
	}

	// ----------------

	export function windows_create_pri_resource(
		package_directory: StoragePath,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_configure_file = temporary_directory.join(`priconfig.xml`);
		ExternalHelper.run_makepri_create_configure(
			temporary_configure_file,
		);
		ExternalHelper.run_makepri_create_resource(
			package_directory,
			temporary_configure_file,
		);
		return;
	}

	// #endregion

	// #region linux

	const LinuxSignatureTypeX = ['elf'] as const;

	export type LinuxSignatureType = typeof LinuxSignatureTypeX[number];

	export const LinuxSignatureTypeE = [...LinuxSignatureTypeX];

	const LinuxPackageTypeX = ['appimage'] as const;

	export type LinuxPackageType = typeof LinuxPackageTypeX[number];

	export const LinuxPackageTypeE = [...LinuxPackageTypeX];

	// ----------------

	export function linux_sign_program(
		target: StoragePath,
		keystore: Keystore,
		type: LinuxSignatureType,
	): void {
		let keystore_information = keystore_query(keystore[0], keystore[1]);
		if (type === 'elf') {
			throw new UnsupportedException();
		}
		return;
	}

	// ----------------

	export function linux_pack_program(
		package_file: StoragePath,
		content_directory: StoragePath,
		type: LinuxPackageType,
	): void {
		if (type === 'appimage') {
			ExternalHelper.run_appimagetool_pack(
				package_file,
				content_directory,
			);
		}
		return;
	}

	// #endregion

	// #region macintosh

	const MacintoshSignatureTypeX = ['macho', 'app'] as const;

	export type MacintoshSignatureType = typeof MacintoshSignatureTypeX[number];

	export const MacintoshSignatureTypeE = [...MacintoshSignatureTypeX];

	const MacintoshPackageTypeX = ['dmg'] as const;

	export type MacintoshPackageType = typeof MacintoshPackageTypeX[number];

	export const MacintoshPackageTypeE = [...MacintoshPackageTypeX];

	// ----------------

	export function macintosh_sign_program(
		target: StoragePath,
		keystore: Keystore,
		type: MacintoshSignatureType,
	): void {
		// TODO
		let keystore_information = keystore_query(keystore[0], keystore[1]);
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_keychain_file = temporary_directory.join(`temporary.keychain`);
		let temporary_keychain_password = `temporary`;
		{
			ExternalHelper.run_security_create_keychain(
				temporary_keychain_file,
				temporary_keychain_password,
			);
		}
		using temporary_keychain_finalizer = new Finalizer(() => {
			ExternalHelper.run_security_delete_keychain(
				temporary_keychain_file,
			);
		});
		ExternalHelper.run_security_import(
			temporary_keychain_file,
			keystore[0],
			keystore[1],
		);
		ExternalHelper.run_security_unlock_keychain(
			temporary_keychain_file,
			temporary_keychain_password,
		);
		ExternalHelper.run_security_set_key_partition_list(
			temporary_keychain_file,
			temporary_keychain_password,
		);
		let current_keychain_file_list = ExternalHelper.run_security_list_keychain(
			null,
		);
		ExternalHelper.run_security_list_keychain(
			[...current_keychain_file_list, temporary_keychain_file],
		);
		let keystore_name = ExternalHelper.run_security_find_identity(
			temporary_keychain_file,
		);
		let default_entitlement_file = temporary_directory.join('default.entitlements');
		StorageHelper.write_file_text(default_entitlement_file, `<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"><plist version="1.0"><dict></dict></plist>`);
		let sign_list = [] as Array<StoragePath>;
		let temporary_binary_file = temporary_directory.join('binary');
		StorageHelper.copy(target, temporary_binary_file, true);
		if (type === 'macho') {
			sign_list.push(target);
		}
		if (type === 'app') {
			sign_list.push(...StorageHelper.list_directory(temporary_binary_file.join('Contents').join('Frameworks'), 1n, false, false, false, true).filter((it) => it.extension()?.toLowerCase() === 'framework'));
			sign_list.push(...StorageHelper.list_directory(temporary_binary_file.join('Contents').join('PlugIns'), 1n, false, false, false, true).filter((it) => it.extension()?.toLowerCase() === 'appex'));
			sign_list.push(...StorageHelper.list_directory(temporary_binary_file.join('Frameworks'), 1n, false, false, false, true).filter((it) => it.extension()?.toLowerCase() === 'framework'));
			sign_list.push(...StorageHelper.list_directory(temporary_binary_file.join('PlugIns'), 1n, false, false, false, true).filter((it) => it.extension()?.toLowerCase() === 'appex'));
			sign_list.push(target);
		}
		for (let item_file of sign_list) {
			let item_entitlement_file = temporary_directory.join(`original.${item_file.emit_posix().replaceAll('/', '_')}.entitlements`);
			ExternalHelper.run_codesign_export_entitlement(
				item_file,
				item_entitlement_file,
			);
			if (StorageHelper.exist_file(item_entitlement_file)) {
				item_entitlement_file = default_entitlement_file;
			}
			ExternalHelper.run_codesign_sign(
				item_file,
				item_entitlement_file,
				keystore_name,
				temporary_keychain_file,
			);
		}
		return;
	}

	// ----------------

	export function macintosh_pack_program(
		package_file: StoragePath,
		content_directory: StoragePath,
		type: MacintoshPackageType,
		name: string,
	): void {
		if (type === 'dmg') {
			ExternalHelper.run_createdmg_pack(
				package_file,
				content_directory,
				name,
			);
		}
		return;
	}

	// #endregion

	// #region android

	const AndroidSignatureTypeX = ['elf', 'apk', 'apks'] as const;

	export type AndroidSignatureType = typeof AndroidSignatureTypeX[number];

	export const AndroidSignatureTypeE = [...AndroidSignatureTypeX];

	const AndroidPackageTypeX = ['apk', 'apks'] as const;

	export type AndroidPackageType = typeof AndroidPackageTypeX[number];

	export const AndroidPackageTypeE = [...AndroidPackageTypeX];

	// ----------------

	export function android_sign_program(
		target: StoragePath,
		keystore: Keystore,
		type: AndroidSignatureType,
		signature_version: [boolean, boolean, boolean],
	): void {
		let keystore_information = keystore_query(keystore[0], keystore[1]);
		if (type === 'elf') {
			throw new UnsupportedException();
		}
		if (type === 'apk') {
			ExternalHelper.run_apksigner_sign(
				target,
				signature_version,
				keystore[0],
				keystore[1],
			);
		}
		if (type === 'apks') {
			let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
			using temporary_directory_using = temporary_directory_finalizer;
			let content_list: ArchiveContentDescriptor = archive_list(target)
				.filter((it) => it.extension()?.toLowerCase() === 'apk')
				.map((it) => ({
					location: it,
					placement: temporary_directory.join(it.name()!),
				}));
			archive_extract(target, content_list);
			for (let content_item of content_list) {
				ExternalHelper.run_apksigner_sign(
					content_item.placement,
					signature_version,
					keystore[0],
					keystore[1],
				);
			}
			archive_modify(target, content_list);
		}
		return;
	}

	// ----------------

	export function android_pack_program(
		package_file: StoragePath,
		content_directory: StoragePath,
		type: AndroidPackageType,
	): void {
		if (type === 'apk') {
			throw new UnsupportedException();
		}
		if (type === 'apks') {
			throw new UnsupportedException();
		}
		return;
	}

	// ----------------

	export function android_align_package(
		package_file: StoragePath,
		type: AndroidPackageType,
	): void {
		if (type === 'apk') {
			ExternalHelper.run_zipalign_align(
				package_file,
			);
		}
		if (type === 'apks') {
			let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
			using temporary_directory_using = temporary_directory_finalizer;
			let content_list: ArchiveContentDescriptor = archive_list(package_file)
				.filter((it) => it.extension()?.toLowerCase() === 'apk')
				.map((it) => ({
					location: it,
					placement: temporary_directory.join(it.name()!),
				}));
			archive_extract(package_file, content_list);
			for (let content_item of content_list) {
				ExternalHelper.run_zipalign_align(
					content_item.placement,
				);
			}
			archive_modify(package_file, content_list);
		}
		return;
	}

	// #endregion

	// #region iphone

	const IphoneSignatureTypeX = ['macho', 'app'] as const;

	export type IphoneSignatureType = typeof IphoneSignatureTypeX[number];

	export const IphoneSignatureTypeE = [...IphoneSignatureTypeX];

	const IphonePackageTypeX = ['ipa'] as const;

	export type IphonePackageType = typeof IphonePackageTypeX[number];

	export const IphonePackageTypeE = [...IphonePackageTypeX];

	// ----------------

	export function iphone_sign_program(
		target: StoragePath,
		keystore: Keystore,
		type: MacintoshSignatureType,
	): void {
		return iphone_sign_program(target, keystore, type);
	}

	// ----------------

	export function iphone_pack_program(
		package_file: StoragePath,
		content_directory: StoragePath,
		type: IphonePackageType,
		name: string,
	): void {
		if (type === 'ipa') {
			archive_pack(package_file, content_directory, new StoragePath().join('Payload').join(`${name}.app`));
		}
		return;
	}

	// #endregion

}
