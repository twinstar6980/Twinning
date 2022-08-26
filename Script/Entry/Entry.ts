/** 以命令处理器为核心的模块入口实现 */
namespace TwinKleS.Entry {

	// ------------------------------------------------

	/** 工作空间 */
	export let g_workspace: string = '';

	/**
	 * 将文件回收到垃圾箱目录
	 * @param path 需要回收的文件
	 */
	export function trash(
		path: string,
	): void {
		let trash_sub_direcotry = `${g_workspace}/trash/${date_to_simple_string(new Date())}`;
		CoreX.FileSystem.create_directory(trash_sub_direcotry);
		CoreX.FileSystem.rename(path, `${trash_sub_direcotry}/${PathUtility.split_pair(path)[1]}`);
		return;
	}

	// ------------------------------------------------

	export namespace ArgumentParser {

		export function path(
			origin_value: string | '?default' | '?input',
			option: {
				input_message: string;
				default_value: string;
			} & ({
				must_exist: null;
			} | {
				must_exist: false;
				if_exist: 'trash' | 'delete' | 'override' | '?input';
			} | {
				must_exist: true;
				type: 'any' | 'file' | 'directory';
			}),
		): string {
			let result: string;
			if (origin_value === '?input') {
				result = Input.string(option.input_message)!;
			} else if (origin_value === '?default') {
				result = option.default_value;
			} else {
				result = origin_value;
			}
			if (option.must_exist !== null) {
				if (!option.must_exist) {
					if (CoreX.FileSystem.exist(result)) {
						switch (option.if_exist) {
							case 'trash': {
								Output.w(`指定路径已存在，将进行回收`);
								Output.v(`${result}`, +1);
								trash(result);
								break;
							}
							case 'delete': {
								Output.w(`指定路径已存在，将进行删除`);
								Output.v(`${result}`, +1);
								CoreX.FileSystem.remove(result);
								break;
							}
							case 'override': {
								Output.w(`指定路径已存在，将进行覆写`);
								Output.v(`${result}`, +1);
								break;
							}
							case '?input': {
								Output.e(`指定路径已存在，请输入不存在的路径，或输入 :t 回收之，或输入 :d 删除之，或输入 :o 覆写之`);
								Output.v(`${result}`, +1);
								while (true) {
									let input = Input.string(null)!;
									if (input === ':t') {
										trash(result);
										break;
									}
									if (input === ':d') {
										CoreX.FileSystem.remove(result);
										break;
									}
									if (input === ':o') {
										break;
									}
									result = input;
									if (!CoreX.FileSystem.exist(input)) {
										break;
									}
									Output.e(`指定路径已存在，请重新输入`);
								}
								break;
							}
						}
					}
				} else {
					let type_name: string;
					let test_function: (path: string) => boolean;
					switch (option.type) {
						case 'any': {
							type_name = `路径`;
							test_function = CoreX.FileSystem.exist;
							break;
						}
						case 'file': {
							type_name = `文件`;
							test_function = CoreX.FileSystem.exist_file;
							break;
						}
						case 'directory': {
							type_name = `目录`;
							test_function = CoreX.FileSystem.exist_directory;
							break;
						}
					}
					if (!test_function(result)) {
						Output.e(`指定${type_name}不存在，请输入已存在的路径，或输入 :d 唤起选择窗口（windows）`);
						Output.v(`${result}`, +1);
						while (true) {
							let input = Input.string(null)!;
							if (input === ':s') {
								if (Shell.name !== 'windows.cli') {
									throw new MyError(`:s only enabled on windows.cli shell`);
								}
								let pick_folder: boolean;
								switch (option.type) {
									case 'any': {
										pick_folder = Input.yon(`选择窗口是否应选择目录（否则为文件）？`)!;
										break;
									}
									case 'file': {
										pick_folder = false;
										break;
									}
									case 'directory': {
										pick_folder = true;
										break;
									}
								}
								let selected = Shell.Windows.select_file_by_dialog(pick_folder, false);
								if (selected.length === 0) {
									input = result;
								} else {
									input = selected[0];
								}
							}
							result = input;
							if (test_function(input)) {
								break;
							}
							Output.e(`指定${type_name}不存在，请重新输入`);
						}
					}
				}
			}
			return result;
		}

	}

	// ------------------------------------------------

	/** 全局执行器功能 */
	export const g_executor_method: Array<Executor.Method> = [];

	/** 全局执行器功能（用于批处理） */
	export const g_executor_method_of_batch: Array<Executor.Method> = [];

	// ------------------------------------------------

	/** 用于执行器功能的通用文件系统相关参数 */
	export type CommonFileSystemArgument = {
		fs_if_exist: 'trash' | 'delete' | 'override' | '?input';
	};

	/** 用于执行器功能的通用文件系统相关参数的默认值 */
	export const k_common_file_system_argument: CommonFileSystemArgument = {
		fs_if_exist: '?input',
	};

	// ------------------------------------------------

	/** @see {@link CommonFileSystemArgument} */
	export type CFSA = CommonFileSystemArgument;

	/** @see {@link k_common_file_system_argument} */
	export const k_cfsa = k_common_file_system_argument;

	// ------------------------------------------------

	/**
	 * 生成用于测试文件路径是否有效的简易函数
	 * @param filter 有效路径规则
	 * @returns 测试函数
	 */
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

	/**
	 * 简单批处理执行
	 * @param directory 扫描路径
	 * @param filter 子项过滤规则
	 * @param work 工作函数
	 */
	export function simple_batch_execute(
		directory: string,
		filter: ['any' | 'file' | 'directory', null | RegExp],
		work: (item: string) => void,
	): void {
		let item_list = CoreX.FileSystem[filter[0] === 'file' ? 'list_file' : filter[0] === 'directory' ? 'list_directory' : 'list'](directory);
		if (filter[1] !== null) {
			item_list = item_list.filter((e) => (filter[1]!.test(e)));
		}
		if (item_list.length === 0) {
			Output.w(`无可处理项目`);
		} else {
			let progress = new TextGenerator.Progress('fraction', 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Output.i(`\r${progress} 处理中... `, 0, null, false);
				try {
					work(item);
				} catch (e: any) {
					Output.e(`失败：${item}`);
					if (e instanceof Error) {
						Output.e(`${e}\n${e.stack}`);
					} else {
						Output.e(`${e}`);
					}
					Input.pause();
				}
			}
			Output.i(``);
		}
		return;
	}

	// ------------------------------------------------

	type Config = {
		force_enable_virtual_terminal_sequences: boolean;
		workspace: string;
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
		// enable virtual-terminal-processing
		switch (Shell.name) {
			case 'windows.cli': {
				let output_mode = Shell.Windows.CLI.get_standard_console_mode('output');
				if ((output_mode & Shell.Windows.CLI.OutputConsoleMode.ENABLE_VIRTUAL_TERMINAL_PROCESSING) !== 0) {
					Output.g_enable_virtual_terminal_sequences = true;
				} else {
					try {
						Shell.Windows.CLI.set_standard_console_mode('output', output_mode | Shell.Windows.CLI.OutputConsoleMode.ENABLE_VIRTUAL_TERMINAL_PROCESSING);
						Output.g_enable_virtual_terminal_sequences = true;
					} catch (e) {
						Output.g_enable_virtual_terminal_sequences = config.force_enable_virtual_terminal_sequences;
						Output.w(`can not enable virtual terminal processing`);
					}
				}
				break;
			}
			case 'linux.cli': {
				Output.g_enable_virtual_terminal_sequences = config.force_enable_virtual_terminal_sequences;
				break;
			}
		}
		// set workspace
		g_workspace = Main.path_at_home(config.workspace);
		CoreX.FileSystem.set_working_directory(g_workspace);
		// set byte stream endian
		Core.g_byte_stream_use_big_endian.value = config.byte_stream_use_big_endian;
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
		// set thread limit
		g_thread_manager.resize(Number(config.thread_limit));

	}

	export function _entry(
		config: Config,
		argument: Array<string>,
	) {
		let timer = new Timer();
		timer.start();
		let raw_command = [...argument];
		if (raw_command.length === 0) {
			Output.i(`请依次输入命令参数，跳过输入则结束输入`);
			while (true) {
				let input = Input.string(null, null, true);
				if (input === null) {
					break;
				}
				raw_command.push(input);
			}
		}
		let command = Executor.parse(raw_command);
		let method = [...g_executor_method, ...g_executor_method_of_batch];
		Output.i(`解析得 ${command.length} 条命令`);
		let progress_text = new TextGenerator.Progress('fraction', 40, command.length);
		for (let e of command) {
			progress_text.increase();
			Output.i(`${progress_text}`);
			Output.v(`<${e.input!.value}>${e.method === null ? '' : ` -method <${e.method}>`}`, +1);
			try {
				Executor.execute(e, method);
			} catch (e: any) {
				if (e instanceof Error) {
					Output.e(`${e}\n${e.stack}`);
				} else {
					Output.e(`${e}`);
				}
				Input.pause();
			}
		}
		timer.stop();
		Output.i(`完毕：${(timer.duration() / 1000).toFixed(3)}s`);
		if (config.pause_when_finish) {
			Input.pause();
		}
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry._injector,
	entry: TwinKleS.Entry._entry,
});