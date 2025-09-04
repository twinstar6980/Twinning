namespace Twinning.Script.Runner {

	// #region run

	let g_configuration: Configuration = undefined!;

	// ----------------

	export function set_configuration(
		configuration: Partial<Configuration>,
	): void {
		let action: { [Key in keyof Configuration]: (value: Configuration[Key]) => void; } = {
			language: (value) => {
				Language.imbue(KernelX.JSON.read_fs_js(HomePath.of(`~/script/Language/${value}.json`)) as unknown as Language.StringMap);
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
				KernelX.g_common_buffer.allocate(Kernel.Size.value(parse_size_from_string(value)));
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
			external_program_path: (value) => {
				ProcessHelper.g_program_path_map = {};
				for (let item_key in value) {
					let item_value = value[item_key];
					if (item_value !== null) {
						ProcessHelper.g_program_path_map[item_key] = item_value;
					}
				}
			},
			thread_limit: (value) => {
				ThreadManager.g_global_manager.resize(Number(value));
			},
			command_notification_time_limit: (value) => {
			},
		};
		for (let key in configuration) {
			assert_test(g_configuration.hasOwnProperty(key));
			let value = (configuration as any)[key];
			(g_configuration as any)[key] = value;
			(action as any)[key](value);
		}
		return;
	}

	// ----------------

	export function run(
		argument: Array<string>,
	): string {
		let command_line = [...argument];
		if (command_line.length === 0) {
			Console.information(los('runner:input_command'), [
				los('runner:input_finish_if_empty'),
			]);
			while (true) {
				let input = Console.path('any', 'input', true, null);
				if (input === null) {
					break;
				}
				command_line.push(input);
			}
		}
		let command = Executor.parse(command_line);
		Console.information(los('runner:all_command_parse'), [
			los('runner:all_command_count', command.length),
		]);
		let duration = 0;
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		let command_log = '';
		for (let item of command) {
			progress.increase();
			Console.information(los('runner:current_command_execute', progress), [
				item.input,
				item.method,
				...record_to_array(item.argument, (key, value) => (`${key}: ${KernelX.JSON.write_s_js(value, true, true, true, true)}`)),
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
					Console.push_system_notification(los('runner:current_command_finish'), los('runner:duration', (state[1] / 1000).toFixed(3)));
				}
			}
		}
		Console.success(los('runner:all_command_finish'), [
			los('runner:duration', (duration / 1000).toFixed(3)),
		]);
		return command_log;
	}

	// #endregion

	// #region partition function

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
		external_program_path: Record<string, null | string>;
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
			external_program_path: undefined!,
			thread_limit: undefined!,
			command_notification_time_limit: undefined!,
		};
		set_configuration(configuration);
		return;
	}

	// #endregion

}

Twinning.Script.Runner.injector;