namespace TwinStar.Entry {

	// ------------------------------------------------

	export const g_executor_method: Array<Executor.Method> = [];

	export const g_executor_method_of_batch: Array<Executor.Method> = [];

	// ------------------------------------------------

	export type CommonFileSystemArgument = {
		fs_tactic_if_exist: 'trash' | 'delete' | 'override' | null;
	};

	export const k_common_file_system_argument: CommonFileSystemArgument = {
		fs_tactic_if_exist: null,
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
		directory: string,
		filter: ['any' | 'file' | 'directory', null | RegExp],
		worker: (item: string) => void,
	): void {
		let item_list = CoreX.FileSystem[filter[0] === 'file' ? 'list_file' : filter[0] === 'directory' ? 'list_directory' : 'list'](directory);
		if (filter[1] !== null) {
			item_list = item_list.filter((e) => (filter[1]!.test(e)));
		}
		if (item_list.length === 0) {
			Console.notify('w', los(`无可处理项目`), []);
		} else {
			let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Console.notify('i', `${progress}`, [`${item}`]);
				try {
					worker(item);
				} catch (e: any) {
					Console.notify_error(e);
					Console.pause();
				}
			}
		}
		return;
	}

	// ------------------------------------------------

	type Config = {
		language: string;
		cli_disable_virtual_terminal_sequences: boolean;
		byte_stream_use_big_endian: boolean;
		json_write: {
			buffer_size: string;
			format: {
				disable_trailing_comma: boolean;
				disable_array_wrap_line: boolean;
			};
		};
		pause_when_finish: boolean;
		thread_limit: bigint;
	};

	export function _injector(
		config: Config,
	) {
		// set language
		Language.push_table(config.language, CoreX.JSON.read_fs_js(HomeDirectory.of(`~/script/Language/${config.language}.json`)) as unknown as Language.Map);
		Language.set_target(config.language);
		// cli disable virtual-terminal-sequences
		Console.cli_disable_virtual_terminal_sequences = config.cli_disable_virtual_terminal_sequences;
		// set byte stream endian
		Core.Miscellaneous.g_byte_stream_use_big_endian.value = config.byte_stream_use_big_endian;
		// set json write option
		{
			CoreX.JSON.set_write_buffer_size(
				parse_size_string(config.json_write.buffer_size),
			);
			CoreX.JSON.set_write_format(
				config.json_write.format.disable_trailing_comma,
				config.json_write.format.disable_array_wrap_line,
			);
		}
	}

	export function _entry(
		config: Config,
		argument: Array<string>,
	) {
		g_thread_manager.resize(Number(config.thread_limit));
		let timer = new Timer();
		timer.start();
		let raw_command = [...argument];
		if (raw_command.length === 0) {
			Console.notify('i', los(`请依次输入命令参数`), [los(`输入为空则结束输入`)]);
			while (true) {
				let input = Console.string(null, true);
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
		Console.notify('i', los(`所有命令已解析`), [los(`共 {} 条`, command.length)]);
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		for (let e of command) {
			progress.increase();
			Console.notify('i', los(`命令执行中：{}`, progress), [`${e.input === null ? '?' : e.input.value}${e.method === null ? '' : ` | ${e.method}`}`]);
			try {
				Executor.execute(e, method);
			} catch (e: any) {
				Console.notify_error(e);
				Console.pause();
			}
		}
		timer.stop();
		Console.notify('s', los(`所有命令已执行`), [los(`用时 {} s`, (timer.duration() / 1000).toFixed(3))]);
		if (config.pause_when_finish) {
			Console.pause();
		}
		return;
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Entry._injector,
	entry: TwinStar.Entry._entry,
});