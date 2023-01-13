/** 以命令处理器为核心的模块入口实现 */
namespace TwinStar.Entry {

	// ------------------------------------------------

	/** 工作空间 */
	export let g_workspace: string = undefined!;

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

	/**
	 * 创建并返回一个临时目录
	 * @param name 
	 * @returns 临时目录
	 */
	export function temporary(
		name: string | undefined = undefined,
	): string {
		let temporary_sub_directory = `${g_workspace}/temporary/${date_to_simple_string(new Date())}`;
		CoreX.FileSystem.create_directory(temporary_sub_directory);
		return temporary_sub_directory;
	}

	// ------------------------------------------------

	/** 全局执行器功能 */
	export const g_executor_method: Array<Executor.Method> = [];

	/** 全局执行器功能（用于批处理） */
	export const g_executor_method_of_batch: Array<Executor.Method> = [];

	// ------------------------------------------------

	/** 用于执行器功能的通用文件系统相关参数 */
	export type CommonFileSystemArgument = {
		fs_tactic_if_exist: 'trash' | 'delete' | 'override' | null;
	};

	/** 用于执行器功能的通用文件系统相关参数的默认值 */
	export const k_common_file_system_argument: CommonFileSystemArgument = {
		fs_tactic_if_exist: null,
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
			Console.notify('w', localized(`无可处理项目`), []);
		} else {
			let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
			for (let item of item_list) {
				progress.increase();
				Console.notify('i', `${progress}`, [`${item}`]);
				try {
					work(item);
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
		// set language target
		Language.g_target = config.language;
		// cli disable virtual-terminal-sequences
		Console.cli_disable_virtual_terminal_sequences = config.cli_disable_virtual_terminal_sequences;
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
			Console.notify('i', localized(`请依次输入命令参数`), [localized(`输入为空则结束输入`)]);
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
		Console.notify('i', localized(`所有命令已解析`), [localized(`共 {} 条`, command.length)]);
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		for (let e of command) {
			progress.increase();
			Console.notify('i', localized(`命令执行中：{}`, progress), [`${e.input === null ? '?' : e.input.value}${e.method === null ? '' : ` | ${e.method}`}`]);
			try {
				Executor.execute(e, method);
			} catch (e: any) {
				Console.notify_error(e);
				Console.pause();
			}
		}
		timer.stop();
		Console.notify('s', localized(`所有命令已执行`), [localized(`用时 {} s`, (timer.duration() / 1000).toFixed(3))]);
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