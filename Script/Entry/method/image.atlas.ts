/**
 * + image.atlas.pack Atlas 打包
 * + image.atlas.unpack Atlas 解包
 * + image.atlas.pack_auto Atlas 自动打包
 */
namespace TwinKleS.Entry.method.image.atlas {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'image.atlas.pack',
				description: 'Atlas 打包',
				worker(a: Entry.CFSA & {
					manifest_file: string;
					sprite_directory: string | '?default' | '?input';
					atlas_file: string | '?default' | '?input';
				}) {
					let manifest_file: string;
					let sprite_directory: string;
					let atlas_file: string;
					{
						manifest_file = a.manifest_file;
						sprite_directory = ArgumentParser.path(a.sprite_directory, {
							input_message: '请输入输入路径',
							default_value: manifest_file.replace(/((\.atlas)(\.json))?$/i, '.sprite'),
							must_exist: true,
							type: 'directory',
						});
						atlas_file = ArgumentParser.path(a.atlas_file, {
							input_message: '请输入输出路径',
							default_value: manifest_file.replace(/((\.atlas)(\.json))?$/i, '.png'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					Support.Atlas.Pack.pack_fs(manifest_file, atlas_file, sprite_directory);
					Output.i(`输出路径：${atlas_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					sprite_directory: '?default',
					atlas_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.atlas)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'image.atlas.unpack',
				description: 'Atlas 解包',
				worker(a: Entry.CFSA & {
					manifest_file: string;
					atlas_file: string | '?default' | '?input';
					sprite_directory: string | '?default' | '?input';
				}) {
					let manifest_file: string;
					let atlas_file: string;
					let sprite_directory: string;
					{
						manifest_file = a.manifest_file;
						atlas_file = ArgumentParser.path(a.atlas_file, {
							input_message: '请输入输入路径',
							default_value: manifest_file.replace(/((\.atlas)(\.json))?$/i, '.png'),
							must_exist: true,
							type: 'file',
						});
						sprite_directory = ArgumentParser.path(a.sprite_directory, {
							input_message: '请输入输出路径',
							default_value: manifest_file.replace(/((\.atlas)(\.json))?$/i, '.sprite'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					Support.Atlas.Pack.unpack_fs(manifest_file, atlas_file, sprite_directory);
					Output.i(`输出路径：${sprite_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					atlas_file: '?default',
					sprite_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.atlas)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'image.atlas.pack_auto',
				description: 'Atlas 自动打包',
				worker(a: Entry.CFSA & {
					sprite_directory: string;
					manifest_file: string | '?default' | '?input';
					atlas_file: string | '?default' | '?input';
				}) {
					let sprite_directory: string;
					let manifest_file: string;
					let atlas_file: string;
					{
						sprite_directory = a.sprite_directory;
						manifest_file = ArgumentParser.path(a.manifest_file, {
							input_message: '请输入输出路径',
							default_value: sprite_directory.replace(/((\.sprite))?$/i, '.atlas.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						atlas_file = ArgumentParser.path(a.atlas_file, {
							input_message: '请输入输出路径',
							default_value: sprite_directory.replace(/((\.sprite))?$/i, '.png'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					Support.Atlas.Pack.pack_auto_fs(manifest_file, atlas_file, sprite_directory);
					Output.i(`输出路径：${atlas_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					sprite_directory: undefined!,
					manifest_file: '?default',
					atlas_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.sprite)$/i]]),
				input_forwarder: 'sprite_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.image.atlas._injector,
});