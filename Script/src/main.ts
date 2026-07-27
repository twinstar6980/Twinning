namespace Twinning.Script {

	// #region common

	export declare const k_version: string;

	// ----------------

	export class UnreachableException extends Error {

		// #region constructor

		// ----------------

		public constructor(
		) {
			super();
			this.name = 'UnreachableException';
			this.message = ``;
			this.stack = this.stack!.substring(this.stack!.indexOf('\n') + 1);
			return;
		}

		// #endregion

	}

	export class UnimplementedException extends Error {

		// #region constructor

		// ----------------

		public constructor(
		) {
			super();
			this.name = 'UnimplementedException';
			this.message = ``;
			this.stack = this.stack!.substring(this.stack!.indexOf('\n') + 1);
			return;
		}

		// #endregion

	}

	export class UnsupportedException extends Error {

		// #region constructor

		// ----------------

		public constructor(
		) {
			super();
			this.name = 'UnsupportedException';
			this.message = ``;
			this.stack = this.stack!.substring(this.stack!.indexOf('\n') + 1);
			return;
		}

		// #endregion

	}

	export class TerminateException extends Error {

		// #region constructor

		// ----------------

		public constructor(
		) {
			super();
			this.name = 'TerminateException';
			this.message = ``;
			this.stack = this.stack!.substring(this.stack!.indexOf('\n') + 1);
			return;
		}

		// #endregion

	}

	export class AssertionException extends Error {

		// #region constructor

		// ----------------

		public constructor(
			expression: string,
		) {
			super();
			this.name = 'AssertionException';
			this.message = `${expression}`;
			this.stack = this.stack!.substring(this.stack!.indexOf('\n') + 1);
			return;
		}

		// #endregion

	}

	// ----------------

	export function assert_test(
		condition: boolean,
		expression: string = '',
	): asserts condition {
		if (!condition) {
			throw new AssertionException(expression);
		}
		return;
	}

	// #endregion

	// #region main

	export namespace MainScript {

		// #region utility

		function query_context(
			name: string,
		): string {
			let value = null as string | null;
			if (name === 'kernel_version') {
				value = Kernel.Miscellaneous.g_version.value;
			}
			if (name === 'shell_name') {
				value = Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['query_context', 'name'])).value[0];
			}
			if (name === 'shell_version') {
				value = Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['query_context', 'version'])).value[0];
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
				throw new Error(`invalid name`);
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
			let script_path = Kernel.Path.value(path);
			let script = Kernel.ByteArray.allocate(Kernel.Storage.size_file(script_path));
			Kernel.Storage.read_file(script_path, Kernel.Size.value(0n), script.view());
			return Kernel.Miscellaneous.g_context.evaluate(Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(script.view()), Kernel.String.value(name), Kernel.Boolean.value(false));
		}

		// ----------------

		function console_output_text(
			title: string,
			description: Array<string>,
		): void {
			let shell_name = query_context('shell_name');
			if (shell_name === 'basic') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['output_text', `● ${title}\n`]));
				for (let description_item of description) {
					Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['output_text', `  ${description_item}\n`]));
				}
			}
			if (shell_name === 'assistant') {
				Kernel.Miscellaneous.g_context.callback(Kernel.StringList.value(['send_message', 'verbosity', title, ...description]));
			}
			return;
		}

		// #endregion

		// #region variable

		const k_partition: Array<string> = [
			`utility/finalizer`,
			`utility/timer`,
			`utility/convert_helper`,
			`utility/check_helper`,
			`utility/text_generator`,
			`utility/virtual_terminal_sequence`,
			`utility/command_line_reader`,
			`utility/json_helper`,
			`utility/xml_helper`,
			`utility/byte_list_view`,
			`utility/byte_stream_view`,
			`utility/kernel_x`,
			`utility/shell`,
			`utility/thread_manager`,
			`utility/terminate_helper`,
			`utility/storage_size`,
			`utility/storage_path`,
			`utility/storage_helper`,
			`utility/process_helper`,
			`utility/assembly_helper`,
			`utility/external_helper`,
			`utility/android_helper`,
			`utility/developer_helper`,
			`utility/console`,
			`utility/language`,
			`utility/home_path`,
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
			`support/popcap/pvz2/text_table/convert`,
			`support/popcap/pvz2/json_generic_getter`,
			`support/popcap/pvz2/resource_manifest/common`,
			`support/popcap/pvz2/resource_manifest/new_type_object_notation/common`,
			`support/popcap/pvz2/resource_manifest/new_type_object_notation/decode`,
			`support/popcap/pvz2/resource_manifest/new_type_object_notation/encode`,
			`support/popcap/pvz2/regular_resource_manifest/common`,
			`support/popcap/pvz2/regular_resource_manifest/convert`,
			`support/popcap/pvz2/package_project/common`,
			`support/popcap/pvz2/package_project/setting`,
			`support/popcap/pvz2/package_project/transpile`,
			`support/popcap/pvz2/package_project/compile`,
			`support/popcap/pvz2/package_project/link`,
			`support/popcap/pvz2/package_project/parse`,
			`support/popcap/pvz2/remote_project/execute`,
			`support/popcap/pvz2/resource_convert`,
			`support/kairosoft/game/program/modify`,
			`support/kairosoft/game/record/encrypt`,
			`support/nitrome/twin_shot_deluxe/record/encode`,
			`support/nitrome/twin_shot_deluxe/record/decode`,
			`executor/generic`,
			`executor/typical`,
			`executor/implementation/common.utility`,
			`executor/implementation/data.hashing`,
			`executor/implementation/data.encoding`,
			`executor/implementation/data.encryption`,
			`executor/implementation/data.compression`,
			`executor/implementation/data.differentiation`,
			`executor/implementation/notation.json`,
			`executor/implementation/notation.xml`,
			`executor/implementation/texture.transformation`,
			`executor/implementation/texture.encoding`,
			`executor/implementation/texture.compression`,
			`executor/implementation/texture.atlas`,
			`executor/implementation/wwise.media`,
			`executor/implementation/wwise.sound_bank`,
			`executor/implementation/marmalade.dzip`,
			`executor/implementation/popcap.zlib`,
			`executor/implementation/popcap.crypt_data`,
			`executor/implementation/popcap.reflection_object_notation`,
			`executor/implementation/popcap.texture`,
			`executor/implementation/popcap.u_texture`,
			`executor/implementation/popcap.sexy_texture`,
			`executor/implementation/popcap.animation`,
			`executor/implementation/popcap.re_animation`,
			`executor/implementation/popcap.particle`,
			`executor/implementation/popcap.trail`,
			`executor/implementation/popcap.particle_effect`,
			`executor/implementation/popcap.render_effect`,
			`executor/implementation/popcap.character_font_widget_2`,
			`executor/implementation/popcap.package`,
			`executor/implementation/popcap.resource_stream_group`,
			`executor/implementation/popcap.resource_stream_bundle`,
			`executor/implementation/popcap.resource_stream_bundle_patch`,
			`executor/implementation/popcap.pvz2.text_table`,
			`executor/implementation/popcap.pvz2.resource_manifest`,
			`executor/implementation/popcap.pvz2.package_project`,
			`executor/implementation/popcap.pvz2.remote_project`,
			`executor/implementation/kairosoft.game`,
			`executor/implementation/nitrome.twin_shot_deluxe`,
			`executor/implementation/developer.utility`,
			`executable/script_console`,
			`executable/compare_language_file`,
			`executable/compute_wwise_short_identifier`,
			`executable/extract_rsb_from_pvz2cn_apk_directory`,
			`executable/decode_rton_lenient`,
			`executable/unpack_rsb_lenient`,
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
			if (!['basic', 'assistant'].includes(query_context('shell_name'))) {
				throw new Error(`shell client unsupported`);
			}
			console_output_text([
				`Twinning`,
				` ~ Kernel:${query_context('kernel_version')}`,
				` & Shell:${query_context('shell_version')}.${query_context('shell_name')}`,
				` & Script:${query_context('script_version')}`,
				` ~ ${query_context('platform_system')}.${query_context('platform_architecture')}`,
			].join(''), argument);
			if (argument.length < 1) {
				throw new Error(`argument too few`);
			}
			let load_timer_begin = Date.now();
			// parse home path
			let home_path = argument[0]
				.replaceAll(`\\`, `/`)
				.replaceAll(/^(?=\.{1,2}(?:\/|$))/g, `${get_process_workspace()}/`);
			// set module home
			set_module_home(`${home_path}/script`);
			// load partition
			for (let partition of k_partition) {
				let partition_name = `script/${partition}.js`;
				load_script_file(`${home_path}/${partition_name}`, partition_name);
			}
			// resource
			{
				g_thread_manager = new ThreadManager();
				HomePath.initialize(home_path);
			}
			using resource_finalizer = new Finalizer(() => {
				g_thread_manager.resize(0, null);
			});
			// load setting
			let setting_data = JsonHelper.decode_file(HomePath.script().join('configuration').join('setting.json'));
			if (!CheckHelper.is_object_of_object(setting_data)) {
				throw new Error(`setting data invalid`);
			}
			g_setting = setting_data as Setting;
			update_setting(g_setting, g_setting);
			// activate executor
			let activate_executor = (current_module: Executor.TypicalMethodModule) => {
				if (current_module.activate !== undefined) {
					current_module = current_module as Executor.TypicalMethodImplementation;
					current_module.activate();
				}
				else {
					current_module = current_module as {[key: string]: Executor.TypicalMethodModule};
					for (let child_name in current_module) {
						activate_executor(current_module[child_name]);
					}
				}
				return;
			}
			activate_executor(Executor.Implementation as Executor.TypicalMethodModule);
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
				TerminateHelper.rethrow(e);
				Console.error_of(e);
			}
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
