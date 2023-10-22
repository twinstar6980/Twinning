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
		} else {
			let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Console.information(`${progress}`, [`${item}`]);
				try {
					worker(item);
				} catch (e) {
					Console.error_of(e);
					Console.pause();
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	export type Configuration = {
		language: string;
		disable_cli_virtual_terminal_sequence: boolean;
		disable_notification: boolean;
		byte_stream_use_big_endian: boolean;
		common_buffer_size: string;
		json_format: {
			disable_trailing_comma: boolean;
			disable_array_wrap_line: boolean;
		};
		method_common_argument: {
			path_tactic_if_out_exist: string;
		};
		thread_limit: bigint;
		notification_time_limit: null | bigint;
		pause_when_finish: boolean;
	};

	export function injector(
		configuration: Configuration,
	): void {
		g_configuration = configuration;
		// language
		Language.imbue(KernelX.JSON.read_fs_js(Home.of(`~/script/Language/${configuration.language}.json`)) as unknown as Language.StringMap);
		// console feature
		Console.g_disable_cli_virtual_terminal_sequence = configuration.disable_cli_virtual_terminal_sequence;
		Console.g_disable_notification = configuration.disable_notification;
		// byte stream endian
		Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value = configuration.byte_stream_use_big_endian;
		// common buffer size
		KernelX.g_common_buffer.allocate(Kernel.Size.value(parse_size_string(configuration.common_buffer_size)));
		// json format
		KernelX.JSON.g_format.disable_trailing_comma = configuration.json_format.disable_trailing_comma;
		KernelX.JSON.g_format.disable_array_wrap_line = configuration.json_format.disable_array_wrap_line;
		return;
	}

	// ------------------------------------------------

	let g_configuration: Configuration = undefined!;

	export function entry(
		argument: Array<string>,
	): void {
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
		let method = [...Executor.g_method, ...Executor.g_method_batch];
		Console.information(los('entry:all_command_parse'), [
			los('entry:all_command_count', command.length),
		]);
		let duration = 0;
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		for (let item of command) {
			progress.increase();
			Console.information(los('entry:current_command_execute', progress), [
				`${item.input === null ? '?' : item.input.value}${item.method === null ? '' : ` | ${item.method}`}`,
			]);
			let state = Executor.execute(item, method);
			if (state !== null) {
				duration += state[1];
				if (configuration.notification_time_limit !== null && configuration.notification_time_limit <= state[1]) {
					Console.push_notification(los('entry:current_command_finish'), los('entry:duration', (state[1] / 1000).toFixed(3)));
				}
			}
		}
		Console.success(los('entry:all_command_finish'), [
			los('entry:duration', (duration / 1000).toFixed(3)),
		]);
		if (configuration.pause_when_finish) {
			Console.pause();
		}
		return;
	}

	// ------------------------------------------------

}

TwinStar.Script.Entry.injector;