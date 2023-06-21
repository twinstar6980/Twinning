namespace TwinStar.Script.Support.PopCap.Package.PackAutomatic {

	// ------------------------------------------------

	export function pack(
		resource_directory: string,
		version_number: [0n][number],
		version_compress_resource_data: boolean,
	): [Core.ByteArray, Core.Size] {
		let version_c = Core.Tool.PopCap.Package.Version.value({ number: version_number, compress_resource_data: version_compress_resource_data });
		let resource_list = CoreX.FileSystem.list_file(resource_directory);
		let definition_js: Core.Tool.PopCap.Package.Definition.JS_N.Package = {
			resource: {},
		};
		let data_size_bound = 0;
		data_size_bound += 4; // magic identifier
		data_size_bound += 8; // version number
		for (let resource of resource_list) {
			definition_js.resource[resource] = {
				time: 0n,
			};
			let resource_size = CoreX.FileSystem.size_file(`${resource_directory}/${resource}`);
			data_size_bound += (1 + 1 + resource.length + 4 + 4 + 8) + (Number(resource_size) + 16); // resource information + resource data
		}
		data_size_bound += 1; // list done flag
		Console.information(los('support.popcap.package.pack_automatic:start'), [
			los('support.popcap.package.pack_automatic:total', resource_list.length),
		]);
		let data = Core.ByteArray.allocate(Core.Size.value(BigInt(data_size_bound)));
		let stream = Core.ByteStreamView.watch(data.view());
		let definition = Core.Tool.PopCap.Package.Definition.Package.json(Core.JSON.Value.value(definition_js), version_c);
		Core.Tool.PopCap.Package.Pack.process(stream, definition, Core.Path.value(resource_directory), version_c);
		Console.success(los('support.popcap.package.pack_automatic:finish'), [
		]);
		return [data, stream.position()];
	}

	// ------------------------------------------------

}