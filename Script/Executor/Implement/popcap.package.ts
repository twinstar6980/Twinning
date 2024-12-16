namespace Twinning.Script.Executor.Implement.popcap.package_ {

	// ------------------------------------------------

	// pack *
	// unpack *
	// pack_automatic
	// encrypt *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'popcap.package', [
			typical_method({
				id: 'pack',
				filter: 'directory',
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
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ bundle_directory, data_file, version_number, version_compress_resource_data, buffer_size }) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.PopCap.Package.pack_fs(data_file, definition_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data }, buffer_size);
					return;
				},
				batch_argument: [
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
				batch_worker: ({ bundle_directory, data_file, version_number, version_compress_resource_data, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
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
				filter: 'file',
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
				worker: ({ data_file, bundle_directory, version_number, version_compress_resource_data }) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.PopCap.Package.unpack_fs(data_file, definition_file, resource_directory, { number: version_number as any, compress_resource_data: version_compress_resource_data });
					return;
				},
				batch_argument: [
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
				batch_worker: null,
			}),
			typical_method({
				id: 'pack_automatic',
				filter: 'directory',
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
				worker: ({ resource_directory, data_file, version_number, version_compress_resource_data }) => {
					let data = Support.PopCap.Package.PackAutomatic.pack(resource_directory, version_number as any, version_compress_resource_data);
					KernelX.Storage.write_file(data_file, data[0].view().sub(Kernel.Size.value(0n), data[1]));
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
			typical_method({
				id: 'encrypt',
				filter: 'file',
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
				],
				worker: ({ plain_file, cipher_file }) => {
					KernelX.Tool.Data.Encryption.EXOR.encrypt_fs(plain_file, cipher_file, [0xF7n]);
					return;
				},
				batch_argument: [
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
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.popcap.package_.injector;