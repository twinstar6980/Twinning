namespace TwinStar.Script.Entry.method.popcap.animation {

	// ------------------------------------------------

	// encode *
	// decode *
	// convert.flash.from *
	// convert.flash.to *
	// convert.flash.resize *
	// convert.flash.link_media *

	type Configuration = {
		version_number: Executor.RequestArgument<bigint, false>;
		encode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.animation.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						manifest_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.pam)(\.json))?$/i, '.pam')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.Animation.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.Animation.encode_fs(data_file, manifest_file, { number: version_number as any }, buffer_size);
					Console.message('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_number: configuration.version_number,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.animation.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					manifest_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_number: bigint;
					{
						data_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						manifest_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							() => (data_file.replace(/((\.pam))?$/i, '.pam.json')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.Animation.VersionNumberE), null, null, initial)),
						);
					}
					CoreX.Tool.PopCap.Animation.decode_fs(data_file, manifest_file, { number: version_number as any });
					Console.message('s', los(`执行成功`), [`${manifest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					manifest_file: '?default',
					version_number: configuration.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.from',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_directory: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_directory: string;
					{
						raw_file = Executor.require_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_directory'),
							a.ripe_directory,
							(value) => (value),
							() => (raw_file.replace(/((\.pam)(\.json))?$/i, '.pam.xfl')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.Animation.Manifest.JS_N.Animation>(raw_file);
					Support.PopCap.Animation.Convert.Flash.From.from_fsh(raw, ripe_directory);
					Support.PopCap.Animation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
					Support.PopCap.Animation.Convert.Flash.create_xfl_content_file(ripe_directory);
					Console.message('s', los(`执行成功`), [`${ripe_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)(\.json)$/i]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.to',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_directory: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
				}) {
					let ripe_directory: string;
					let raw_file: string;
					{
						ripe_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'ripe_directory'),
							a.ripe_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_directory.replace(/((\.pam)(\.xfl))?$/i, '.pam.json')),
							(initial) => (Console.path('file', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					Support.PopCap.Animation.Convert.Flash.To.to_fs(raw_file, ripe_directory);
					Console.message('s', los(`执行成功`), [`${raw_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_directory: undefined!,
					raw_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pam)(\.xfl)$/i]]),
				input_forwarder: 'ripe_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.resize',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					target_directory: Executor.RequireArgument<string>;
					resolution: Executor.RequestArgument<bigint, false>;
				}) {
					let target_directory: string;
					let resolution: bigint;
					{
						target_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'target_directory'),
							a.target_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						resolution = Executor.request_argument(
							Executor.query_argument_name(this.id, 'resolution'),
							a.resolution,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (value > 0n ? null : los(`分辨率应大于零`)), initial)),
						);
					}
					Support.PopCap.Animation.Convert.Flash.SourceManager.resize_fs(target_directory, resolution);
					Console.message('s', los(`执行成功`), [`${target_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					target_directory: undefined!,
					resolution: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pam)(\.xfl)$/i]]),
				input_forwarder: 'target_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.link_media',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					target_directory: Executor.RequireArgument<string>;
				}) {
					let target_directory: string;
					{
						target_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'target_directory'),
							a.target_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
					}
					let media_directory = `${target_directory}/LIBRARY/media`;
					if (CoreX.FileSystem.exist(media_directory)) {
						CoreX.FileSystem.remove(media_directory);
					}
					CoreX.FileSystem.create_directory(media_directory);
					CoreX.FileSystem.list_file(`${target_directory}/..`, 1n)
						.filter((e) => (/.+(\.png)$/i.test(e)))
						.forEach((e) => {
							CoreX.FileSystem.create_hard_link(`${media_directory}/${e}`, `${target_directory}/../${e}`);
						});
					Console.message('s', los(`执行成功`), [`${target_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					target_directory: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pam)(\.xfl)$/i]]),
				input_forwarder: 'target_directory',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.animation.encode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_number: bigint;
					let buffer_size: bigint;
					{
						manifest_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.encode')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.Animation.VersionNumberE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						manifest_file_directory,
						['file', /.+(\.pam)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -5)}`;
							CoreX.Tool.PopCap.Animation.encode_fs(data_file, manifest_file, { number: version_number as any }, data_buffer.view());
						},
					);
					Console.message('s', los(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_number: configuration.version_number,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					manifest_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_number: bigint;
					{
						data_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.generate_discretized_integer_option(CoreX.Tool.PopCap.Animation.VersionNumberE), null, null, initial)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.pam)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let manifest_file = `${manifest_file_directory}/${item}.json`;
							CoreX.Tool.PopCap.Animation.decode_fs(data_file, manifest_file, { number: version_number as any });
						},
					);
					Console.message('s', los(`执行成功`), [`${manifest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					manifest_file_directory: '?default',
					version_number: configuration.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.from.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file_directory: Executor.RequireArgument<string>;
					ripe_directory_directory: Executor.RequestArgument<string, true>;
				}) {
					let raw_file_directory: string;
					let ripe_directory_directory: string;
					{
						raw_file_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						ripe_directory_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'ripe_directory_directory'),
							a.ripe_directory_directory,
							(value) => (value),
							() => (raw_file_directory.replace(/$/i, '.convert_flash_from')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					simple_batch_execute(
						raw_file_directory,
						['file', /.+(\.pam)(\.json)$/i],
						(item) => {
							let raw_file = `${raw_file_directory}/${item}`;
							let ripe_directory = `${ripe_directory_directory}/${item.slice(0, -5)}.xfl`;
							let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.Animation.Manifest.JS_N.Animation>(raw_file);
							Support.PopCap.Animation.Convert.Flash.From.from_fsh(raw, ripe_directory);
							Support.PopCap.Animation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
							Support.PopCap.Animation.Convert.Flash.create_xfl_content_file(ripe_directory);
						},
					);
					Console.message('s', los(`执行成功`), [`${ripe_directory_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file_directory: undefined!,
					ripe_directory_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'raw_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.to.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_directory_directory: Executor.RequireArgument<string>;
					raw_file_directory: Executor.RequestArgument<string, true>;
				}) {
					let ripe_directory_directory: string;
					let raw_file_directory: string;
					{
						ripe_directory_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'ripe_directory_directory'),
							a.ripe_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_directory_directory.replace(/$/i, '.convert_flash_to')),
							(initial) => (Console.path('directory', [false, a.fs_tactic_if_exist], null, null, initial)),
						);
					}
					simple_batch_execute(
						ripe_directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let ripe_directory = `${ripe_directory_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.json`;
							Support.PopCap.Animation.Convert.Flash.To.to_fs(raw_file, ripe_directory);
						},
					);
					Console.message('s', los(`执行成功`), [`${raw_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_directory_directory: undefined!,
					raw_file_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'ripe_directory_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.resize.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					target_directory_directory: Executor.RequireArgument<string>;
					resolution: Executor.RequestArgument<bigint, false>;
				}) {
					let target_directory_directory: string;
					let resolution: bigint;
					{
						target_directory_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'target_directory_directory'),
							a.target_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						resolution = Executor.request_argument(
							Executor.query_argument_name(this.id, 'resolution'),
							a.resolution,
							(value) => (value),
							null,
							(initial) => (Console.integer(null, (value) => (value > 0n ? null : los(`分辨率应大于零`)), initial)),
						);
					}
					simple_batch_execute(
						target_directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let target_directory = `${target_directory_directory}/${item}`;
							Support.PopCap.Animation.Convert.Flash.SourceManager.resize_fs(target_directory, resolution);
						},
					);
					Console.message('s', los(`执行成功`), [`${target_directory_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					target_directory_directory: undefined!,
					resolution: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'target_directory_directory',
			}),
			Executor.method_of({
				id: 'popcap.animation.convert.flash.link_media.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CFSA & {
					target_directory_directory: Executor.RequireArgument<string>;
				}) {
					let target_directory_directory: string;
					{
						target_directory_directory = Executor.require_argument(
							Executor.query_argument_name(this.id, 'target_directory_directory'),
							a.target_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
					}
					simple_batch_execute(
						target_directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let target_directory = `${target_directory_directory}/${item}`;
							let media_directory = `${target_directory}/LIBRARY/media`;
							if (CoreX.FileSystem.exist(media_directory)) {
								CoreX.FileSystem.remove(media_directory);
							}
							CoreX.FileSystem.create_directory(media_directory);
							CoreX.FileSystem.list_file(`${target_directory}/..`, 1n)
								.filter((e) => (/.+(\.png)$/i.test(e)))
								.forEach((e) => {
									CoreX.FileSystem.create_hard_link(`${media_directory}/${e}`, `${target_directory}/../${e}`);
								});
						},
					);
					Console.message('s', los(`执行成功`), [`${target_directory_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					target_directory_directory: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'target_directory_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.animation._injector,
});