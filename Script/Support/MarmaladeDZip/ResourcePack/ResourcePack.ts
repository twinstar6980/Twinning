/** Marmalade-DZip资源打包 */
namespace TwinKleS.Support.MarmaladeDZip.ResourcePack {

	// ------------------------------------------------

	/**
	 * 将资源目录内的所有文件打包为Marmalade-DZip数据包
	 * @param resource_directory 资源目录
	 * @param version_number 版本
	 * @returns 数据包数据
	 */
	export function pack(
		resource_directory: string,
		version_number: bigint,
	): [Core.ByteArray, Core.Size] {
		let version_c = Core.Tool.Marmalade.DZip.Version.json(Core.JSON.Value.value({ number: version_number as any }));
		let resource_list = CoreX.FileSystem.list_file(resource_directory);
		Console.notify('i', `共${resource_list.length}个资源文件`, []);
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
		Console.notify('i', `开始打包 ...`, []);
		let data = Core.ByteArray.allocate(Core.Size.value(BigInt(data_size_bound)));
		let stream = Core.ByteStreamView.look(data.view());
		let manifest = Core.Tool.Marmalade.DZip.Manifest.PackageVariant.from_json(Core.JSON.Value.value(manifest_js), version_c);
		Core.Tool.Marmalade.DZip.Pack.process(stream, manifest, Core.Path.value(resource_directory), version_c);
		Console.notify('i', `完成`, []);
		return [data, stream.position()];
	}

	// ------------------------------------------------

}