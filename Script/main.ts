namespace TwinStar.Script {

	// ------------------------------------------------

	export const k_version = 83;

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
			return Kernel.FileSystem.exist_file(Kernel.Path.value(path)).value;
		}

		export function exist_directory(
			path: string,
		): boolean {
			return Kernel.FileSystem.exist_directory(Kernel.Path.value(path)).value;
		}

		// ------------------------------------------------

		export function read_json<ConstraintT extends Kernel.JSON.JS_Value>(
			file: string,
		): ConstraintT {
			let data = Kernel.FileSystem.read_file(Kernel.Path.value(file));
			let stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
			let json = Kernel.JSON.Value.default<ConstraintT>();
			Kernel.Tool.Data.Serialization.JSON.Read.process(stream, json);
			return json.value;
		}

		// ------------------------------------------------

		export function get_working_directory(
		): string {
			return Kernel.Process.get_working_directory().value;
		}

		// ------------------------------------------------

		export function evaluate(
			script_file: string,
			name: string,
		): any {
			let script = Kernel.FileSystem.read_file(Kernel.Path.value(script_file));
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Kernel.String.value(name), Kernel.Boolean.value(false));
		}

		// ------------------------------------------------

		export function output(
			message: string,
		): void {
			var host = Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['host'])).value[0];
			if (host === 'cli') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['output', `● ${message}\n`]));
			}
			if (host === 'gui') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['output_message', 'v', `${message}`]));
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
			KernelX.FileSystem.create_directory(trash_sub_direcotry);
			KernelX.FileSystem.rename(path, `${trash_sub_direcotry}/${PathUtility.name(path)}`);
			return;
		}

		export function new_temporary(
		): string {
			let temporary_sub_directory = `${temporary()}/${date_to_simple_string(new Date())}.${make_prefix_padded_string((Math.random() * 10000).toFixed(0), '0', 4)}`;
			KernelX.FileSystem.create_directory(temporary_sub_directory);
			return temporary_sub_directory;
		}

		// ------------------------------------------------

		export function initialize(
		): void {
			KernelX.FileSystem.create_directory(workspace());
			KernelX.FileSystem.create_directory(trash());
			KernelX.FileSystem.create_directory(temporary());
			KernelX.Process.set_working_directory(workspace());
			return;
		}

		export function deinitialize(
		): void {
			let temporary_sub_directory_list = KernelX.FileSystem.list(temporary());
			for (let temporary_sub_directory of temporary_sub_directory_list) {
				KernelX.FileSystem.remove(`${temporary()}/${temporary_sub_directory}`);
			}
			return;
		}

		// ------------------------------------------------

	}

	export namespace PartitionLoader {

		// ------------------------------------------------

		export type Configuration = Record<string, Kernel.JSON.JS_Value>;

		export type Injector = (configuration: null | Configuration) => void;

		// ------------------------------------------------

		export function load(
			partition_list: Array<string>,
			directory: string,
		): void {
			assert_test(Detail.exist_directory(directory), `directory is not found : <${directory}>`);
			for (let partition of partition_list) {
				let script_name = `script/${partition}.js`;
				let script_file = `${directory}/${partition}.js`;
				let configuration_file = `${directory}/${partition}.json`;
				assert_test(Detail.exist_file(script_file), `partition script file not found : <${partition}>`);
				let configuration: null | Configuration = null;
				if (Detail.exist_file(configuration_file)) {
					let raw_configuration = Detail.read_json(configuration_file);
					assert_test(raw_configuration !== null && typeof raw_configuration === 'object' && (raw_configuration as Object).constructor.name === 'Object', `partition configuration must be object : <${partition}>`);
					configuration = raw_configuration as Configuration;
				}
				let injector = Detail.evaluate(script_file, script_name) as undefined | Injector;
				if (injector !== undefined) {
					injector(configuration);
				}
			}
			return;
		}

		// ------------------------------------------------

	}

	// ------------------------------------------------

	export namespace Main {

		// ------------------------------------------------

		let g_partition_list: Array<string> = [
			`utility/Timer`,
			`utility/TypeUtility`,
			`utility/PathUtility`,
			`utility/Check`,
			`utility/TextGenerator`,
			`utility/VirtualTerminalSequence`,
			`utility/XML`,
			`utility/ByteListView`,
			`utility/KernelX`,
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
			`Executor/Generic`,
			`Executor/Typical`,
			`Executor/Implement/js`,
			`Executor/Implement/data.hash`,
			`Executor/Implement/data.encoding`,
			`Executor/Implement/data.encryption`,
			`Executor/Implement/data.compression`,
			`Executor/Implement/data.differentiation`,
			`Executor/Implement/text.json`,
			`Executor/Implement/text.xml`,
			`Executor/Implement/texture.transformation`,
			`Executor/Implement/texture.encoding`,
			`Executor/Implement/texture.atlas`,
			`Executor/Implement/wwise.media`,
			`Executor/Implement/wwise.sound_bank`,
			`Executor/Implement/marmalade.dzip`,
			`Executor/Implement/popcap.zlib`,
			`Executor/Implement/popcap.crypt_data`,
			`Executor/Implement/popcap.reflection_object_notation`,
			`Executor/Implement/popcap.texture`,
			`Executor/Implement/popcap.u_texture`,
			`Executor/Implement/popcap.sexy_texture`,
			`Executor/Implement/popcap.animation`,
			`Executor/Implement/popcap.re_animation`,
			`Executor/Implement/popcap.particle`,
			`Executor/Implement/popcap.trail`,
			`Executor/Implement/popcap.particle_effect`,
			`Executor/Implement/popcap.render_effect`,
			`Executor/Implement/popcap.character_font_widget_2`,
			`Executor/Implement/popcap.package`,
			`Executor/Implement/popcap.resource_stream_group`,
			`Executor/Implement/popcap.resource_stream_bundle`,
			`Executor/Implement/popcap.resource_stream_bundle_patch`,
			`Executor/Implement/pvz2.text_table`,
			`Executor/Implement/pvz2.resource_manifest`,
			`Executor/Implement/pvz2.remote_android_helper`,
			`Entry/Entry`,
		];

		// ------------------------------------------------

		async function internal(
			argument: Array<string>,
		): Promise<string> {
			Detail.output(`TwinStar.ToolKit ~ Kernel:${Kernel.Miscellaneous.g_version.value} & Shell:${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['host'])).value[0]}:${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['version'])).value[0]} & Script:${k_version} ~ ${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['system'])).value[0]} & ${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['architecture'])).value[0]}`);
			assert_test(argument.length >= 1, `argument too few`);
			// 获取主目录
			let home_path = argument[0].replaceAll(`\\`, '/');
			if (/^\.{1,2}[\/]/.test(home_path)) {
				home_path = `${Detail.get_working_directory()}/${home_path}`;
			}
			Home.path = home_path;
			// 设置模块主目录（ES）
			Kernel.Miscellaneous.g_context.query_module_home().value = Home.script();
			// 加载脚本分区
			let timer_begin = Date.now();
			PartitionLoader.load(g_partition_list, Home.script());
			let timer_end = Date.now();
			// 执行模块入口函数
			try {
				Console.success(los('main:partition_load_finish'), [
					los('main:partition_load_duration', ((timer_end - timer_begin) / 1000).toFixed(3)),
				]);
				Home.initialize();
				Entry.entry(argument.slice(1));
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

TwinStar.Script.Main.external;