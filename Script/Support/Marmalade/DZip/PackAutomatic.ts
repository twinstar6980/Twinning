namespace Twinning.Script.Support.Marmalade.DZip.PackAutomatic {

	// ------------------------------------------------

	export function pack(
		resource_directory: string,
		version_number: [0n][number],
	): [Kernel.ByteArray, Kernel.Size] {
		let version_c = Kernel.Tool.Marmalade.DZip.Version.value({ number: version_number });
		let resource_list = KernelX.Storage.list_file(resource_directory);
		let definition_js: Kernel.Tool.Marmalade.DZip.Definition.JS_N.Package = {
			resource: [],
		};
		let data_size_bound = 0;
		data_size_bound += 4; // magic identifier
		data_size_bound += 9; // archive setting
		data_size_bound += 4; // chunk setting
		data_size_bound += 1; // empty string
		for (let resource of resource_list) {
			definition_js.resource.push({
				path: resource,
				chunk: [{ flag: 'copy_coded' }],
			});
			let resource_size = KernelX.Storage.size_file(`${resource_directory}/${resource}`);
			data_size_bound += (2 + resource.length) + 6 + 16 + Number(resource_size); // path string + resource information + chunk information + resource data
		}
		Console.information(los('support.marmalade.dzip.pack_automatic:start'), [
			los('support.marmalade.dzip.pack_automatic:total', resource_list.length),
		]);
		let data = Kernel.ByteArray.allocate(Kernel.Size.value(BigInt(data_size_bound)));
		let stream = Kernel.ByteStreamView.watch(data.view());
		let definition = Kernel.Tool.Marmalade.DZip.Definition.Package.json(Kernel.JSON.Value.value(definition_js), version_c);
		Kernel.Tool.Marmalade.DZip.Pack.process(stream, definition, Kernel.Path.value(resource_directory), version_c);
		Console.success(los('support.marmalade.dzip.pack_automatic:finish'), [
		]);
		return [data, stream.position()];
	}

	// ------------------------------------------------

}