namespace Twinning.Script.Executor.Implement.wwise.media {

	// ------------------------------------------------

	// encode *
	// decode *

	export type Configuration = {
		method: TypicalMethodConfigurationGroup;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method(configuration.method, 'wwise.media', [
			typical_method({
				id: 'encode',
				filter: 'file',
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
						automatic: (argument: { raw_file: string; }) => (argument.raw_file.replace(/(\.wav)?$/i, '.wem')),
						condition: null,
					}),
					typical_argument_string({
						id: 'format',
						option: Support.Wwise.Media.FormatE,
						checker: null,
						automatic: null,
						condition: null,
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
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'ripe_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { raw_file: string; }) => (argument.raw_file + '.encode'),
						condition: null,
						item_mapper: (argument: {}, value) => (value.replace(/(\.wav)?$/i, '.wem')),
					}),
				],
				batch_worker: null,
			}),
			typical_method({
				id: 'decode',
				filter: 'file',
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
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file.replace(/(\.wem)?$/i, '.wav')),
						condition: null,
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
						item_mapper: (argument: {}, value) => (value),
					}),
					typical_argument_batch({
						id: 'raw_file',
						rule: 'output',
						checker: null,
						automatic: (argument: { ripe_file: string; }) => (argument.ripe_file + '.decode'),
						condition: null,
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

Twinning.Script.Executor.Implement.wwise.media.injector;