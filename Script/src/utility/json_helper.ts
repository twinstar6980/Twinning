namespace Twinning.Script.JsonHelper {

	// #region encoding

	/** result is a view of buffer */
	export function encode_data<TValue extends Kernel.Json.JS_Value>(
		value: TValue,
		disable_array_trailing_comma: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_array_trailing_comma,
		disable_array_line_breaking: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_array_line_breaking,
		disable_object_trailing_comma: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_object_trailing_comma,
		disable_object_line_breaking: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_object_line_breaking,
		data_buffer: Kernel.CharacterListView = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(KernelX.g_common_buffer.view()),
	): ArrayBuffer {
		return KernelX.Tool.Data.Serialization.Json.write(Kernel.Json.Value.value<TValue>(value), disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
	}

	export function decode_data<TValue extends Kernel.Json.JS_Value>(
		data: ArrayBuffer,
	): TValue {
		return KernelX.Tool.Data.Serialization.Json.read<TValue>(data).value;
	}

	// ----------------

	export function encode_text<TValue extends Kernel.Json.JS_Value>(
		value: TValue,
		disable_array_trailing_comma: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_array_trailing_comma,
		disable_array_line_breaking: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_array_line_breaking,
		disable_object_trailing_comma: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_object_trailing_comma,
		disable_object_line_breaking: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_object_line_breaking,
		data_buffer: Kernel.CharacterListView = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(KernelX.g_common_buffer.view()),
	): string {
		return KernelX.Tool.Data.Serialization.Json.write_s(Kernel.Json.Value.value<TValue>(value), disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
	}

	export function decode_text<TValue extends Kernel.Json.JS_Value>(
		text: string,
	): TValue {
		return KernelX.Tool.Data.Serialization.Json.read_s<TValue>(text).value;
	}

	// ----------------

	export function encode_file<TValue extends Kernel.Json.JS_Value>(
		path: StoragePath,
		value: TValue,
		disable_array_trailing_comma: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_array_trailing_comma,
		disable_array_line_breaking: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_array_line_breaking,
		disable_object_trailing_comma: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_object_trailing_comma,
		disable_object_line_breaking: boolean = KernelX.Tool.Data.Serialization.Json.g_format.disable_object_line_breaking,
		data_buffer: Kernel.CharacterListView = Kernel.Miscellaneous.cast_ByteListView_to_CharacterListView(KernelX.g_common_buffer.view()),
	): void {
		return KernelX.Tool.Data.Serialization.Json.write_fs(path, Kernel.Json.Value.value<TValue>(value), disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, data_buffer);
	}

	export function decode_file<TValue extends Kernel.Json.JS_Value>(
		path: StoragePath,
	): TValue {
		return KernelX.Tool.Data.Serialization.Json.read_fs<TValue>(path).value;
	}

	// #endregion

}
