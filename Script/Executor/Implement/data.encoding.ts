namespace TwinStar.Script.Executor.Implement.data.encoding {

	// ------------------------------------------------

	// base64.encode *
	// base64.decode *

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('data.encoding', [
			typical_method({
				id: 'base64.encode',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/()?$/i, '.bin')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: (argument) => {
					let { raw_file, ripe_file } = argument;
					KernelX.Tool.Data.Encoding.Base64.encode_fs(raw_file, ripe_file);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'raw_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.encode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'base64.decode',
				filter: ['file', /()$/i],
				argument: [
					typical_argument_path({
						id: 'ripe_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
					typical_argument_path({
						id: 'raw_file',
						rule: ['file', 'output'],
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/()?$/i, '.bin')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: (argument) => {
					let { ripe_file, raw_file } = argument;
					KernelX.Tool.Data.Encoding.Base64.decode_fs(ripe_file, raw_file);
					return;
				},
				batch_argument: [
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'input',
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.decode'),
						condition: null,
						default: '?automatic',
						item_mapper: (argument: {}, value) => (value.replace(/()?$/i, '')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.data.encoding.injector;