/**
 * + popcap.pam.encode
 * + popcap.pam.decode
 * + popcap.pam.convert.flash.from
 * + popcap.pam.convert.flash.to
 * + popcap.pam.convert.flash.resize
 * + popcap.pam.convert.flash.link_media
 * + popcap.pam.encode.batch
 * + popcap.pam.decode.batch 
 * + popcap.pam.convert.flash.from.batch
 * + popcap.pam.convert.flash.to.batch
 * + popcap.pam.convert.flash.resize.batch
 * + popcap.pam.convert.flash.link_media.batch
 */
namespace TwinStar.Script.Entry.method.popcap.pam {

	// ------------------------------------------------

	type Config = {
		version_number: Executor.RequestArgument<bigint, false>;
		encode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.pam.encode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					let buffer_size: bigint;
					{
						manifest_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.pam)(\.json))?$/i, '.pam')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([0n, null, [1n, ''], [2n, ''], [3n, ''], [4n, ''], [5n, ''], [6n, '']], null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : los(`版本不受支持`)),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.PAM.encode_fs(data_file, manifest_file, { number: version_number }, buffer_size);
					Console.notify('s', los(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_number: config.version_number,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.pam.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					manifest_file: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					{
						data_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						manifest_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							() => (data_file.replace(/((\.pam))?$/i, '.pam.json')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([0n, null, [1n, ''], [2n, ''], [3n, ''], [4n, ''], [5n, ''], [6n, '']], null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : los(`版本不受支持`)),
						);
					}
					CoreX.Tool.PopCap.PAM.decode_fs(data_file, manifest_file, { number: version_number });
					Console.notify('s', los(`执行成功`), [`${manifest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					manifest_file: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.pam.convert.flash.from',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file: Executor.RequireArgument<string>;
					ripe_directory: Executor.RequestArgument<string, true>;
				}) {
					let raw_file: string;
					let ripe_directory: string;
					{
						raw_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_directory'),
							a.ripe_directory,
							(value) => (value),
							() => (raw_file.replace(/((\.pam)(\.json))?$/i, '.pam.xfl')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.PAM.Manifest.JS_N.Animation>(raw_file);
					Support.PopCap.PAM.Convert.Flash.From.from_fsh(raw, ripe_directory);
					Support.PopCap.PAM.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
					Support.PopCap.PAM.Convert.Flash.create_xfl_content_file(ripe_directory);
					Console.notify('s', los(`执行成功`), [`${ripe_directory}`]);
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
				id: 'popcap.pam.convert.flash.to',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_directory: Executor.RequireArgument<string>;
					raw_file: Executor.RequestArgument<string, true>;
				}) {
					let ripe_directory: string;
					let raw_file: string;
					{
						ripe_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'ripe_directory'),
							a.ripe_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'raw_file'),
							a.raw_file,
							(value) => (value),
							() => (ripe_directory.replace(/((\.pam)(\.xfl))?$/i, '.pam.json')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.PopCap.PAM.Convert.Flash.To.to_fs(raw_file, ripe_directory);
					Console.notify('s', los(`执行成功`), [`${raw_file}`]);
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
				id: 'popcap.pam.convert.flash.resize',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					directory: Executor.RequireArgument<string>;
					resolution: Executor.RequestArgument<bigint, false>;
				}) {
					let directory: string;
					let resolution: bigint;
					{
						directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'directory'),
							a.directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						resolution = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'resolution'),
							a.resolution,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (value > 0n ? null : los(`分辨率应大于零`)),
						);
					}
					Support.PopCap.PAM.Convert.Flash.SourceManager.resize_fs(directory, resolution);
					Console.notify('s', los(`执行成功`), [`${directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					directory: undefined!,
					resolution: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pam)(\.xfl)$/i]]),
				input_forwarder: 'directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.convert.flash.link_media',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					directory: Executor.RequireArgument<string>;
				}) {
					let directory: string;
					{
						directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'directory'),
							a.directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
					}
					let media_directory = `${directory}/LIBRARY/media`;
					if (CoreX.FileSystem.exist(media_directory)) {
						CoreX.FileSystem.remove(media_directory);
					}
					CoreX.FileSystem.create_directory(media_directory);
					CoreX.FileSystem.list_file(`${directory}/..`, 1n)
						.filter((e) => (/.+(\.png)$/i.test(e)))
						.forEach((e) => {
							CoreX.FileSystem.create_hard_link(`${media_directory}/${e}`, `${directory}/../${e}`);
						});
					Console.notify('s', los(`执行成功`), [`${directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					directory: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', /.+(\.pam)(\.xfl)$/i]]),
				input_forwarder: 'directory',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.pam.encode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					let buffer_size: bigint;
					{
						manifest_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.pam_encode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([0n, null, [1n, ''], [2n, ''], [3n, ''], [4n, ''], [5n, ''], [6n, '']], null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : los(`版本不受支持`)),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						manifest_file_directory,
						['file', /.+(\.pam)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -5)}`;
							CoreX.Tool.PopCap.PAM.encode_fs(data_file, manifest_file, { number: version_number }, data_buffer.view());
						},
					);
					Console.notify('s', los(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_number: config.version_number,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					manifest_file_directory: Executor.RequestArgument<string, true>;
					version_number: Executor.RequestArgument<bigint, false>;
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					{
						data_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.pam_decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							() => (Console.option([0n, null, [1n, ''], [2n, ''], [3n, ''], [4n, ''], [5n, ''], [6n, '']], null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : los(`版本不受支持`)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.pam)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let manifest_file = `${manifest_file_directory}/${item}.json`;
							CoreX.Tool.PopCap.PAM.decode_fs(data_file, manifest_file, { number: version_number });
						},
					);
					Console.notify('s', los(`执行成功`), [`${manifest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					manifest_file_directory: '?default',
					version_number: config.version_number,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.convert.flash.from.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					raw_file_directory: Executor.RequireArgument<string>;
					ripe_directory_directory: Executor.RequestArgument<string, true>;
				}) {
					let raw_file_directory: string;
					let ripe_directory_directory: string;
					{
						raw_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						ripe_directory_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'ripe_directory_directory'),
							a.ripe_directory_directory,
							(value) => (value),
							() => (raw_file_directory.replace(/$/i, '.pam_xfl_from')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					simple_batch_execute(
						raw_file_directory,
						['file', /.+(\.pam)(\.json)$/i],
						(item) => {
							let raw_file = `${raw_file_directory}/${item}`;
							let ripe_directory = `${ripe_directory_directory}/${item.slice(0, -5)}.xfl`;
							let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.PAM.Manifest.JS_N.Animation>(raw_file);
							Support.PopCap.PAM.Convert.Flash.From.from_fsh(raw, ripe_directory);
							Support.PopCap.PAM.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
							Support.PopCap.PAM.Convert.Flash.create_xfl_content_file(ripe_directory);
						},
					);
					Console.notify('s', los(`执行成功`), [`${ripe_directory_directory}`]);
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
				id: 'popcap.pam.convert.flash.to.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					ripe_directory_directory: Executor.RequireArgument<string>;
					raw_file_directory: Executor.RequestArgument<string, true>;
				}) {
					let ripe_directory_directory: string;
					let raw_file_directory: string;
					{
						ripe_directory_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'ripe_directory_directory'),
							a.ripe_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'raw_file_directory'),
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_directory_directory.replace(/$/i, '.pam_xfl_to')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					simple_batch_execute(
						ripe_directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let ripe_directory = `${ripe_directory_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.json`;
							Support.PopCap.PAM.Convert.Flash.To.to_fs(raw_file, ripe_directory);
						},
					);
					Console.notify('s', los(`执行成功`), [`${raw_file_directory}`]);
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
				id: 'popcap.pam.convert.flash.resize.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					directory_directory: Executor.RequireArgument<string>;
					resolution: Executor.RequestArgument<bigint, false>;
				}) {
					let directory_directory: string;
					let resolution: bigint;
					{
						directory_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'directory_directory'),
							a.directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						resolution = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'resolution'),
							a.resolution,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (value > 0n ? null : los(`分辨率应大于零`)),
						);
					}
					simple_batch_execute(
						directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let directory = `${directory_directory}/${item}`;
							Support.PopCap.PAM.Convert.Flash.SourceManager.resize_fs(directory, resolution);
						},
					);
					Console.notify('s', los(`执行成功`), [`${directory_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					directory_directory: undefined!,
					resolution: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'directory_directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.convert.flash.link_media.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					directory_directory: Executor.RequireArgument<string>;
				}) {
					let directory_directory: string;
					{
						directory_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'directory_directory'),
							a.directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
					}
					simple_batch_execute(
						directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let directory = `${directory_directory}/${item}`;
							let media_directory = `${directory}/LIBRARY/media`;
							if (CoreX.FileSystem.exist(media_directory)) {
								CoreX.FileSystem.remove(media_directory);
							}
							CoreX.FileSystem.create_directory(media_directory);
							CoreX.FileSystem.list_file(`${directory}/..`, 1n)
								.filter((e) => (/.+(\.png)$/i.test(e)))
								.forEach((e) => {
									CoreX.FileSystem.create_hard_link(`${media_directory}/${e}`, `${directory}/../${e}`);
								});
						},
					);
					Console.notify('s', los(`执行成功`), [`${directory_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					directory_directory: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'directory_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.pam._injector,
});