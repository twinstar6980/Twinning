namespace TwinStar.Script.Entry.method.image.atlas {

	// ------------------------------------------------

	// pack
	// unpack
	// pack_automatic

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'image.atlas.pack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file: Executor.RequireArgument<string>;
					sprite_directory: Executor.RequestArgument<string, true>;
					atlas_file: Executor.RequestArgument<string, true>;
				}) {
					let manifest_file: string;
					let sprite_directory: string;
					let atlas_file: string;
					{
						manifest_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						sprite_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'sprite_directory'),
							a.sprite_directory,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.sprite')),
							(initial) => (Console.path('directory', [true], null, null, initial)),
						);
						atlas_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'atlas_file'),
							a.atlas_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.atlas.png')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					Support.Atlas.Pack.pack_fs(manifest_file, atlas_file, sprite_directory);
					Console.message('s', los(`执行成功`), [`${atlas_file}`]);
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
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file: Executor.RequireArgument<string>;
					atlas_file: Executor.RequestArgument<string, true>;
					sprite_directory: Executor.RequestArgument<string, true>;
				}) {
					let manifest_file: string;
					let atlas_file: string;
					let sprite_directory: string;
					{
						manifest_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						atlas_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'atlas_file'),
							a.atlas_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.atlas.png')),
							(initial) => (Console.path('file', [true], null, null, initial)),
						);
						sprite_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'sprite_directory'),
							a.sprite_directory,
							(value) => (value),
							() => (manifest_file.replace(/((\.atlas)(\.json))?$/i, '.sprite')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					Support.Atlas.Pack.unpack_fs(manifest_file, atlas_file, sprite_directory);
					Console.message('s', los(`执行成功`), [`${sprite_directory}`]);
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
				id: 'image.atlas.pack_automatic',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					sprite_directory: Executor.RequireArgument<string>;
					manifest_file: Executor.RequestArgument<string, true>;
					atlas_file: Executor.RequestArgument<string, true>;
				}) {
					let sprite_directory: string;
					let manifest_file: string;
					let atlas_file: string;
					{
						sprite_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'sprite_directory'),
							a.sprite_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							() => (sprite_directory.replace(/((\.sprite))?$/i, '.atlas.json')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						atlas_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'atlas_file'),
							a.atlas_file,
							(value) => (value),
							() => (sprite_directory.replace(/((\.sprite))?$/i, '.atlas.png')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					Support.Atlas.Pack.pack_automatic_fs(manifest_file, atlas_file, sprite_directory);
					Console.message('s', los(`执行成功`), [`${atlas_file}`]);
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
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.image.atlas._injector,
});