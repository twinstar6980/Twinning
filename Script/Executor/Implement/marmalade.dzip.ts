namespace Twinning.Script.Executor.Implement.marmalade.dzip {

	// ------------------------------------------------

	// pack *
	// unpack *
	// pack_automatic

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'marmalade.dzip', [
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
						automatic: (argument: { bundle_directory: string; }) => (argument.bundle_directory.replace(/(\.dz\.bundle)?$/i, '.dz')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.Marmalade.DZip.VersionNumberE,
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
				worker: ({ bundle_directory, data_file, version_number, buffer_size }) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.Marmalade.DZip.pack_fs(data_file, definition_file, resource_directory, { number: version_number as any }, buffer_size);
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.dz\.bundle)?$/i, '.dz')),
					}),
				],
				batch_worker: ({ bundle_directory, data_file, version_number, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.Marmalade.DZip.pack_fs(data_file, definition_file, resource_directory, { number: version_number as any }, temporary.buffer.view());
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
						automatic: (argument: { data_file: string; }) => (argument.data_file.replace(/(\.dz)?$/i, '.dz.bundle')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.Marmalade.DZip.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ data_file, bundle_directory, version_number }) => {
					let definition_file = `${bundle_directory}/definition.json`;
					let resource_directory = `${bundle_directory}/resource`;
					KernelX.Tool.Marmalade.DZip.unpack_fs(data_file, definition_file, resource_directory, { number: version_number as any });
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.dz)?$/i, '.dz.bundle')),
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
						automatic: (argument: { resource_directory: string; }) => (argument.resource_directory.replace(/()?$/i, '.dz')),
						condition: null,
					}),
					typical_argument_integer({
						id: 'version_number',
						option: KernelX.Tool.Marmalade.DZip.VersionNumberE,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				worker: ({ resource_directory, data_file, version_number }) => {
					let data = Support.Marmalade.DZip.PackAutomatic.pack(resource_directory, version_number as any);
					KernelX.Storage.write_file(data_file, data[0].view().sub(Kernel.Size.value(0n), data[1]));
					return;
				},
				batch_argument: null,
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

Twinning.Script.Executor.Implement.marmalade.dzip.injector;