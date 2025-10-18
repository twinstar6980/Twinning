namespace Twinning.Script.Executor.Implement.Data.Compression {

	// #region interface

	export function activate(
	): void {
		push_typical_method('data.compression', [
			typical_method({
				id: 'deflate.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					KernelX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'none');
					return;
				},
			}),
			typical_method({
				id: 'deflate.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
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
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'none', temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'zlib.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					KernelX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'zlib');
					return;
				},
			}),
			typical_method({
				id: 'zlib.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
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
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'zlib', temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'gzip.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					KernelX.Tool.Data.Compression.Deflate.compress_fs(raw_file, ripe_file, 9n, 15n, 9n, 'default_mode', 'gzip');
					return;
				},
			}),
			typical_method({
				id: 'gzip.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
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
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Data.Compression.Deflate.uncompress_fs(ripe_file, raw_file, 15n, 'gzip', temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'bzip2.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					KernelX.Tool.Data.Compression.BZip2.compress_fs(raw_file, ripe_file, 9n);
					return;
				},
			}),
			typical_method({
				id: 'bzip2.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
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
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Data.Compression.BZip2.uncompress_fs(ripe_file, raw_file, temporary.buffer.view());
					return;
				},
			}),
			typical_method({
				id: 'lzma.compress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
					}),
				],
				batch: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.compress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ raw_file, ripe_file }, temporary: {}) => {
					KernelX.Tool.Data.Compression.Lzma.compress_fs(raw_file, ripe_file, 9n);
					return;
				},
			}),
			typical_method({
				id: 'lzma.uncompress',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
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
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.uncompress'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				worker: ({ ripe_file, raw_file, buffer_size }, temporary: { buffer: Kernel.ByteArray; }) => {
					if (temporary.buffer === undefined) {
						temporary.buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					}
					KernelX.Tool.Data.Compression.Lzma.uncompress_fs(ripe_file, raw_file, temporary.buffer.view());
					return;
				},
			}),
		]);
		return;
	}

	// #endregion

}