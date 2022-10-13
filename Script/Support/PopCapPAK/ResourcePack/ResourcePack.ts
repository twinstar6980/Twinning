/** PopCap-PAK资源打包 */
namespace TwinKleS.Support.PopCapPAK.ResourcePack {

	// ------------------------------------------------

	/**
	 * 将资源目录内的所有文件打包为PopCap-PAK数据包
	 * @param resource_directory 资源目录
	 * @returns 数据包数据
	 */
	export function pack(
		resource_directory: string,
		version_number: [0n][number],
		version_compress_resource_data: boolean,
	): [Core.ByteArray, Core.Size] {
		let version_c = Core.Tool.PopCap.PAK.Version.json(Core.JSON.Value.value({ number: version_number, compress_resource_data: version_compress_resource_data }));
		let resource_list = CoreX.FileSystem.list_file(resource_directory);
		let manifest_js: Core.Tool.PopCap.PAK.Manifest.JS_N.Package = {
			resource: {},
		};
		let data_size_bound = 0;
		data_size_bound += 4; // magic identifier
		data_size_bound += 8; // version number
		for (let resource of resource_list) {
			manifest_js.resource[resource] = {
				time: 0n,
			};
			let resource_size = CoreX.FileSystem.size_file(`${resource_directory}/${resource}`);
			data_size_bound += (1 + 1 + resource.length + 4 + 4 + 8) + (Number(resource_size) + 16); // resource information + resource data
		}
		data_size_bound += 1; // list done flag
		Console.notify('i', localized(`打包开始`), [localized(`共 {} 个资源文件`, resource_list.length)]);
		let data = Core.ByteArray.allocate(Core.Size.value(BigInt(data_size_bound)));
		let stream = Core.ByteStreamView.look(data.view());
		let manifest = Core.Tool.PopCap.PAK.Manifest.PackageVariant.from_json(Core.JSON.Value.value(manifest_js), version_c);
		Core.Tool.PopCap.PAK.Pack.process(stream, manifest, Core.Path.value(resource_directory), version_c);
		Console.notify('s', localized(`打包完成`), []);
		return [data, stream.position()];
	}

	// ------------------------------------------------

}