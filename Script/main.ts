namespace Twinning.Script {

	// #region version

	export const k_version = '144';

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

		export function get_workspace(
		): string {
			return Kernel.Process.get_workspace().value;
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
			if (shell_name === 'assistant' || shell_name === 'assistant_plus') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['send_message', 'verbosity', title, ...description]));
			}
			return;
		}

		// #endregion

	}

	// ----------------

	export namespace MainScript {

		// #region variable

		const k_partition: Array<string> = [
			`utility/timer`,
			`utility/convert_helper`,
			`utility/check_helper`,
			`utility/text_generator`,
			`utility/virtual_terminal_sequence`,
			`utility/command_line_reader`,
			`utility/xml`,
			`utility/byte_list_view`,
			`utility/byte_stream_view`,
			`utility/kernel_x`,
			`utility/shell`,
			`utility/storage_helper`,
			`utility/thread_manager`,
			`utility/process_helper`,
			`utility/console`,
			`utility/language`,
			`utility/home_path`,
			`utility/android_helper`,
			`support/atlas/pack`,
			`support/atlas/pack_automatic`,
			`support/marmalade/dzip/pack_automatic`,
			`support/wwise/media/common`,
			`support/wwise/media/encode`,
			`support/wwise/media/decode`,
			`support/popcap/reflection_object_notation/decode_lenient`,
			`support/popcap/texture/encoding`,
			`support/popcap/animation/convert/common`,
			`support/popcap/animation/convert/flash/common`,
			`support/popcap/animation/convert/flash/from`,
			`support/popcap/animation/convert/flash/to`,
			`support/popcap/animation/convert/flash/source_manager`,
			`support/popcap/package/pack_automatic`,
			`support/popcap/resource_stream_bundle/unpack_lenient`,
			`support/pvz2/text_table/convert`,
			`support/pvz2/json_generic_getter`,
			`support/pvz2/resource_manifest/common`,
			`support/pvz2/resource_manifest/new_type_object_notation/common`,
			`support/pvz2/resource_manifest/new_type_object_notation/decode`,
			`support/pvz2/resource_manifest/new_type_object_notation/encode`,
			`support/pvz2/regular_resource_manifest/common`,
			`support/pvz2/regular_resource_manifest/convert`,
			`support/pvz2/package_project/common`,
			`support/pvz2/package_project/setting`,
			`support/pvz2/package_project/diagnose`,
			`support/pvz2/package_project/transpile`,
			`support/pvz2/package_project/compile`,
			`support/pvz2/package_project/link`,
			`support/pvz2/package_project/parse`,
			`support/pvz2/remote_project/execute`,
			`support/pvz2/resource_convert`,
			`support/kairosoft/game/encrypt_record`,
			`support/kairosoft/game/modify_program`,
			`executor/generic`,
			`executor/typical`,
			`executor/implement/common.utility`,
			`executor/implement/data.hash`,
			`executor/implement/data.encoding`,
			`executor/implement/data.encryption`,
			`executor/implement/data.compression`,
			`executor/implement/data.differentiation`,
			`executor/implement/text.json`,
			`executor/implement/text.xml`,
			`executor/implement/texture.transformation`,
			`executor/implement/texture.encoding`,
			`executor/implement/texture.atlas`,
			`executor/implement/wwise.media`,
			`executor/implement/wwise.sound_bank`,
			`executor/implement/marmalade.dzip`,
			`executor/implement/popcap.zlib`,
			`executor/implement/popcap.crypt_data`,
			`executor/implement/popcap.reflection_object_notation`,
			`executor/implement/popcap.texture`,
			`executor/implement/popcap.u_texture`,
			`executor/implement/popcap.sexy_texture`,
			`executor/implement/popcap.animation`,
			`executor/implement/popcap.re_animation`,
			`executor/implement/popcap.particle`,
			`executor/implement/popcap.trail`,
			`executor/implement/popcap.particle_effect`,
			`executor/implement/popcap.render_effect`,
			`executor/implement/popcap.character_font_widget_2`,
			`executor/implement/popcap.package`,
			`executor/implement/popcap.resource_stream_group`,
			`executor/implement/popcap.resource_stream_bundle`,
			`executor/implement/popcap.resource_stream_bundle_patch`,
			`executor/implement/pvz2.text_table`,
			`executor/implement/pvz2.resource_manifest`,
			`executor/implement/pvz2.package_project`,
			`executor/implement/pvz2.remote_project`,
			`executor/implement/kairosoft.game`,
			`executable/script_console`,
			`executable/compare_language_file`,
			`executable/compute_wwise_short_id`,
			`executable/extract_rsb_from_pvz2_cn_apk_directory`,
			`runner/runner`,
			`setting`,
		];

		// ----------------

		export let g_setting: Setting = undefined!;

		export let g_thread_manager: ThreadManager = undefined!;

		// #endregion

		// #region life

		async function run(
			argument: Array<string>,
		): Promise<Array<string>> {
			Detail.output_text([
				`Twinning`,
				` ~ Kernel:${Kernel.Miscellaneous.g_version.value}`,
				` & Shell:${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['version'])).value[0]}.${Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['name'])).value[0]}`,
				` & Script:${k_version}`,
				` ~ ${Kernel.Miscellaneous.g_system.value}.${Kernel.Miscellaneous.g_architecture.value}`,
			].join(''), argument);
			if (argument.length < 1) {
				throw new Error(`argument too few`);
			}
			let load_timer_begin = Date.now();
			// parse home path
			let home_path = argument[0].replaceAll(`\\`, '/');
			if (/^\.{1,2}[\/]/.test(home_path)) {
				home_path = `${Detail.get_workspace()}/${home_path}`;
			}
			// set module home
			Kernel.Miscellaneous.g_context.query_module_home().value = `${home_path}/script`;
			// load partition
			for (let partition of k_partition) {
				let script_name = `script/${partition}.js`;
				Detail.evaluate_script(`${home_path}/${script_name}`, script_name);
			}
			// load setting
			let setting_data = Detail.read_json(`${home_path}/script/configuration/setting.json`);
			if (setting_data === null || typeof setting_data !== 'object' || setting_data.constructor.name !== 'Object') {
				throw new Error(`setting data invalid`);
			}
			g_setting = setting_data as Setting;
			// initialize resource
			g_thread_manager = new ThreadManager();
			HomePath.initialize(home_path);
			update_setting(g_setting, g_setting);
			// activate executor
			let executor_implement = Executor.Implement as Record<string, Record<string, Executor.TypicalMethodImplementeModule>>;
			for (let major_name of Object.keys(executor_implement)) {
				let major_module = executor_implement[major_name];
				for (let minor_name of Object.keys(major_module)) {
					let minor_module = major_module[minor_name];
					minor_module.activate();
				}
			}
			let load_timer_end = Date.now();
			// execute runner
			let result = '';
			try {
				Console.success(los('main:load_finish'), [
					los('main:load_duration', ((load_timer_end - load_timer_begin) / 1000).toFixed(3)),
				]);
				result = Runner.run(argument.slice(1));
			}
			catch (e) {
				Console.error_of(e);
				throw '';
			}
			// finalize resource
			g_thread_manager.resize(0, null);
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