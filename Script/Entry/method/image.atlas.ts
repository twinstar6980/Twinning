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
					manifest_file: Argument.Require<string>;
					sprite_directory: Argument.Request<string, true>;
					atlas_file: Argument.Request<string, true>;
				}) {
					let manifest_file: string;
					let sprite_directory: string;
					let atlas_file: string;
					{
						manifest_file = Argument.require(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						sprite_directory = Argument.request(
							'精灵目录', '',
							a.sprite_directory,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.sprite')),
							...Argument.requester_for_path('directory', [true]),
						);
						atlas_file = Argument.request(
							'图集文件', '',
							a.atlas_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.png')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.Atlas.Pack.pack_fs(manifest_file, atlas_file, sprite_directory);
					Console.notify('s', `执行成功`, [`${atlas_file}`]);
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
					manifest_file: Argument.Require<string>;
					atlas_file: Argument.Request<string, true>;
					sprite_directory: Argument.Request<string, true>;
				}) {
					let manifest_file: string;
					let atlas_file: string;
					let sprite_directory: string;
					{
						manifest_file = Argument.require(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						atlas_file = Argument.request(
							'图集文件', '',
							a.atlas_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.png')),
							...Argument.requester_for_path('file', [true]),
						);
						sprite_directory = Argument.request(
							'精灵目录', '',
							a.sprite_directory,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.sprite')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.Atlas.Pack.unpack_fs(manifest_file, atlas_file, sprite_directory);
					Console.notify('s', `执行成功`, [`${sprite_directory}`]);
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
					sprite_directory: Argument.Require<string>;
					manifest_file: Argument.Request<string, true>;
					atlas_file: Argument.Request<string, true>;
				}) {
					let sprite_directory: string;
					let manifest_file: string;
					let atlas_file: string;
					{
						sprite_directory = Argument.require(
							'精灵目录', '',
							a.sprite_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file = Argument.request(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							() => (sprite_directory.replace(/((\.sprite))?$/i, '.atlas.json')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						atlas_file = Argument.request(
							'图集文件', '',
							a.atlas_file,
							(value) => (value),
							() => (sprite_directory.replace(/((\.sprite))?$/i, '.png')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.Atlas.Pack.pack_auto_fs(manifest_file, atlas_file, sprite_directory);
					Console.notify('s', `执行成功`, [`${atlas_file}`]);
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