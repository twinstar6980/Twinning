namespace TwinStar {

	// ------------------------------------------------

	/** 版本编号 */
	export const k_version = 27;

	// ------------------------------------------------

	namespace Detail {

		// ------------------------------------------------

		export function exist_file(
			path: string,
		): boolean {
			return Core.FileSystem.exist_file(Core.Path.value(path)).value;
		}

		export function exist_directory(
			path: string,
		): boolean {
			return Core.FileSystem.exist_directory(Core.Path.value(path)).value;
		}

		// ------------------------------------------------

		export function read_json<ConstraintT extends Core.JSON.JS_Value>(
			file: string,
		): ConstraintT {
			let data = Core.FileSystem.read_file(Core.Path.value(file));
			let stream = Core.CharacterStreamView.look(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
			let json = Core.JSON.Value.default<ConstraintT>();
			Core.Tool.Data.Serialization.JSON.Read.process_whole(stream, json);
			return json.value;
		}

		// ------------------------------------------------

		export function get_working_directory(
		): string {
			return Core.FileSystem.get_working_directory().value;
		}

		// ------------------------------------------------

		export function evaluate(
			script_file: string,
		): any {
			let script = Core.FileSystem.read_file(Core.Path.value(script_file));
			return Core.Miscellaneous.g_context.evaluate(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Core.String.value(script_file));
		}

		// ------------------------------------------------

		export function notify(
			message: string,
		): void {
			var shell_name = Core.Miscellaneous.g_context.callback(Core.StringList.value(['name'])).value[1];
			if (shell_name === 'cli') {
				Core.Miscellaneous.g_context.callback(Core.StringList.value(['output', `● ${message}\n`]));
			}
			if (shell_name === 'gui') {
				Core.Miscellaneous.g_context.callback(Core.StringList.value(['output_notify', 'v', `${message}`]));
			}
			return;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/**
	 * 分割错误堆栈字符串
	 * @param stack 堆栈
	 * @returns 分割所得的字符串数组
	 */
	export function parse_stack_string(
		stack: string | undefined,
	): Array<string> {
		if (stack === undefined) {
			return [];
		} else {
			let stack_array = stack.split('\n');
			stack_array.pop();
			stack_array = stack_array.map((e) => {
				let result: string;
				let regexp_result = /    at (.*) \((.*)\)/.exec(e);
				if (regexp_result === null) {
					result = '';
				} else {
					result = `@ ${regexp_result[2]} ${regexp_result[1]}`;
				}
				return result;
			});
			return stack_array;
		}
	}

	// ------------------------------------------------

	/** 主目录 */
	export namespace HomeDirectory {

		// ------------------------------------------------

		export let path: string = undefined!;

		export function of(
			format: string,
		): string {
			return format.replaceAll(/^~(?=[\\\/])/g, path);
		}

		// ------------------------------------------------

		export function script(
		): string {
			return of(`~/script`);
		}

		export function workspace(
		): string {
			return of(`~/workspace`);
		}

		export function trash(
		): string {
			return of(`~/trash`);
		}

		export function temporary(
		): string {
			return of(`~/temporary`);
		}

		// ------------------------------------------------

		/**
		 * 将文件回收到垃圾箱目录
		 * @param path 需要回收的文件
		 */
		export function new_trash(
			path: string,
		): void {
			// TODO : only allow if same drive
			let trash_sub_direcotry = `${trash()}/${date_to_simple_string(new Date())}.${make_prefix_padded_string((Math.random() * 10000).toFixed(0), '0', 4)}`;
			CoreX.FileSystem.create_directory(trash_sub_direcotry);
			CoreX.FileSystem.rename(path, `${trash_sub_direcotry}/${PathUtility.split_pair(path)[1]}`);
			return;
		}

		/**
		 * 创建并返回一个临时目录
		 * @returns 临时目录
		 */
		export function new_temporary(
		): string {
			let temporary_sub_directory = `${temporary()}/${date_to_simple_string(new Date())}.${make_prefix_padded_string((Math.random() * 10000).toFixed(0), '0', 4)}`;
			CoreX.FileSystem.create_directory(temporary_sub_directory);
			return temporary_sub_directory;
		}

		// ------------------------------------------------

		export function initialize(
		): void {
			CoreX.FileSystem.create_directory(workspace());
			CoreX.FileSystem.create_directory(trash());
			CoreX.FileSystem.create_directory(temporary());
			CoreX.FileSystem.set_working_directory(workspace());
			return;
		}

		export function deinitialize(
		): void {
			let temporary_sub_directory_list = CoreX.FileSystem.list(temporary());
			for (let temporary_sub_directory of temporary_sub_directory_list) {
				CoreX.FileSystem.remove(`${temporary()}/${temporary_sub_directory}`);
			}
			return;
		}

		// ------------------------------------------------

	}

	/** 模块加载 */
	export namespace ModuleLoader {

		// ------------------------------------------------

		export type Manifest = {
			module: Array<string>;
			entry: null | string;
		};

		export type Config = Record<string, Core.JSON.JS_Value>;

		export type Injector = (config: null | Config) => void;

		export type Entry = (config: null | Config, argument: Array<string>) => void;

		export type EvaluateResult = undefined | {
			injector?: Injector;
			entry?: Entry;
		};

		// ------------------------------------------------

		export function load(
			manifest: Manifest,
			main_directory: string,
		): [Entry, null | Config] | null {
			if (!Detail.exist_directory(main_directory)) {
				throw new Error(`main directory is not found : <${main_directory}>`);
			}
			let entry: [Entry, null | Config] | null = null;
			if (manifest.entry !== null && !manifest.module.includes(manifest.entry)) {
				throw new Error(`entry module is invalid : <${manifest.entry}>`);
			}
			for (let module of manifest.module) {
				let script_file = `${main_directory}/${module}.js`;
				let config_file = `${main_directory}/${module}.json`;
				if (!Detail.exist_file(script_file)) {
					throw new Error(`module script file not found : <${module}>`);
				}
				let config: null | Config = null;
				if (Detail.exist_file(config_file)) {
					let raw_module_config = Detail.read_json(config_file);
					if (typeof raw_module_config !== 'object' || raw_module_config === null || (raw_module_config as Object).constructor.name !== 'Object') {
						throw new Error(`module config must be object : <${module}>`);
					}
					config = raw_module_config as Config;
				}
				let evaluate_result = Detail.evaluate(script_file) as EvaluateResult;
				if (evaluate_result !== undefined) {
					if (evaluate_result.injector !== undefined) {
						evaluate_result.injector(config);
					}
				}
				if (module === manifest.entry) {
					if (evaluate_result !== undefined && evaluate_result.entry !== undefined) {
						entry = [evaluate_result.entry as Entry, config];
					} else {
						throw new Error(`module is loaded, but entry function is not found : <${module}>`);
					}
				}
			}
			return entry;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

	/** 主函数实现 */
	export namespace Main {

		// ------------------------------------------------

		export let g_module_manifest: ModuleLoader.Manifest = undefined!;

		export function main(
			argument: Array<string>,
		): null | string {
			let result: null | string = null;
			let load_module_success = false;
			try {
				Detail.notify(`TwinStar.ToolKit ~ Core:${Core.Miscellaneous.g_version.value} & Shell:${Core.Miscellaneous.g_context.callback(Core.StringList.value(['name'])).value[1]}:${Core.Miscellaneous.g_context.callback(Core.StringList.value(['version'])).value[1]} & Script:${k_version} ~ ${Core.Miscellaneous.g_context.callback(Core.StringList.value(['system'])).value[1]}`);
				if (argument.length < 1) {
					throw new Error(`argument too few`);
				}
				// 获取主目录
				let home_directory = argument[0];
				home_directory = home_directory.replaceAll(`\\`, '/');
				if (/^\.{1,2}[\/]/.test(home_directory)) {
					home_directory = `${Detail.get_working_directory()}/${home_directory}`;
				}
				HomeDirectory.path = home_directory;
				// 加载子模块
				// 仅当所有子模块加载完成时，它们才处于可用状态，否则，请勿使用任何子模块
				let begin_time = Date.now();
				let entry = ModuleLoader.load(g_module_manifest, HomeDirectory.script());
				let end_time = Date.now();
				load_module_success = true;
				// 现在，子模块可用，因此应使用 Console 模块来保证更好的交互效果
				try {
					Console.notify('s', los(`模块加载完成`), [los(`用时 {} s`, ((end_time - begin_time) / 1000).toFixed(3))]);
					HomeDirectory.initialize();
					{
						//Console.notify('s', EnvironmentVariable.search_from_path('adb')!, []);
						// let env = CoreX.Process.environment();
						// let em = EnvironmentVariable.parse(env);
						// Console.notify('w', 'env', record_to_array(em, (key, value) => (`${key}: ${value}`)));
						// let demo = HomeDirectory.of('~/workspace/demo');
						// let r = CoreX.Process.execute(
						// 	demo + "/build/windows/x64/release/main.exe",
						// 	//demo + "/build/linux/x86_64/release/main",
						// 	//demo + "/build/macosx/x86_64/release/main",
						// 	['s', `a"r"" fz "d "'z`],
						// 	env,
						// 	demo + `/in`,
						// 	demo + `/out`,
						// 	demo + `/err`,
						// );
						// Console.notify('w', r.toString(16), []);
					}
					{
						// Console.notify('w', '输入测试', []);
						// Console.notify('i', 'pause', []);
						// Console.pause();
						// Console.notify('i', 'confirm', []);
						// Console.confirm(null, true);
						// Console.notify('i', 'number', []);
						// Console.number(null, true);
						// Console.notify('i', 'integer', []);
						// Console.integer(null, true);
						// Console.notify('i', 'size', []);
						// Console.size(null, true);
						// Console.notify('i', 'string', []);
						// Console.string(null, true);
						// Console.notify('i', 'option', []);
						// Console.option([[43]], null, true);
					}
					entry?.[0](entry[1], argument.slice(1));
				} catch (e: any) {
					Console.notify_error(e);
					Console.pause();
				}
			} catch (error: any) {
				if (error instanceof Error) {
					if (error.name === 'NativeError') {
						result = `${error.name}\n${[...error.message.split('\n'), ...parse_stack_string(error.stack)].join('\n')}`;
					} else {
						result = `${error.name} : ${error.message}\n${[...parse_stack_string(error.stack)].join('\n')}`;
					}
				} else {
					result = `${error}`;
				}
			} finally {
				// 需要释放的资源可能位于子模块中，而目前无法保证子模块已被成功加载
				if (load_module_success) {
					HomeDirectory.deinitialize();
					g_thread_manager.resize(0);
				}
			}
			return result;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}

TwinStar.Main.g_module_manifest = {
	module: [
		`utility/Timer`,
		`utility/TypeUtility`,
		`utility/PathUtility`,
		`utility/Check`,
		`utility/TextGenerator`,
		`utility/VirtualTerminalSequences`,
		`utility/XML`,
		`utility/CoreX`,
		`utility/Shell`,
		`utility/ThreadManager`,
		`utility/EnvironmentVariable`,
		`utility/Console`,
		`utility/ADBHelper`,
		`Language/Language`,
		`Support/MarmaladeDZip/ResourcePack/ResourcePack`,
		`Support/PopCapPAK/ResourcePack/ResourcePack`,
		`Support/Atlas/Pack/AutoPack`,
		`Support/Atlas/Pack/Pack`,
		`Support/PopCapTexture/Encode/Encode`,
		`Support/PopCapAnimation/Convert/common`,
		`Support/PopCapAnimation/Convert/Flash/common`,
		`Support/PopCapAnimation/Convert/Flash/From`,
		`Support/PopCapAnimation/Convert/Flash/To`,
		`Support/PopCapAnimation/Convert/Flash/SourceManager`,
		`Support/PvZ2/JSONGenericGetter/JSONGenericGetter`,
		`Support/PvZ2/LawnStringText/LawnStringText`,
		`Support/PvZ2/RSB/ResourceManifest/Convert`,
		`Support/PvZ2/RSB/ResourceManifest/ResourceManifest`,
		`Support/PvZ2/RSB/ResourceManifest/OfficialResourceManifest`,
		`Support/PvZ2/RSB/ResourceConvert/ResourceConvert`,
		`Support/PvZ2/RemoteAndroidHelper`,
		`Executor/Method`,
		`Executor/Argument`,
		`Executor/Command`,
		`Entry/Entry`,
		`Entry/method/js`,
		`Entry/method/json`,
		`Entry/method/data.hash`,
		`Entry/method/data.encode`,
		`Entry/method/data.encrypt`,
		`Entry/method/data.compress`,
		`Entry/method/image.atlas`,
		`Entry/method/wwise.encoded_media`,
		`Entry/method/wwise.sound_bank`,
		`Entry/method/marmalade.dzip`,
		`Entry/method/popcap.zlib`,
		`Entry/method/popcap.reanim`,
		`Entry/method/popcap.rton`,
		`Entry/method/popcap.ptx`,
		`Entry/method/popcap.pam`,
		`Entry/method/popcap.pak`,
		`Entry/method/popcap.rsgp`,
		`Entry/method/popcap.rsb`,
		`Entry/method/pvz2.lawn_string_text`,
		`Entry/method/pvz2.remote_android_helper`,
		`Entry/method/expand`,
	],
	entry: `Entry/Entry`,
};

(TwinStar.Main.main);