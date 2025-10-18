namespace Twinning.Script {

	// #region utility

	export type Setting = {
		byte_stream_use_big_endian: boolean;
		common_buffer_size: string;
		json_format_disable_array_trailing_comma: boolean;
		json_format_disable_array_line_breaking: boolean;
		json_format_disable_object_trailing_comma: boolean;
		json_format_disable_object_line_breaking: boolean;
		thread_limit: bigint;
		external_program_path: Record<string, null | string>;
		console_basic_disable_virtual_terminal_sequence: boolean;
		language: string;
		executor_typical_method_disable_name_filter: boolean;
		executor_typical_method_configuration: Record<string, Record<string, Executor.TypicalMethodConfiguration>>;
		executor_pvz2_resource_convert_ptx_format_map_list: Record<string, Support.PvZ2.ResourceConvert.PTXFormatMap>;
		executor_pvz2_package_project_conversion_setting: Support.PvZ2.PackageProject.ConversionSetting;
		command_notification_time_limit: null | bigint;
	};

	// ----------------

	export function update_setting(
		data: Partial<Setting>,
		store: Setting,
	): void {
		let action: { [Key in keyof Setting]: (value: Setting[Key]) => void; } = {
			byte_stream_use_big_endian: (value) => {
				Kernel.Miscellaneous.g_context.query_byte_stream_use_big_endian().value = value;
			},
			common_buffer_size: (value) => {
				KernelX.g_common_buffer.allocate(Kernel.Size.value(parse_size_from_string(value)));
			},
			json_format_disable_array_trailing_comma: (value) => {
				KernelX.JSON.g_format.disable_array_trailing_comma = value;
			},
			json_format_disable_array_line_breaking: (value) => {
				KernelX.JSON.g_format.disable_array_line_breaking = value;
			},
			json_format_disable_object_trailing_comma: (value) => {
				KernelX.JSON.g_format.disable_object_trailing_comma = value;
			},
			json_format_disable_object_line_breaking: (value) => {
				KernelX.JSON.g_format.disable_object_line_breaking = value;
			},
			thread_limit: (value) => {
				MainScript.g_thread_manager.resize(Number(value), null);
			},
			external_program_path: (value) => {
				ProcessHelper.g_program_path_map = value;
			},
			console_basic_disable_virtual_terminal_sequence: (value) => {
				Console.g_basic_disable_virtual_terminal_sequence = value;
			},
			language: (value) => {
				Language.imbue(KernelX.JSON.read_fs_js(HomePath.of(`~/script/configuration/language/${value}.json`)) as Language.StringMap);
			},
			executor_typical_method_disable_name_filter: (value) => {
				Executor.g_typical_method_disable_name_filter = value;
			},
			executor_typical_method_configuration: (value) => {
				Executor.g_typical_method_configuration = value;
			},
			executor_pvz2_resource_convert_ptx_format_map_list: (value) => {
			},
			executor_pvz2_package_project_conversion_setting: (value) => {
			},
			command_notification_time_limit: (value) => {
				Runner.g_command_notification_time_limit = value;
			},
		};
		for (let key in data) {
			assert_test(store.hasOwnProperty(key));
			let value = (data as any)[key];
			(action as any)[key](value);
			(store as any)[key] = value;
		}
		return;
	}

	// #endregion

}