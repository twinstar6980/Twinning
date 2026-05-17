namespace Twinning.Script.Executable.DecodeRtonLenient {

	// #region interface

	export function run(
	): void {
		Console.information(los('executable.decode_rton_lenient:please_input_data_file'), [
		]);
		let data_file = Console.path('file', 'input', null, null);
		Console.information(los('executable.decode_rton_lenient:please_input_definition_file'), [
		]);
		let definition_file = Console.path('file', 'output', null, null);
		Console.information(los('executable.decode_rton_lenient:please_input_version_native_string_encoding_use_utf8'), [
		]);
		let version_native_string_encoding_use_utf8 = Console.boolean(null, null);
		Support.Popcap.ReflectionObjectNotation.DecodeLenient.process_fs(data_file, definition_file, {number: 1n, native_string_encoding_use_utf8: version_native_string_encoding_use_utf8});
		Console.success(los('executable.decode_rton_lenient:finish'), [
		]);
		return;
	}

	// #endregion

}
