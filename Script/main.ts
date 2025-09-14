namespace Twinning.Script {

	// #region version

	export const k_version = '141';

	// #endregion

	// #region exception

	export function assert_test(
		condition: boolean,
		expression: string = ``,
	): asserts condition {
		if (!condition) {
			let error = new Error(expression);
			error.name = 'AssertionError';
			throw error;
		}
		return;
	}

	export function generate_exception_message(
		exception: any,
	): [string, Array<string>] {
		let title: string = '';
		let description: Array<string> = [];
		if (exception instanceof Error) {
			if (exception.name === 'NativeError') {
				title = `${exception.name}`;
				description.push(...exception.message.split('\n'));
			}
			else {
				title = `${exception.name}: ${exception.message}`;
			}
			if (exception.stack !== undefined) {
				description.push(...exception.stack.split('\n').slice(0, -1)
					.map((value) => (/^    at (.*) \((.*?)(?:\:(\d+)\:(\d+))?\)$/.exec(value)))
					.filter((value) => (value !== null))
					.map((value) => (`@ ${['native', 'missing', 'null'].includes(value[2]) ? `<${value[2]}>` : value[2]}:${value[3] === undefined ? '?' : value[3]}:${value[4] === undefined ? '?' : value[4]} ${value[1]}`))
				);
			}
		}
		else {
			title = `${exception}`;
		}
		return [title, description];
	}

	// #endregion

	// #region implement

	namespace Detail {

		// #region utility

		export function exist_file(
			target: string,
		): boolean {
			return Kernel.Storage.exist_file(Kernel.Path.value(target)).value;
		}

		export function exist_directory(
			target: string,
		): boolean {
			return Kernel.Storage.exist_directory(Kernel.Path.value(target)).value;
		}

		// ----------------

		export function create_directory(
			target: string,
		): void {
			return Kernel.Storage.create_directory(Kernel.Path.value(target));
		}

		// ----------------

		export function get_working_directory(
		): string {
			return Kernel.Process.get_working_directory().value;
		}

		export function set_working_directory(
			target: string,
		): void {
			return Kernel.Process.set_working_directory(Kernel.Path.value(target));
		}

		// ----------------

		export function read_json<Constraint extends Kernel.JSON.JS_Value>(
			file: string,
		): Constraint {
			let data = Kernel.Storage.read_file(Kernel.Path.value(file));
			let data_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(data.view()));
			let value = Kernel.JSON.Value.default<Constraint>();
			let buffer = Kernel.ByteArray.allocate(Kernel.Size.value(0x400n));
			let buffer_stream = Kernel.CharacterStreamView.watch(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(buffer.view()));
			Kernel.Tool.Data.Serialization.JSON.Read.process(data_stream, value, buffer_stream);
			return value.value;
		}

		// ----------------

		export function evaluate_script(
			script_file: string,
			name: string,
		): any {
			let script = Kernel.Storage.read_file(Kernel.Path.value(script_file));
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Kernel.String.value(name), Kernel.Boolean.value(false));
		}

		// ----------------

		export function output_text(
			title: string,
			description: Array<string>,
		): void {
			let shell_name = Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['name'])).value[0];
			if (shell_name === 'basic') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['output_text', `● ${title}\n`]));
				for (let description_item of description) {
					Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['output_text', `  ${description_item}\n`]));
				}
			}
			if (shell_name === 'assistant' || shell_name === 'assistant.plus') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['send_message', 'verbosity', title, ...description]));
			}
			return;
		}

		// #endregion

	}

	// ----------------

	export namespace HomePath {

		// #region utility

		let g_location: string = undefined!;

		export function of(
			format: string,
		): string {
			return format.replaceAll(/^~(?=([/]|$))/g, g_location);
		}

		// ----------------

		export function script(
		): string {
			return of(`~/script`);
		}

		export function workspace(
		): string {
			return of(`~/workspace`);
		}

		export function temporary(
		): string {
			return of(`~/temporary`);
		}

		// ----------------

		export function new_temporary(
			name: null | string,
			create: null | 'file' | 'directory',
		): string {
			let temporary_name = name !== null ? name : make_date_to_string_simple(new Date());
			let temporary_path = PathUtility.generate_suffix_path(`${temporary()}/${temporary_name}`);
			if (create === 'file') {
				KernelX.Storage.create_file(temporary_path);
			}
			if (create === 'directory') {
				KernelX.Storage.create_directory(temporary_path);
			}
			return temporary_path;
		}

		// ----------------

		export function initialize(
			location: string,
		): void {
			g_location = location;
			Detail.create_directory(workspace());
			Detail.create_directory(temporary());
			Detail.set_working_directory(workspace());
			return;
		}

		// #endregion

	}

	export namespace PartitionLoader {

		// #region utility

		export type Configuration = Record<string, Kernel.JSON.JS_Value>;

		export type Inject = (configuration: null | Configuration) => void;

		// ----------------

		export function load(
			partition_list: Array<string>,
			directory: string,
		): void {
			if (!Detail.exist_directory(directory)) {
				throw new Error(`directory is not found: <${directory}>`);
			}
			for (let partition of partition_list) {
				let script_name = `script/${partition}.js`;
				let script_file = `${directory}/${partition}.js`;
				let configuration_file = `${directory}/${partition}.json`;
				if (!Detail.exist_file(script_file)) {
					throw new Error(`partition script file not found: <${partition}>`);
				}
				let configuration: null | Configuration = null;
				if (Detail.exist_file(configuration_file)) {
					let raw_configuration = Detail.read_json(configuration_file);
					if (raw_configuration === null || typeof raw_configuration !== 'object' || raw_configuration.constructor.name !== 'Object') {
						throw new Error(`partition configuration must be object: <${partition}>`);
					}
					configuration = raw_configuration as Configuration;
				}
				let inject = Detail.evaluate_script(script_file, script_name) as undefined | Inject;
				if (inject !== undefined) {
					inject(configuration);
				}
			}
			return;
		}

		// #endregion

	}

	export namespace MainScript {

		// #region life

		const k_partition_list: Array<string> = [
			`utility/Timer`,
			`utility/TypeUtility`,
			`utility/PathUtility`,
			`utility/Check`,
			`utility/TextGenerator`,
			`utility/VirtualTerminalSequence`,
			`utility/CommandLineReader`,
			`utility/XML`,
			`utility/ByteListView`,
			`utility/ByteStreamView`,
			`utility/KernelX`,
			`utility/Shell`,
			`utility/ThreadManager`,
			`utility/ProcessHelper`,
			`utility/Console`,
			`utility/AndroidHelper`,
			`Language/Language`,
			`Support/Atlas/Pack`,
			`Support/Atlas/PackAutomatic`,
			`Support/Marmalade/DZip/PackAutomatic`,
			`Support/Wwise/Media/common`,
			`Support/Wwise/Media/Encode`,
			`Support/Wwise/Media/Decode`,
			`Support/PopCap/ReflectionObjectNotation/DecodeLenient`,
			`Support/PopCap/Texture/Encoding`,
			`Support/PopCap/Animation/Convert/common`,
			`Support/PopCap/Animation/Convert/Flash/common`,
			`Support/PopCap/Animation/Convert/Flash/From`,
			`Support/PopCap/Animation/Convert/Flash/To`,
			`Support/PopCap/Animation/Convert/Flash/SourceManager`,
			`Support/PopCap/Package/PackAutomatic`,
			`Support/PopCap/ResourceStreamBundle/UnpackLenient`,
			`Support/PvZ2/TextTable/Convert`,
			`Support/PvZ2/JSONGenericGetter`,
			`Support/PvZ2/ResourceManifest/common`,
			`Support/PvZ2/ResourceManifest/NewTypeObjectNotation/common`,
			`Support/PvZ2/ResourceManifest/NewTypeObjectNotation/Decode`,
			`Support/PvZ2/ResourceManifest/NewTypeObjectNotation/Encode`,
			`Support/PvZ2/RegularResourceManifest/common`,
			`Support/PvZ2/RegularResourceManifest/Convert`,
			`Support/PvZ2/PackageProject/common`,
			`Support/PvZ2/PackageProject/Setting`,
			`Support/PvZ2/PackageProject/Diagnose`,
			`Support/PvZ2/PackageProject/Transpile`,
			`Support/PvZ2/PackageProject/Compile`,
			`Support/PvZ2/PackageProject/Link`,
			`Support/PvZ2/PackageProject/Parse`,
			`Support/PvZ2/RemoteProject/Execute`,
			`Support/PvZ2/ResourceConvert`,
			`Support/Kairosoft/Game/EncryptRecord`,
			`Support/Kairosoft/Game/ModifyProgram`,
			`Executor/Generic`,
			`Executor/Typical`,
			`Executor/Implement/common.utility`,
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
			`Executor/Implement/pvz2.package_project`,
			`Executor/Implement/pvz2.remote_project`,
			`Executor/Implement/kairosoft.game`,
			`Runner/Runner`,
		];

		// ----------------

		async function run(
			argument: Array<string>,
		): Promise<Array<string>> {
			Detail.output_text(`Twinning ~ Kernel:${Kernel.Miscellaneous.g_version.value} & Shell:${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['name'])).value[0]}:${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['version'])).value[0]} & Script:${k_version} ~ ${Kernel.Miscellaneous.g_system.value}:${Kernel.Miscellaneous.g_architecture.value}`, argument);
			if (argument.length < 1) {
				throw new Error(`argument too few`);
			}
			// parse home path
			let home_path = argument[0].replaceAll(`\\`, '/');
			if (/^\.{1,2}[\/]/.test(home_path)) {
				home_path = `${Detail.get_working_directory()}/${home_path}`;
			}
			HomePath.initialize(home_path);
			// set module home directory
			Kernel.Miscellaneous.g_context.query_module_home().value = HomePath.script();
			// load script partition
			let load_timer_begin = Date.now();
			PartitionLoader.load(k_partition_list, HomePath.script());
			let load_timer_end = Date.now();
			// execute runner
			let result = '';
			try {
				Console.success(los('main:partition_load_finish'), [
					los('main:partition_load_duration', ((load_timer_end - load_timer_begin) / 1000).toFixed(3)),
				]);
				result = Runner.run(argument.slice(1));
			}
			catch (e) {
				Console.error_of(e);
				throw '';
			}
			// release resource
			ThreadManager.g_global_manager.resize(0);
			return [result];
		}

		export function runWrapper(
			data: {
				argument: Array<string>;
				result: undefined | Array<string>;
				exception: undefined | any;
			},
		): void {
			data.result = [];
			run(data.argument)
				.then(
					(value) => {
						data.result = value;
					},
					(reason) => {
						data.exception = reason;
					},
				);
			return;
		}

		// #endregion

	}

	// #endregion

}

Twinning.Script.MainScript.runWrapper;