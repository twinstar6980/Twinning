namespace Twinning.Script.Entry {

	// ------------------------------------------------

	let g_configuration: Configuration = undefined!;

	let g_configuration_action: { [Key in keyof Configuration]: (value: Configuration[Key]) => void; } = undefined!;

	function update_configuration(
		configuration: Partial<Configuration>,
	): void {
		for (let key in configuration) {
			assert_test(g_configuration.hasOwnProperty(key));
			let value = (configuration as any)[key];
			(g_configuration as any)[key] = value;
			(g_configuration_action as any)[key](value);
		}
		return;
	}

	// ------------------------------------------------

	export function simple_batch_execute(
		parent: string,
		filter: ['any' | 'file' | 'directory', null | RegExp],
		worker: (item: string) => void,
	): void {
		let item_list = KernelX.Storage[({ any: 'list', file: 'list_file', directory: 'list_directory' } as const)[filter[0]]](parent);
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
		console_basic_disable_virtual_terminal_sequence: boolean;
		executor_typical_method_disable_name_filter: boolean;
		byte_stream_use_big_endian: boolean;
		common_buffer_size: string;
		json_format_disable_array_trailing_comma: boolean;
		json_format_disable_array_line_breaking: boolean;
		json_format_disable_object_trailing_comma: boolean;
		json_format_disable_object_line_breaking: boolean;
		external_program_path_sh: null | string;
		external_program_path_adb: null | string;
		external_program_path_vgmstream: null | string;
		external_program_path_wwise: null | string;
		external_program_path_il2cpp_dumper: null | string;
		thread_limit: bigint;
		command_notification_time_limit: null | bigint;
	};

	export function injector(
		configuration: Configuration,
	): void {
		g_configuration = {
			language: undefined!,
			console_basic_disable_virtual_terminal_sequence: undefined!,
			executor_typical_method_disable_name_filter: undefined!,
			byte_stream_use_big_endian: undefined!,
			common_buffer_size: undefined!,
			json_format_disable_array_trailing_comma: undefined!,
			json_format_disable_array_line_breaking: undefined!,
			json_format_disable_object_trailing_comma: undefined!,
			json_format_disable_object_line_breaking: undefined!,
			external_program_path_sh: undefined!,
			external_program_path_adb: undefined!,
			external_program_path_vgmstream: undefined!,
			external_program_path_wwise: undefined!,
			external_program_path_il2cpp_dumper: undefined!,
			thread_limit: undefined!,
			command_notification_time_limit: undefined!,
		};
		g_configuration_action = {
			language: (value) => {
				Language.imbue(KernelX.JSON.read_fs_js(Home.of(`~/script/Language/${value}.json`)) as unknown as Language.StringMap);
			},
			console_basic_disable_virtual_terminal_sequence: (value) => {
				Console.g_basic_disable_virtual_terminal_sequence = value;
			},
			executor_typical_method_disable_name_filter: (value) => {
				Executor.g_typical_method_disable_name_filter = value;
			},
			byte_stream_use_big_endian: (value) => {
				Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value = value;
			},
			common_buffer_size: (value) => {
				KernelX.g_common_buffer.allocate(Kernel.Size.value(parse_size_string(value)));
			},
			json_format_disable_array_trailing_comma: (value) => {
				KernelX.JSON.g_format.disable_array_trailing_comma = value;
			},
			json_format_disable_array_line_breaking: (value) => {
				KernelX.JSON.g_format.disable_array_line_breaking = value;
			},
			json_format_disable_object_trailing_comma: (value) => {
				KernelX.JSON.g_format.disable_object_trailing_comma = value;
			},
			json_format_disable_object_line_breaking: (value) => {
				KernelX.JSON.g_format.disable_object_line_breaking = value;
			},
			external_program_path_sh: (value) => {
				AndroidHelper.g_sh_program_file = value;
			},
			external_program_path_adb: (value) => {
				AndroidHelper.g_adb_program_file = value;
			},
			external_program_path_vgmstream: (value) => {
				Support.Wwise.Media.Decode.g_vgmstream_program_file = value;
			},
			external_program_path_wwise: (value) => {
				Support.Wwise.Media.Encode.g_wwise_program_file = value;
			},
			external_program_path_il2cpp_dumper: (value) => {
				Support.Kairosoft.Game.ModifyProgram.g_il2cpp_dumper_program_file = value;
			},
			thread_limit: (value) => {
				g_thread_manager.resize(Number(value));
			},
			command_notification_time_limit: (value) => {
			},
		};
		update_configuration(configuration);
		return;
	}

	export function entry(
		argument: Array<string>,
	): string {
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
				item.input === null ? '?' : item.input.value,
				item.method === null ? '?' : item.method,
				...record_to_array(item.argument, (key, value) => (`"${key}": ${KernelX.JSON.write_s_js(value, true, true, true, true)}`)),
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
				if (g_configuration.command_notification_time_limit !== null && g_configuration.command_notification_time_limit <= state[1]) {
					Console.push_system_notification(los('entry:current_command_finish'), los('entry:duration', (state[1] / 1000).toFixed(3)));
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

Twinning.Script.Entry.injector;