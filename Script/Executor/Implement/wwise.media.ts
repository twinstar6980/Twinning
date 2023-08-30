namespace TwinStar.Script.Executor.Implement.wwise.media {

	// ------------------------------------------------

	// decode *

	export type Configuration = {
		tool_ffmpeg_program_file: TypicalArgumentExpression<string>;
		tool_ww2ogg_program_file: TypicalArgumentExpression<string>;
		tool_ww2ogg_code_book_file: TypicalArgumentExpression<string>;
	};

	export function injector(
		configuration: Configuration,
	): void {
		push_typical_method('wwise.media', [
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
					typical_argument_path({
						id: 'tool_ffmpeg_program_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.tool_ffmpeg_program_file,
					}),
					typical_argument_path({
						id: 'tool_ww2ogg_program_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.tool_ww2ogg_program_file,
					}),
					typical_argument_path({
						id: 'tool_ww2ogg_code_book_file',
						rule: ['file', 'input'],
						checker: null,
						automatic: null,
						condition: null,
						default: configuration.tool_ww2ogg_code_book_file,
					}),
				],
				worker: (argument) => {
					let { ripe_file, raw_file, tool_ffmpeg_program_file, tool_ww2ogg_program_file, tool_ww2ogg_code_book_file } = argument;
					let temporary_directpry = Home.new_temporary();
					KernelX.Tool.Wwise.Media.decode_fs(ripe_file, raw_file, tool_ffmpeg_program_file, tool_ww2ogg_program_file, tool_ww2ogg_code_book_file, temporary_directpry, {});
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