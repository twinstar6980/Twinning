namespace Twinning.Script.Executable.UnpackRsbLenient {

	// #region interface

	export function run(
	): void {
		Console.information(los('executable.unpack_rsb_lenient:please_input_data_file'), [
		]);
		let data_file = Console.path('file', 'input', null, null);
		Console.information(los('executable.unpack_rsb_lenient:please_input_bundle_directory'), [
		]);
		let bundle_directory = Console.path('directory', 'output', null, null);
		let definition_file = bundle_directory.join('definition.json');
		let manifest_file = bundle_directory.join('manifest.json');
		let resource_directory = bundle_directory.join('resource');
		Support.Popcap.ResourceStreamBundle.UnpackLenient.process_fs(data_file, definition_file, manifest_file, resource_directory);
		Console.success(los('executable.unpack_rsb_lenient:finish'), [
		]);
		return;
	}

	// #endregion

}
