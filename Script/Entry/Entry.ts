namespace TwinStar.Script.Entry {

	// ------------------------------------------------

	export function simple_batch_execute(
		parent: string,
		filter: ['any' | 'file' | 'directory', null | RegExp],
		worker: (item: string) => void,
	): void {
		let item_list = KernelX.FileSystem[({ any: 'list', file: 'list_file', directory: 'list_directory' } as const)[filter[0]]](parent);
		if (filter[1] !== null) {
			item_list = item_list.filter((e) => (filter[1]!.test(e)));
		}
		if (item_list.length === 0) {
			Console.warning(los('entry:batch_no_item'), [
			]);
		}
		else {
			let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Console.information(`${progress}`, [`${item}`]);
				try {
					worker(item);
				}
				catch (e) {
					Console.error_of(e);
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	export type Configuration = {
		language: string;
		console_cli_disable_virtual_terminal_sequence: boolean;
		executor_typical_method_disable_name_filter: boolean;
		byte_stream_use_big_endian: boolean;
		common_buffer_size: string;
		json_format: {
			disable_array_trailing_comma: boolean;
			disable_array_line_breaking: boolean;
			disable_object_trailing_comma: boolean;
			disable_object_line_breaking: boolean;
		};
		external_program: Record<'sh' | 'adb' | 'vgmstream' | 'wwise', null | string>;
		thread_limit: bigint;
		command_notification_time_limit: null | bigint;
	};

	export function injector(
		configuration: Configuration,
	): void {
		g_configuration = configuration;
		// language
		Language.imbue(KernelX.JSON.read_fs_js(Home.of(`~/script/Language/${configuration.language}.json`)) as unknown as Language.StringMap);
		// console
		Console.g_cli_disable_virtual_terminal_sequence = configuration.console_cli_disable_virtual_terminal_sequence;
		// executor
		Executor.g_typical_method_disable_name_filter = configuration.executor_typical_method_disable_name_filter;
		// byte stream
		Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value = configuration.byte_stream_use_big_endian;
		// common buffer
		KernelX.g_common_buffer.allocate(Kernel.Size.value(parse_size_string(configuration.common_buffer_size)));
		// json format
		KernelX.JSON.g_format.disable_array_trailing_comma = configuration.json_format.disable_array_trailing_comma;
		KernelX.JSON.g_format.disable_array_line_breaking = configuration.json_format.disable_array_line_breaking;
		KernelX.JSON.g_format.disable_object_trailing_comma = configuration.json_format.disable_object_trailing_comma;
		KernelX.JSON.g_format.disable_object_line_breaking = configuration.json_format.disable_object_line_breaking;
		// external program
		AndroidHelper.g_sh_program_file = configuration.external_program.sh;
		AndroidHelper.g_adb_program_file = configuration.external_program.adb;
		Support.Wwise.Media.Decode.g_vgmstream_program_file = configuration.external_program.vgmstream;
		Support.Wwise.Media.Encode.g_wwise_program_file = configuration.external_program.wwise;
		return;
	}

	// ------------------------------------------------

	let g_configuration: Configuration = undefined!;

	export function entry(
		argument: Array<string>,
	): string {
		let configuration = g_configuration;
		g_thread_manager.resize(Number(configuration.thread_limit));
		let raw_command = [...argument];
		if (raw_command.length === 0) {
			Console.information(los('entry:input_command'), [
				los('entry:input_finish_if_null'),
			]);
			while (true) {
				let input = Console.path('any', 'input', true, null);
				if (input === null) {
					break;
				}
				raw_command.push(input);
			}
		}
		let command = Executor.parse(raw_command);
		Console.information(los('entry:all_command_parse'), [
			los('entry:all_command_count', command.length),
		]);
		let duration = 0;
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		let command_log = '';
		for (let item of command) {
			progress.increase();
			Console.information(los('entry:current_command_execute', progress), [
				`${item.input === null ? '?' : item.input.value}${item.method === null ? '' : ` | ${item.method}`}`,
			]);
			let state = Executor.execute(item, Executor.g_method, Executor.g_method_batch);
			if (state === null) {
				command_log += '~';
			}
			else {
				if (state[0] === false) {
					command_log += 'f';
				}
				if (state[0] === true) {
					command_log += 's';
				}
				duration += state[1];
				if (configuration.command_notification_time_limit !== null && configuration.command_notification_time_limit <= state[1]) {
					Console.push_notification(los('entry:current_command_finish'), los('entry:duration', (state[1] / 1000).toFixed(3)));
				}
			}
		}
		Console.success(los('entry:all_command_finish'), [
			los('entry:duration', (duration / 1000).toFixed(3)),
		]);
		return command_log;
	}

	// ------------------------------------------------

}

TwinStar.Script.Entry.injector;