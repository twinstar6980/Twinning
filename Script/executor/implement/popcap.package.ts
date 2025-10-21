namespace Twinning.Script.Executor.Implement.PopCap.Package {

	// #region interface

	export function activate(
	): void {
		push_typical_method('popcap.package', [
			typical_method({
				id: 'pack',
				filter: ['directory', /(\.pak\.bundle)$/i],
				argument: [
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory.replace(/(\.pak\.bundle)?$/i, '.pak')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.Package.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_compress_resource_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_size({
						id: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'data_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory + '.pack'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pak\.bundle)?$/i, '.pak')),
					}),
				],
				worker: ({ bundle_directory, data_file, version_number, version_compress_resource_data, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.PopCap.Package.pack_fs(data_file, definition_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data }, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'unpack',
				filter: ['file', /(\.pak)$/i],
				argument: [
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'bundle_directory',
						rule: ['directory', 'output'],
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.pak)?$/i, '.pak.bundle')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.Package.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_compress_resource_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'data_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'bundle_directory',
						rule: 'output',
						checker: null,
						automatic: (argument: { data_file: string; }) => (argument.data_file + '.unpack'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pak)?$/i, '.pak.bundle')),
					}),
				],
				worker: ({ data_file, bundle_directory, version_number, version_compress_resource_data }, temporary: {}) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.PopCap.Package.unpack_fs(data_file, definition_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data });
					return;
				},
			}),
			typical_method({
				id: 'pack_automatic',
				filter: ['directory', /()$/i],
				argument: [
					typical_argument_path({
						id: 'resource_directory',
						rule: ['directory', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'data_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { resource_directory: string; }) => (argument.resource_directory.replace(/()?$/i, '.pak')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.PopCap.Package.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_boolean({
						id: 'version_compress_resource_data',
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: null,
				worker: ({ resource_directory, data_file, version_number, version_compress_resource_data }, temporary: {}) => {
					let [data, data_size] = Support.PopCap.Package.PackAutomatic.pack(resource_directory, version_number as any, version_compress_resource_data);
					KernelX.Storage.write_file(data_file, data.view().sub(Kernel.Size.value(0n), data_size));
					return;
				},
			}),
			typical_method({
				id: 'encrypt',
				filter: ['file', /(\.pak)$/i],
				argument: [
					typical_argument_path({
						id: 'plain_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'cipher_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file.replace(/(\.pak)?$/i, '.cipher.pak')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'key',
						option: null,
						checker: (argument: {}, value) => ((0n < value && value < 256n) ? null : los('executor.implement:*.illegal_key')),
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'plain_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'cipher_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { plain_file: string; }) => (argument.plain_file + '.encrypt'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.pak)?$/i, '.pak')),
					}),
				],
				worker: ({ plain_file, cipher_file, key }, temporary: {}) => {
					KernelX.Tool.Data.Encryption.EXOR.encrypt_fs(plain_file, cipher_file, [key]);
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
