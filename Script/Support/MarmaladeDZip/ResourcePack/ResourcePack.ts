/** Marmalade-DZip资源打包 */
namespace TwinStar.Support.MarmaladeDZip.ResourcePack {

	// ------------------------------------------------

	/**
	 * 将资源目录内的所有文件打包为Marmalade-DZip数据包
	 * @param resource_directory 资源目录
	 * @param version_number 版本
	 * @returns 数据包数据
	 */
	export function pack(
		resource_directory: string,
		version_number: [0n][number],
	): [Core.ByteArray, Core.Size] {
		let version_c = Core.Tool.Marmalade.DZip.Version.value({ number: version_number });
		let resource_list = CoreX.FileSystem.list_file(resource_directory);
		let manifest_js: Core.Tool.Marmalade.DZip.Manifest.JS_N.Package = {
			resource: {},
		};
		let data_size_bound = 0;
		data_size_bound += 4; // magic identifier
		data_size_bound += 9; // archive setting
		data_size_bound += 4; // chunk setting
		data_size_bound += 1; // empty string
		for (let resource of resource_list) {
			manifest_js.resource[resource] = {
				chunk: [{ flag: 'copy_coded' }],
			};
			let resource_size = CoreX.FileSystem.size_file(`${resource_directory}/${resource}`);
			data_size_bound += (2 + resource.length) + 6 + 16 + Number(resource_size); // path string + resource information + chunk information + resource data
		}
		Console.notify('i', localized(`打包开始`), [localized(`共 {} 个资源文件`, resource_list.length)]);
		let data = Core.ByteArray.allocate(Core.Size.value(BigInt(data_size_bound)));
		let stream = Core.ByteStreamView.look(data.view());
		let manifest = Core.Tool.Marmalade.DZip.Manifest.Package.json(Core.JSON.Value.value(manifest_js), version_c);
		Core.Tool.Marmalade.DZip.Pack.process_package(stream, manifest, Core.Path.value(resource_directory), version_c);
		Console.notify('s', localized(`打包完成`), []);
		return [data, stream.position()];
	}

	// ------------------------------------------------

}