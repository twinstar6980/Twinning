namespace TwinStar.Script {

	// ------------------------------------------------

	export const k_version = 73;

	// ------------------------------------------------

	export function assert_test(
		condition: boolean,
		message: string = ``,
	): asserts condition {
		if (!condition) {
			let error = new Error(message);
			error.name = 'AssertionError';
			throw error;
		}
		return;
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
			Core.Tool.Data.Serialization.JSON.Read.process(stream, json);
			return json.value;
		}

		// ------------------------------------------------

		export function get_working_directory(
		): string {
			return Core.Process.get_working_directory().value;
		}

		// ------------------------------------------------

		export function evaluate(
			script_file: string,
			name: string,
		): any {
			let script = Core.FileSystem.read_file(Core.Path.value(script_file));
			return Core.Miscellaneous.g_context.evaluate(Core.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Core.String.value(name), Core.Boolean.value(false));
		}

		// ------------------------------------------------

		export function output(
			message: string,
		): void {
			var host = Core.Miscellaneous.g_context.callback(Core.StringList.value(['host'])).value[0];
			if (host === 'cli') {
				Core.Miscellaneous.g_context.callback(Core.StringList.value(['output', `● ${message}\n`]));
			}
			if (host === 'gui') {
				Core.Miscellaneous.g_context.callback(Core.StringList.value(['output_message', 'v', `${message}`]));
			}
			return;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

	export namespace Home {

		// ------------------------------------------------

		export let path: string = undefined!;

		export function of(
			format: string,
		): string {
			return format.replaceAll(/^~(?=[/])/g, path);
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
			CoreX.Process.set_working_directory(workspace());
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

		export type Configuration = Record<string, Core.JSON.JS_Value>;

		export type Injector = (configuration: null | Configuration) => void;

		export type Entry = (configuration: null | Configuration, argument: Array<string>) => void;

		export type EvaluateResult = undefined | {
			injector?: Injector;
			entry?: Entry;
		};

		// ------------------------------------------------

		export function load(
			manifest: Manifest,
			directory: string,
		): [Entry, null | Configuration] | null {
			assert_test(Detail.exist_directory(directory), `directory is not found : <${directory}>`);
			let entry: [Entry, null | Configuration] | null = null;
			assert_test(manifest.entry === null || manifest.module.includes(manifest.entry), `entry module is invalid : <${manifest.entry}>`);
			for (let module of manifest.module) {
				let script_name = `script/${module}.js`;
				let script_file = `${directory}/${module}.js`;
				let configuration_file = `${directory}/${module}.json`;
				assert_test(Detail.exist_file(script_file), `module script file not found : <${module}>`);
				let configuration: null | Configuration = null;
				if (Detail.exist_file(configuration_file)) {
					let raw_module_configuration = Detail.read_json(configuration_file);
					assert_test(raw_module_configuration !== null && typeof raw_module_configuration === 'object' && (raw_module_configuration as Object).constructor.name === 'Object', `module configuration must be object : <${module}>`);
					configuration = raw_module_configuration as Configuration;
				}
				let evaluate_result = Detail.evaluate(script_file, script_name) as EvaluateResult;
				if (evaluate_result !== undefined) {
					if (evaluate_result.injector !== undefined) {
						evaluate_result.injector(configuration);
					}
				}
				if (module === manifest.entry) {
					assert_test(evaluate_result !== undefined && evaluate_result.entry !== undefined, `module is loaded, but entry function is not found : <${module}>`);
					entry = [evaluate_result.entry as Entry, configuration];
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

		// ------------------------------------------------

		export async function internal(
			argument: Array<string>,
		): Promise<string> {
			Detail.output(`TwinStar.ToolKit ~ Core:${Core.Miscellaneous.g_version.value} & Shell:${Core.Miscellaneous.g_context.callback(Core.StringList.value(['host'])).value[0]}:${Core.Miscellaneous.g_context.callback(Core.StringList.value(['version'])).value[0]} & Script:${k_version} ~ ${Core.Miscellaneous.g_context.callback(Core.StringList.value(['system'])).value[0]} & ${Core.Miscellaneous.g_context.callback(Core.StringList.value(['architecture'])).value[0]}`);
			assert_test(argument.length >= 1, `argument too few`);
			// 获取主目录
			let home_path = argument[0];
			home_path = home_path.replaceAll(`\\`, '/');
			if (/^\.{1,2}[\/]/.test(home_path)) {
				home_path = `${Detail.get_working_directory()}/${home_path}`;
			}
			Home.path = home_path;
			// 设置模块主目录（ES）
			Core.Miscellaneous.g_context.query_module_home().value = Home.script();
			// 加载子模块
			let timer_begin = Date.now();
			let entry = ModuleLoader.load(g_module_manifest, Home.script());
			let timer_end = Date.now();
			// 执行模块入口函数
			try {
				Console.success(los('main:module_load_finish'), [
					los('main:module_load_duration', ((timer_end - timer_begin) / 1000).toFixed(3)),
				]);
				Home.initialize();
				entry?.[0](entry[1], argument.slice(1));
			} catch (e) {
				Console.error_of(e);
				Console.pause();
			}
			// 释放资源
			Home.deinitialize();
			g_thread_manager.resize(0);
			return '';
		}

		export function external(
			data: {
				argument: Array<string>;
				result: string | undefined;
				error: any | undefined;
			},
		): void {
			data.result = '';
			internal(data.argument)
				.then(
					(value) => {
						data.result = value;
					},
					(reason) => {
						data.error = reason;
					},
				);
			return;
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
		`utility/ProcessHelper`,
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
		`Support/PopCap/ResourceStreamBundle/UnpackLenient`,
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
		`Entry/method/texture.transformation`,
		`Entry/method/texture.atlas`,
		`Entry/method/wwise.media`,
		`Entry/method/wwise.sound_bank`,
		`Entry/method/marmalade.dzip`,
		`Entry/method/popcap.zlib`,
		`Entry/method/popcap.crypt_data`,
		`Entry/method/popcap.reflection_object_notation`,
		`Entry/method/popcap.texture`,
		`Entry/method/popcap.u_texture`,
		`Entry/method/popcap.sexy_texture`,
		`Entry/method/popcap.animation`,
		`Entry/method/popcap.re_animation`,
		`Entry/method/popcap.particle`,
		`Entry/method/popcap.trail`,
		`Entry/method/popcap.particle_effect`,
		`Entry/method/popcap.render_effect`,
		`Entry/method/popcap.character_font_widget_2`,
		`Entry/method/popcap.package`,
		`Entry/method/popcap.resource_stream_group`,
		`Entry/method/popcap.resource_stream_bundle`,
		`Entry/method/popcap.resource_stream_bundle_patch`,
		`Entry/method/pvz2.text_table`,
		`Entry/method/pvz2.remote_android_helper`,
	],
	entry: `Entry/Entry`,
};

(TwinStar.Script.Main.external);