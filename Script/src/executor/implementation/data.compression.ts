namespace Twinning.Script.Executor.Implementation.Data.Compression {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.compression', [
			typical_method({
				identifier: 'deflate.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file}, store: {}) => {
					KernelX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'none');
					return;
				},
			}),
			typical_method({
				identifier: 'deflate.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'none', store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'zlib.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file}, store: {}) => {
					KernelX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'zlib');
					return;
				},
			}),
			typical_method({
				identifier: 'zlib.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'zlib', store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'gzip.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file}, store: {}) => {
					KernelX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'gzip');
					return;
				},
			}),
			typical_method({
				identifier: 'gzip.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'gzip', store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'bzip2.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file}, store: {}) => {
					KernelX.Tool.Data.Compression.Bzip2.compress_fs(raw_file, ripe_file, 9n);
					return;
				},
			}),
			typical_method({
				identifier: 'bzip2.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Data.Compression.Bzip2.uncompress_fs(ripe_file, raw_file, store.buffer.view());
					return;
				},
			}),
			typical_method({
				identifier: 'lzma.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.bin'),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {raw_file: StoragePath}) => ConvertHelper.replace_path_name(argument.raw_file, /()?$/i, '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({raw_file, ripe_file}, store: {}) => {
					KernelX.Tool.Data.Compression.Lzma.compress_fs(raw_file, ripe_file, 9n);
					return;
				},
			}),
			typical_method({
				identifier: 'lzma.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						identifier: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						identifier: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.bin'),
						condition: null,
					}),
					typical_argument_size({
						identifier: 'buffer_size',
						option: null,
						checker: null,
						automatic: null,
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						identifier: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						identifier: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: {ripe_file: StoragePath}) => ConvertHelper.replace_path_name(argument.ripe_file, /()?$/i, '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => ConvertHelper.replace_path_name(value, /()?$/i, ''),
					}),
				],
				worker: ({ripe_file, raw_file, buffer_size}, store: {buffer: Kernel.ByteArray}) => {
					if (store.buffer === undefined) {
						store.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size.value()));
					}
					KernelX.Tool.Data.Compression.Lzma.uncompress_fs(ripe_file, raw_file, store.buffer.view());
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}
