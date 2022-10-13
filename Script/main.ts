namespace TwinKleS {

	// ------------------------------------------------

	/** 版本编号 */
	export const k_version = 21;

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

	/** 主函数实现 */
	export namespace Main {

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
				let stream = Core.CharacterStreamView.look(Core.Misc.cast_ByteListView_to_CharacterListView(data.view()));
				let json = Core.JSON.Value.default<ConstraintT>();
				Core.JSON.Read.process(stream, json);
				return json.value;
			}

			// ------------------------------------------------

			export function evaluate(
				script_file: string,
			): any {
				let script = Core.FileSystem.read_file(Core.Path.value(script_file));
				return Core.Misc.g_context.evaluate(Core.Misc.cast_ByteListView_to_CharacterListView(script.view()), Core.String.value(script_file));
			}

			// ------------------------------------------------

			export function notify(
				message: string,
			): void {
				var shell_name = Core.Misc.g_context.shell_callback(Core.StringList.value(['name'])).value[1];
				if (shell_name.endsWith('.cli')) {
					Core.Misc.g_context.shell_callback(Core.StringList.value(['output', `${message}\n`]));
				}
				if (shell_name.endsWith('.gui')) {
					Core.Misc.g_context.shell_callback(Core.StringList.value(['output', 'v', `${message}`]));
				}
				return;
			}

			// ------------------------------------------------

		}

		// ------------------------------------------------

		type ModuleManifest = {
			module: Array<string>;
			entry: null | string;
		};

		type ModuleConfig = Record<string, Core.JSON.JS_Value>;

		type Injector = (config: null | ModuleConfig) => void;

		type Entry = (config: null | ModuleConfig, argument: Array<string>) => void;

		type ModuleEvaluateResult = undefined | {
			injector?: Injector;
			entry?: Entry;
		};

		// ------------------------------------------------

		function load_module(
			manifest: ModuleManifest,
			main_directory: string,
		): [Entry, null | ModuleConfig] | null {
			if (!Detail.exist_directory(main_directory)) {
				throw new Error(`main directory is not found : <${main_directory}>`);
			}
			let entry: [Entry, null | ModuleConfig] | null = null;
			if (manifest.entry !== null && !manifest.module.includes(manifest.entry)) {
				throw new Error(`entry module is invalid : <${manifest.entry}>`);
			}
			for (let module of manifest.module) {
				let script_file = `${main_directory}/${module}.js`;
				let config_file = `${main_directory}/${module}.json`;
				if (!Detail.exist_file(script_file)) {
					throw new Error(`module script file not found : <${module}>`);
				}
				let config: null | ModuleConfig = null;
				if (Detail.exist_file(config_file)) {
					let raw_module_config = Detail.read_json(config_file);
					if (typeof raw_module_config !== 'object' || raw_module_config === null || (raw_module_config as Object).constructor.name !== 'Object') {
						throw new Error(`module config must be object : <${module}>`);
					}
					config = raw_module_config as ModuleConfig;
				}
				let evaluate_result = Detail.evaluate(script_file) as ModuleEvaluateResult;
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

		let g_home_directory: string = undefined!;

		export function path_at_home(
			format: string,
		): string {
			return format.replaceAll(/<home>/g, g_home_directory);
		}

		// ------------------------------------------------

		export let g_module_manifest: ModuleManifest = undefined!;

		export function main(
			script_path: null | string,
			argument: Array<string>,
		): null | string {
			let result: null | string = null;
			Detail.notify(`TwinKleS.ToolKit.Script ${k_version}`);
			try {
				if (script_path === null) {
					throw new Error(`must run as file`);
				}
				let script_path_match = /^(.+)\/script\/main.js$/.exec(script_path.replaceAll('\\', '/'));
				if (script_path_match === null) {
					throw new Error(`script path error`);
				}
				if (script_path_match[1].startsWith('.') || script_path_match[1].startsWith('..')) {
					let current_directory = Core.FileSystem.get_working_directory().value;
					g_home_directory = `${current_directory}/${script_path_match[1]}`;
				} else {
					g_home_directory = script_path_match[1];
				}
				let begin_time = Date.now();
				let entry = load_module(g_module_manifest, `${g_home_directory}/script`);
				let end_time = Date.now();
				Console.notify('s', localized(`所有脚本模块已加载`), [localized(`用时 {} s`, ((end_time - begin_time) / 1000).toFixed(3))]);
				entry?.[0](entry[1], argument);
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
			}
			return result;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

}

TwinKleS.Main.g_module_manifest = {
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
		`utility/Console`,
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
		`Support/PvZ2/RSB/ResourceExtract/ResourceExtract`,
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
		`Entry/method/misc.pvz2.lawn_string_text`,
		`Entry/method/expand`,
	],
	entry: `Entry/Entry`,
};

(TwinKleS.Main.main);