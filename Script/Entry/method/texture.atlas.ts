namespace TwinStar.Script.Entry.method.texture.atlas {

	// ------------------------------------------------

	// pack
	// unpack
	// pack_automatic

	type Configuration = {
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'texture.atlas.pack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					definition_file: Executor.Argument<string, false>;
					sprite_directory: Executor.Argument<string, true>;
					atlas_file: Executor.Argument<string, true>;
				}) {
					let definition_file: string;
					let sprite_directory: string;
					let atlas_file: string;
					{
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						sprite_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'sprite_directory'),
							a.sprite_directory,
							(value) => (value),
							() => (definition_file.replace(/((\.atlas)(\.json))?$/i, '.sprite')),
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						atlas_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'atlas_file'),
							a.atlas_file,
							(value) => (value),
							() => (definition_file.replace(/((\.atlas)(\.json))?$/i, '.atlas.png')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					Support.Atlas.Pack.pack_fs(definition_file, atlas_file, sprite_directory);
					return [`${atlas_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					definition_file: '?input',
					sprite_directory: '?default',
					atlas_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.atlas)(\.json)$/i]]),
				input_forwarder: 'definition_file',
			}),
			Executor.method_of({
				id: 'texture.atlas.unpack',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					definition_file: Executor.Argument<string, false>;
					atlas_file: Executor.Argument<string, true>;
					sprite_directory: Executor.Argument<string, true>;
				}) {
					let definition_file: string;
					let atlas_file: string;
					let sprite_directory: string;
					{
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						atlas_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'atlas_file'),
							a.atlas_file,
							(value) => (value),
							() => (definition_file.replace(/((\.atlas)(\.json))?$/i, '.atlas.png')),
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						sprite_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'sprite_directory'),
							a.sprite_directory,
							(value) => (value),
							() => (definition_file.replace(/((\.atlas)(\.json))?$/i, '.sprite')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					Support.Atlas.Pack.unpack_fs(definition_file, atlas_file, sprite_directory);
					return [`${sprite_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					definition_file: '?input',
					atlas_file: '?default',
					sprite_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.atlas)(\.json)$/i]]),
				input_forwarder: 'definition_file',
			}),
			Executor.method_of({
				id: 'texture.atlas.pack_automatic',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					sprite_directory: Executor.Argument<string, false>;
					definition_file: Executor.Argument<string, true>;
					atlas_file: Executor.Argument<string, true>;
				}) {
					let sprite_directory: string;
					let definition_file: string;
					let atlas_file: string;
					{
						sprite_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'sprite_directory'),
							a.sprite_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							() => (sprite_directory.replace(/((\.sprite))?$/i, '.atlas.json')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						atlas_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'atlas_file'),
							a.atlas_file,
							(value) => (value),
							() => (sprite_directory.replace(/((\.sprite))?$/i, '.atlas.png')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
					}
					Support.Atlas.Pack.pack_automatic_fs(definition_file, atlas_file, sprite_directory);
					return [`${atlas_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					sprite_directory: '?input',
					definition_file: '?default',
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
	injector: TwinStar.Script.Entry.method.texture.atlas._injector,
});