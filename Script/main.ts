namespace Twinning.Script {

	// #region common

	export const k_version = '148';

	// ----------------

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

	// #endregion

	// #region main

	export namespace MainScript {

		// #region utility

		function query_environment(
			name: string,
		): string {
			let value = null as string | null;
			if (name === 'kernel_version') {
				value = Kernel.Miscellaneous.g_version.value;
			}
			if (name === 'shell_name') {
				value = Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['name'])).value[0];
			}
			if (name === 'shell_version') {
				value = Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['version'])).value[0];
			}
			if (name === 'script_version') {
				value = k_version;
			}
			if (name === 'platform_system') {
				value = Kernel.Miscellaneous.g_system.value;
			}
			if (name === 'platform_architecture') {
				value = Kernel.Miscellaneous.g_architecture.value;
			}
			if (value === null) {
				throw new Error(`information name invalid`);
			}
			return value;
		}

		// ----------------

		function get_process_workspace(
		): string {
			return Kernel.Process.get_workspace().value;
		}

		// ----------------

		function set_module_home(
			path: string,
		): void {
			Kernel.Miscellaneous.g_context.query_module_home().value = path;
			return;
		}

		function load_script_file(
			path: string,
			name: string,
		): any {
			let script = Kernel.Storage.read_file(Kernel.Path.value(path));
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Kernel.String.value(name), Kernel.Boolean.value(false));
		}

		// ----------------

		function console_output_text(
			title: string,
			description: Array<string>,
		): void {
			let shell_name = query_environment('shell_name');
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
			`support/popcap/resource_stream_bundle/common`,
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
			`executable/compute_wwise_short_identifier`,
			`executable/extract_rsb_from_pvz2_cn_apk_directory`,
			`runner/runner`,
			`setting`,
		];

		// ----------------

		export let g_setting: Setting = undefined!;

		export let g_thread_manager: ThreadManager = undefined!;

		// #endregion

		// #region life

		export async function run(
			argument: Array<string>,
		): Promise<Array<string>> {
			if (!['basic', 'assistant', 'assistant_plus'].includes(query_environment('shell_name'))) {
				throw new Error(`shell client unsupported`);
			}
			console_output_text([
				`Twinning`,
				` ~ Kernel:${query_environment('kernel_version')}`,
				` & Shell:${query_environment('shell_version')}.${query_environment('shell_name')}`,
				` & Script:${query_environment('script_version')}`,
				` ~ ${query_environment('platform_system')}.${query_environment('platform_architecture')}`,
			].join(''), argument);
			if (argument.length < 1) {
				throw new Error(`argument too few`);
			}
			let load_timer_begin = Date.now();
			// parse home path
			let home_path = argument[0].replaceAll(`\\`, '/');
			if (/^\.{1,2}[\/]/.test(home_path)) {
				home_path = `${get_process_workspace()}/${home_path}`;
			}
			// set module home
			set_module_home(`${home_path}/script`);
			// load partition
			for (let partition of k_partition) {
				let partition_name = `script/${partition}.js`;
				load_script_file(`${home_path}/${partition_name}`, partition_name);
			}
			// initialize resource
			g_thread_manager = new ThreadManager();
			HomePath.initialize(home_path);
			// load setting
			let setting_data = KernelX.JSON.read_fs_js(HomePath.of(`~/script/configuration/setting.json`));
			if (!CheckHelper.is_object_of_object(setting_data)) {
				throw new Error(`setting data invalid`);
			}
			g_setting = setting_data as Setting;
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
			let result = null as null | string;
			try {
				Console.success(los('main:load_finish'), [
					los('main:load_duration', ((load_timer_end - load_timer_begin) / 1000).toFixed(3)),
				]);
				result = Runner.run(argument.slice(1));
			}
			catch (e) {
				Console.error_of(e);
			}
			// finalize resource
			g_thread_manager.resize(0, null);
			// check result
			if (result === null) {
				throw '';
			}
			return [result];
		}

		// #endregion

	}

	// ----------------

	export function main(
		data: {
			argument: Array<string>;
			result: undefined | Array<string>;
			exception: undefined | any;
		},
	): void {
		data.result = [];
		MainScript.run(data.argument)
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

Twinning.Script.main;