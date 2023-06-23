namespace TwinStar.Script.Entry {

	// ------------------------------------------------

	export const g_executor_method: Array<Executor.Method> = [];

	export const g_executor_method_of_batch: Array<Executor.Method> = [];

	// ------------------------------------------------

	export type CommonArgument = {
		path_tactic_if_out_exist: 'none' | 'trash' | 'delete' | 'override';
	};

	export const k_common_argument: CommonArgument = {
		path_tactic_if_out_exist: 'none',
	};

	// ------------------------------------------------

	export function file_system_path_test_generator(
		filter: Array<['any' | 'file' | 'directory', null | RegExp]>,
	): (path: string) => boolean {
		return (path) => {
			for (let e of filter) {
				if (!CoreX.FileSystem[({ any: 'exist', file: 'exist_file', directory: 'exist_directory' } as const)[e[0]]](path)) {
					continue;
				}
				if (e[1] === null || e[1].test(path)) {
					return true;
				}
			}
			return false;
		};
	}

	// ------------------------------------------------

	export function simple_batch_execute(
		parent: string,
		filter: ['any' | 'file' | 'directory', null | RegExp],
		worker: (item: string) => void,
	): void {
		let item_list = CoreX.FileSystem[({ any: 'list', file: 'list_file', directory: 'list_directory' } as const)[filter[0]]](parent);
		if (filter[1] !== null) {
			item_list = item_list.filter((e) => (filter[1]!.test(e)));
		}
		if (item_list.length === 0) {
			Console.warning(los('entry:batch_no_item'), [
			]);
		} else {
			let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Console.information(`${progress}`, [`${item}`]);
				try {
					worker(item);
				} catch (e: any) {
					Console.error_of(e);
					Console.pause();
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	type Configuration = {
		language: string;
		disable_cli_virtual_terminal_sequence: boolean;
		disable_notification: boolean;
		pause_when_finish: boolean;
		notification_time_limit: null | bigint;
		thread_limit: bigint;
		byte_stream_use_big_endian: boolean;
		common_buffer_size: string;
		json_format: {
			disable_trailing_comma: boolean;
			disable_array_wrap_line: boolean;
		};
		method_common_argument: {
			path_tactic_if_out_exist: string;
		};
	};

	export function _injector(
		configuration: Configuration,
	) {
		// set language
		Language.push_table(configuration.language, CoreX.JSON.read_fs_js(Home.of(`~/script/Language/${configuration.language}.json`)) as unknown as Language.Map);
		Language.set_target(configuration.language);
		// set console feature
		Console.disable_cli_virtual_terminal_sequence = configuration.disable_cli_virtual_terminal_sequence;
		Console.disable_notification = configuration.disable_notification;
		// set byte stream endian
		Core.Miscellaneous.g_context.query_byte_stream_use_big_endian().value = configuration.byte_stream_use_big_endian;
		// set common buffer size
		CoreX.g_common_buffer.allocate(Core.Size.value(parse_size_string(configuration.common_buffer_size)));
		// set json format
		CoreX.JSON.g_format.disable_trailing_comma = configuration.json_format.disable_trailing_comma;
		CoreX.JSON.g_format.disable_array_wrap_line = configuration.json_format.disable_array_wrap_line;
		// set method common argument
		k_common_argument.path_tactic_if_out_exist = configuration.method_common_argument.path_tactic_if_out_exist as any;
	}

	export function _entry(
		configuration: Configuration,
		argument: Array<string>,
	) {
		g_thread_manager.resize(Number(configuration.thread_limit));
		let timer = new Timer();
		timer.start();
		let raw_command = [...argument];
		if (raw_command.length === 0) {
			Console.information(los('entry:input_command'), [
				los('entry:input_finish_if_null'),
			]);
			while (true) {
				let input = Console.path('any', ['in'], true, null);
				if (input === null) {
					break;
				}
				raw_command.push(input);
			}
		}
		let command = Executor.parse(raw_command);
		let method = [...g_executor_method, ...g_executor_method_of_batch];
		Console.information(los('entry:all_command_parse'), [
			los('entry:all_command_count', command.length),
		]);
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		for (let item of command) {
			progress.increase();
			Console.information(los('entry:current_command_execute', progress), [
				`${item.input === null ? '?' : item.input.value}${item.method === null ? '' : ` | ${item.method}`}`,
			]);
			let state = Executor.execute(item, method);
			if (configuration.notification_time_limit !== null && configuration.notification_time_limit <= state[1]) {
				Console.push_notification(los('entry:current_command_finish'), los('entry:duration', (state[1] / 1000).toFixed(3)));
			}
		}
		timer.stop();
		Console.success(los('entry:all_command_finish'), [
			los('entry:duration', (timer.duration() / 1000).toFixed(3)),
		]);
		if (configuration.pause_when_finish) {
			Console.pause();
		}
		return;
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry._injector,
	entry: TwinStar.Script.Entry._entry,
});