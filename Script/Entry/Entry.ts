namespace TwinStar.Script.Entry {

	// ------------------------------------------------

	export const g_executor_method: Array<Executor.Method> = [];

	export const g_executor_method_of_batch: Array<Executor.Method> = [];

	// ------------------------------------------------

	export type CommonFileSystemArgument = {
		fs_tactic_if_exist: 'none' | 'trash' | 'delete' | 'override';
	};

	export const k_common_file_system_argument: CommonFileSystemArgument = {
		fs_tactic_if_exist: 'none',
	};

	// ------------------------------------------------

	/** @see {@link CommonFileSystemArgument} */
	export type CFSA = CommonFileSystemArgument;

	/** @see {@link k_common_file_system_argument} */
	export const k_cfsa = k_common_file_system_argument;

	// ------------------------------------------------

	export function file_system_path_test_generator(
		filter: Array<['any' | 'file' | 'directory', null | RegExp]>,
	): (path: string) => boolean {
		return (path) => {
			for (let e of filter) {
				if (!CoreX.FileSystem[e[0] === 'file' ? 'exist_file' : e[0] === 'directory' ? 'exist_directory' : 'exist'](path)) {
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
		let item_list = CoreX.FileSystem[filter[0] === 'file' ? 'list_file' : filter[0] === 'directory' ? 'list_directory' : 'list'](parent);
		if (filter[1] !== null) {
			item_list = item_list.filter((e) => (filter[1]!.test(e)));
		}
		if (item_list.length === 0) {
			Console.message('w', los('entry:batch_no_item'), [
			]);
		} else {
			let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Console.message('i', `${progress}`, [`${item}`]);
				try {
					worker(item);
				} catch (e: any) {
					Console.message_error(e);
					Console.pause();
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	type Configuration = {
		language: string;
		cli_disable_virtual_terminal_sequence: boolean;
		pause_when_finish: boolean;
		thread_limit: bigint;
		byte_stream_use_big_endian: boolean;
		common_buffer_size: string;
		json_format: {
			disable_trailing_comma: boolean;
			disable_array_wrap_line: boolean;
		};
		common_file_system_argument: {
			fs_tactic_if_exist: string,
		},
	};

	export function _injector(
		configuration: Configuration,
	) {
		// set language
		Language.push_table(configuration.language, CoreX.JSON.read_fs_js(HomeDirectory.of(`~/script/Language/${configuration.language}.json`)) as unknown as Language.Map);
		Language.set_target(configuration.language);
		// cli disable virtual-terminal-sequence
		Console.cli_disable_virtual_terminal_sequence = configuration.cli_disable_virtual_terminal_sequence;
		// set byte stream endian
		Core.Miscellaneous.g_byte_stream_use_big_endian.value = configuration.byte_stream_use_big_endian;
		// set common buffer size
		CoreX.g_common_buffer.allocate(Core.Size.value(parse_size_string(configuration.common_buffer_size)));
		// set json format
		CoreX.JSON.g_format.disable_trailing_comma = configuration.json_format.disable_trailing_comma;
		CoreX.JSON.g_format.disable_array_wrap_line = configuration.json_format.disable_array_wrap_line;
		// set common file system argument
		k_common_file_system_argument.fs_tactic_if_exist = configuration.common_file_system_argument.fs_tactic_if_exist as any;
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
			Console.message('i', los('entry:input_command'), [
				los('entry:input_finish_if_null'),
			]);
			while (true) {
				let input = Console.string(true, null);
				if (input === null) {
					break;
				}
				if (input.startsWith('"') && input.endsWith('"')) {
					input = input.substring(1, input.length - 1);
				}
				raw_command.push(input);
			}
		}
		let command = Executor.parse(raw_command);
		let method = [...g_executor_method, ...g_executor_method_of_batch];
		Console.message('i', los('entry:all_command_parse'), [
			los('entry:all_command_count', command.length),
		]);
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		for (let item of command) {
			progress.increase();
			Console.message('i', los('entry:all_command_finish', progress), [
				`${item.input === null ? '?' : item.input.value}${item.method === null ? '' : ` | ${item.method}`}`,
			]);
			let current_exception = null;
			let current_timer = new Timer();
			current_timer.start();
			try {
				Executor.execute(item, method);
			} catch (e: any) {
				current_exception = e;
			}
			current_timer.stop();
			if (current_exception !== null) {
				Console.message_error(current_exception);
				Console.pause();
			}
		}
		timer.stop();
		Console.message('s', los('entry:all_command_finish'), [
			los('entry:all_command_duration', (timer.duration() / 1000).toFixed(3)),
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