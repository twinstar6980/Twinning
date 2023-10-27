namespace TwinStar.Script.Executor.Implement.wwise.media {

	// ------------------------------------------------

	// encode *
	// decode *

	export type Configuration = {
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('wwise.media', [
			typical_method({
				id: 'encode',
				filter: ['file', /(\.wav)$/i],
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
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/(\.wav)?$/i, '.wem')),
						condition: null,
						default: '?automatic',
					}),
					typical_argument_string({
						id: 'format',
						option: Support.Wwise.Media.Encode.FormatE,
						checker: null,
						automatic: null,
						condition: null,
						default: '?input',
					}),
				],
				worker: ({ raw_file, ripe_file, format }) => {
					Support.Wwise.Media.Encode.encode_fs(raw_file, ripe_file, format as any);
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.wav)?$/i, '.wem')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'decode',
				filter: ['file', /(\.wem)$/i],
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
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/(\.wem)?$/i, '.wav')),
						condition: null,
						default: '?automatic',
					}),
				],
				worker: ({ ripe_file, raw_file }) => {
					Support.Wwise.Media.Decode.decode_fs(ripe_file, raw_file);
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
						item_mapper: (argument: {}, value) => (value.replace(/(\.wem)?$/i, '.wav')),
					}),
				],
				batch_worker: null,
			}),
		]);
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Executor.Implement.wwise.media.injector;