namespace Twinning.Script.Executor.Implementation.Developer.Utility {

	// #region interface

	export function activate(
	): void {
		push_typical_method('developer.utility', [
			typical_method({
				identifier: 'keystore.generate',
				filter: null,
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'subject',
						option: null,
						checker: (argument: {}, value) => (/^(\/[^=\/]+=[^=\/]+)+$/.test(value) ? null : los('executor.implementation:*.invalid_certificate_subject_expression')),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'begin_time',
						option: null,
						checker: (argument: {}, value) => (/^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}$/.test(value) ? null : los('executor.implementation:*.invalid_time_expression')),
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'end_time',
						option: null,
						checker: (argument: {}, value) => (/^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}$/.test(value) ? null : los('executor.implementation:*.invalid_time_expression')),
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({target_file, password, subject, begin_time, end_time}, store: {}) => {
					DeveloperHelper.keystore_generate(target_file, password, subject, [new Date(begin_time), new Date(end_time)]);
					return;
				},
			}),
			typical_method({
				identifier: 'keystore.query',
				filter: ['file', /(\.(p12|pfx))$/i],
				argument: [
					typical_argument_path({
						identifier: 'target_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'result_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {target_file: StoragePath}) => ConvertHelper.replace_path_name(argument.target_file, /()?$/i, '.json'),
						condition: null,
					}),
					typical_argument_string({
						identifier: 'password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'result_file',
						rule: 'input',
						checker: null,
						automatic: (argument: {target_file: StoragePath}) => ConvertHelper.replace_path_name(argument.target_file, /()?$/i, '.result'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, '.json'),
					}),
				],
				worker: ({target_file, result_file, password}, store: {}) => {
					let result = DeveloperHelper.keystore_query(target_file, password);
					JsonHelper.encode_file(result_file, result);
					return;
				},
			}),
			typical_method({
				identifier: 'windows.sign_program',
				filter: ['any', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target',
						rule: ['any', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'keystore_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'keystore_password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.WindowsSignatureTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target, keystore_file, keystore_password, type}, store: {}) => {
					DeveloperHelper.windows_sign_program(target, [keystore_file, keystore_password], type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'windows.pack_program',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'content_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'package_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.WindowsPackageTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({content_directory, package_file, type}, store: {}) => {
					DeveloperHelper.windows_pack_program(package_file, content_directory, type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'windows.import_manifest',
				filter: ['file', /(\.(exe|dll))$/i],
				argument: [
					typical_argument_path({
						identifier: 'executable_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'manifest_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: (argument: {executable_file: StoragePath}) => ConvertHelper.replace_path_name(argument.executable_file, /()?$/i, '.manifest'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'resource_identifier',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({executable_file, manifest_file, resource_identifier}, store: {}) => {
					DeveloperHelper.windows_import_manifest(executable_file, manifest_file, resource_identifier);
					return;
				},
			}),
			typical_method({
				identifier: 'windows.export_manifest',
				filter: ['file', /(\.(exe|dll))$/i],
				argument: [
					typical_argument_path({
						identifier: 'executable_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'manifest_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {executable_file: StoragePath}) => ConvertHelper.replace_path_name(argument.executable_file, /()?$/i, '.manifest'),
						condition: null,
					}),
					typical_argument_integer({
						identifier: 'resource_identifier',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({executable_file, manifest_file, resource_identifier}, store: {}) => {
					DeveloperHelper.windows_export_manifest(executable_file, manifest_file, resource_identifier);
					return;
				},
			}),
			typical_method({
				identifier: 'windows.create_pri_resource',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'package_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({package_directory}, store: {}) => {
					DeveloperHelper.windows_create_pri_resource(package_directory);
					return;
				},
			}),
			typical_method({
				identifier: 'linux.sign_program',
				filter: ['any', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target',
						rule: ['any', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'keystore_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'keystore_password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.LinuxSignatureTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target, keystore_file, keystore_password, type}, store: {}) => {
					DeveloperHelper.linux_sign_program(target, [keystore_file, keystore_password], type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'linux.pack_program',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'content_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'package_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.LinuxPackageTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({content_directory, package_file, type}, store: {}) => {
					DeveloperHelper.linux_pack_program(package_file, content_directory, type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'macintosh.sign_program',
				filter: ['any', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target',
						rule: ['any', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'keystore_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'keystore_password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.MacintoshSignatureTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target, keystore_file, keystore_password, type}, store: {}) => {
					DeveloperHelper.macintosh_sign_program(target, [keystore_file, keystore_password], type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'macintosh.pack_program',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'content_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'package_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.MacintoshPackageTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'name',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({content_directory, package_file, type, name}, store: {}) => {
					DeveloperHelper.macintosh_pack_program(package_file, content_directory, type as any, name);
					return;
				},
			}),
			typical_method({
				identifier: 'android.sign_program',
				filter: ['any', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target',
						rule: ['any', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'keystore_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'keystore_password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.AndroidSignatureTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'signature_version_1',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'signature_version_2',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						identifier: 'signature_version_3',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target, keystore_file, keystore_password, type, signature_version_1, signature_version_2, signature_version_3}, store: {}) => {
					DeveloperHelper.android_sign_program(target, [keystore_file, keystore_password], type as any, [signature_version_1, signature_version_2, signature_version_3]);
					return;
				},
			}),
			typical_method({
				identifier: 'android.pack_program',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'content_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'package_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.AndroidPackageTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({content_directory, package_file, type}, store: {}) => {
					DeveloperHelper.android_pack_program(package_file, content_directory, type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'android.align_package',
				filter: ['file', /(\.(apk|apks))$/i],
				argument: [
					typical_argument_path({
						identifier: 'package_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.AndroidPackageTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'package_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({package_file, type}, store: {}) => {
					DeveloperHelper.android_align_package(package_file, type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'iphone.sign_program',
				filter: ['any', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'target',
						rule: ['any', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'keystore_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'keystore_password',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.IphoneSignatureTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'target',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
				],
				worker: ({target, keystore_file, keystore_password, type}, store: {}) => {
					DeveloperHelper.iphone_sign_program(target, [keystore_file, keystore_password], type as any);
					return;
				},
			}),
			typical_method({
				identifier: 'iphone.pack_program',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'content_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'package_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'type',
						option: DeveloperHelper.IphonePackageTypeE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_string({
						identifier: 'name',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({content_directory, package_file, type, name}, store: {}) => {
					DeveloperHelper.iphone_pack_program(package_file, content_directory, type as any, name);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
