namespace TwinStar.Script {

	// ------------------------------------------------

	export const k_version = 48;

	// ------------------------------------------------

	export function assert_test(
		condition: boolean,
		message: string = `assertion failed`,
	): asserts condition {
		if (!condition) {
			throw new Error(message);
		}
		return;
	}

	// ------------------------------------------------

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
			let stream = Core.CharacterStreamView.watch(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
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

		export function new_trash(
			path: string,
		): void {
			// TODO : only allow if same drive
			let trash_sub_direcotry = `${trash()}/${date_to_simple_string(new Date())}.${make_prefix_padded_string((Math.random() * 10000).toFixed(0), '0', 4)}`;
			CoreX.FileSystem.create_directory(trash_sub_direcotry);
			CoreX.FileSystem.rename(path, `${trash_sub_direcotry}/${PathUtility.name(path)}`);
			return;
		}

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
			directory: string,
		): [Entry, null | Config] | null {
			assert_test(Detail.exist_directory(directory), `directory is not found : <${directory}>`);
			let entry: [Entry, null | Config] | null = null;
			assert_test(manifest.entry === null || manifest.module.includes(manifest.entry), `entry module is invalid : <${manifest.entry}>`);
			for (let module of manifest.module) {
				let script_file = `${directory}/${module}.js`;
				let config_file = `${directory}/${module}.json`;
				assert_test(Detail.exist_file(script_file), `module script file not found : <${module}>`);
				let config: null | Config = null;
				if (Detail.exist_file(config_file)) {
					let raw_module_config = Detail.read_json(config_file);
					assert_test(raw_module_config !== null && typeof raw_module_config === 'object' && (raw_module_config as Object).constructor.name === 'Object', `module config must be object : <${module}>`);
					config = raw_module_config as Config;
				}
				let evaluate_result = Detail.evaluate(script_file) as EvaluateResult;
				if (evaluate_result !== undefined) {
					if (evaluate_result.injector !== undefined) {
						evaluate_result.injector(config);
					}
				}
				if (module === manifest.entry) {
					assert_test(evaluate_result !== undefined && evaluate_result.entry !== undefined, `module is loaded, but entry function is not found : <${module}>`);
					entry = [evaluate_result.entry as Entry, config];
				}
			}
			return entry;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

	export namespace Main {

		// ------------------------------------------------

		export let g_module_manifest: ModuleLoader.Manifest = undefined!;

		export function main(
			argument: Array<string>,
		): null | string {
			let result: null | string = null;
			let load_module_succeed = false;
			try {
				Detail.notify(`TwinStar.ToolKit ~ Core:${Core.Miscellaneous.g_version.value} & Shell:${Core.Miscellaneous.g_context.callback(Core.StringList.value(['name'])).value[1]}:${Core.Miscellaneous.g_context.callback(Core.StringList.value(['version'])).value[1]} & Script:${k_version} ~ ${Core.Miscellaneous.g_context.callback(Core.StringList.value(['system'])).value[1]}`);
				assert_test(argument.length >= 1, `argument too few`);
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
				load_module_succeed = true;
				// 现在，子模块可用，因此应使用 Console 模块来保证更好的交互效果
				try {
					Console.notify('s', los(`模块加载完成`), [los(`用时 {} s`, ((end_time - begin_time) / 1000).toFixed(3))]);
					HomeDirectory.initialize();
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
				if (load_module_succeed) {
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

TwinStar.Script.Main.g_module_manifest = {
	module: [
		`utility/Timer`,
		`utility/TypeUtility`,
		`utility/PathUtility`,
		`utility/Check`,
		`utility/TextGenerator`,
		`utility/VirtualTerminalSequence`,
		`utility/XML`,
		`utility/ByteListView`,
		`utility/CoreX`,
		`utility/Shell`,
		`utility/ThreadManager`,
		`utility/EnvironmentVariable`,
		`utility/Console`,
		`utility/ADBHelper`,
		`Language/Language`,
		`Support/Atlas/Pack`,
		`Support/Atlas/PackAutomatic`,
		`Support/Marmalade/DZip/PackAutomatic`,
		`Support/PopCap/ReflectionObjectNotation/DecodeLenient`,
		`Support/PopCap/Texture/Encode`,
		`Support/PopCap/Animation/Convert/common`,
		`Support/PopCap/Animation/Convert/Flash/common`,
		`Support/PopCap/Animation/Convert/Flash/From`,
		`Support/PopCap/Animation/Convert/Flash/To`,
		`Support/PopCap/Animation/Convert/Flash/SourceManager`,
		`Support/PopCap/Package/PackAutomatic`,
		`Support/PopCap/ResourceStreamBundle/Repair`,
		`Support/PvZ2/JSONGenericGetter/JSONGenericGetter`,
		`Support/PvZ2/ResourceStreamBundle/ResourceManifest/Convert`,
		`Support/PvZ2/ResourceStreamBundle/ResourceManifest/ResourceManifest`,
		`Support/PvZ2/ResourceStreamBundle/ResourceManifest/OfficialResourceManifest`,
		`Support/PvZ2/ResourceStreamBundle/ResourceConvert/ResourceConvert`,
		`Support/PvZ2/TextTable/Convert`,
		`Support/PvZ2/RemoteAndroidHelper`,
		`Executor/Method`,
		`Executor/Argument`,
		`Executor/Command`,
		`Entry/Entry`,
		`Entry/method/js`,
		`Entry/method/json`,
		`Entry/method/data.hash`,
		`Entry/method/data.encoding`,
		`Entry/method/data.encryption`,
		`Entry/method/data.compression`,
		`Entry/method/data.differentiation`,
		`Entry/method/image.transformation`,
		`Entry/method/image.atlas`,
		`Entry/method/wwise.media`,
		`Entry/method/wwise.sound_bank`,
		`Entry/method/marmalade.dzip`,
		`Entry/method/popcap.zlib`,
		`Entry/method/popcap.crypt_data`,
		`Entry/method/popcap.reflection_object_notation`,
		`Entry/method/popcap.texture`,
		`Entry/method/popcap.animation`,
		`Entry/method/popcap.re_animation`,
		`Entry/method/popcap.particle`,
		`Entry/method/popcap.trail`,
		`Entry/method/popcap.package`,
		`Entry/method/popcap.resource_stream_group`,
		`Entry/method/popcap.resource_stream_bundle`,
		`Entry/method/popcap.resource_stream_bundle_patch`,
		`Entry/method/pvz2.text_table`,
		`Entry/method/pvz2.remote_android_helper`,
		`Entry/method/expand`,
	],
	entry: `Entry/Entry`,
};

(TwinStar.Script.Main.main);