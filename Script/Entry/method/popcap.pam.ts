/**
 * + popcap.pam.encode PopCap-PAM 编码
 * + popcap.pam.decode PopCap-PAM 解码
 * + popcap.pam.convert.flash.from PopCap-PAM JSON转换至Flash
 * + popcap.pam.convert.flash.to PopCap-PAM JSON转换自Flash
 * + popcap.pam.convert.flash.resize PopCap-PAM Flash图像分辨率调整
 * + popcap.pam.convert.flash.link_media PopCap-PAM Flash创建图像文件链接
 * + popcap.pam.encode.batch [批处理] PopCap-PAM 编码
 * + popcap.pam.decode.batch [批处理] PopCap-PAM 解码
 * + popcap.pam.convert.flash.from.batch [批处理] PopCap-PAM JSON转换至Flash
 * + popcap.pam.convert.flash.to.batch [批处理] PopCap-PAM JSON转换自Flash
 * + popcap.pam.convert.flash.resize.batch [批处理] PopCap-PAM Flash图像分辨率调整
 * + popcap.pam.convert.flash.link_media.batch [批处理] PopCap-PAM Flash创建图像文件链接
 */
namespace TwinKleS.Entry.method.popcap.pam {

	// ------------------------------------------------

	type Config = {
		encode_buffer_size: Argument.Request<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.pam.encode',
				description: 'PopCap-PAM 编码',
				worker(a: Entry.CFSA & {
					manifest_file: Argument.Require<string>;
					data_file: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					let buffer_size: bigint;
					{
						manifest_file = Argument.require(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Argument.request(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.pam)(\.json))?$/i, '.pam')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : `版本不受支持`),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.PAM.encode_fs(data_file, manifest_file, { number: version_number }, buffer_size);
					Console.notify('s', `执行成功`, [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_number: '?input',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.pam.decode',
				description: 'PopCap-PAM 解码',
				worker(a: Entry.CFSA & {
					data_file: Argument.Require<string>;
					manifest_file: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					{
						data_file = Argument.require(
							'数据文件', '',
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						manifest_file = Argument.request(
							'清单文件', '',
							a.manifest_file,
							(value) => (value),
							() => (data_file.replace(/((\.pam))?$/i, '.pam.json')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : `版本不受支持`),
						);
					}
					CoreX.Tool.PopCap.PAM.decode_fs(data_file, manifest_file, { number: version_number });
					Console.notify('s', `执行成功`, [`${manifest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					manifest_file: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.pam.convert.flash.from',
				description: 'PopCap-PAM JSON转换至Flash',
				worker(a: Entry.CFSA & {
					raw_file: Argument.Require<string>;
					ripe_directory: Argument.Request<string, true>;
				}) {
					let raw_file: string;
					let ripe_directory: string;
					{
						raw_file = Argument.require(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_directory = Argument.request(
							'成品目录', '',
							a.ripe_directory,
							(value) => (value),
							() => (raw_file.replace(/((\.pam)(\.json))?$/i, '.pam.xfl')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.PAM.Manifest.JS_N.Animation>(raw_file);
					Support.PopCapAnimation.Convert.Flash.From.from_fsh(raw, ripe_directory);
					Support.PopCapAnimation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
					Support.PopCapAnimation.Convert.Flash.create_xfl_content_file(ripe_directory);
					Console.notify('s', `执行成功`, [`${ripe_directory}`]);
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
				description: 'PopCap-PAM JSON转换自Flash',
				worker(a: Entry.CFSA & {
					ripe_directory: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
				}) {
					let ripe_directory: string;
					let raw_file: string;
					{
						ripe_directory = Argument.require(
							'成品目录', '',
							a.ripe_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file = Argument.request(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							() => (ripe_directory.replace(/((\.pam)(\.xfl))?$/i, '.pam.json')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					Support.PopCapAnimation.Convert.Flash.To.to_fs(raw_file, ripe_directory);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
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
				description: 'PopCap-PAM Flash图像分辨率调整',
				worker(a: Entry.CFSA & {
					directory: Argument.Require<string>;
					resolution: Argument.Request<bigint, false>;
				}) {
					let directory: string;
					let resolution: bigint;
					{
						directory = Argument.require(
							'目录', '',
							a.directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						resolution = Argument.request(
							'图像分辨率', '',
							a.resolution,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (value > 0n ? null : `分辨率应大于零`),
						);
					}
					Support.PopCapAnimation.Convert.Flash.SourceManager.resize_fs(directory, resolution);
					Console.notify('s', `执行成功`, [`${directory}`]);
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
				description: 'PopCap-PAM Flash创建图像文件链接',
				worker(a: Entry.CFSA & {
					directory: Argument.Require<string>;
				}) {
					let directory: string;
					{
						directory = Argument.require(
							'目录', '',
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
					Console.notify('s', `执行成功`, [`${directory}`]);
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
				description: '[批处理] PopCap-PAM 编码',
				worker(a: Entry.CFSA & {
					manifest_file_directory: Argument.Require<string>;
					data_file_directory: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
					buffer_size: Argument.Request<string, false>;
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					let buffer_size: bigint;
					{
						manifest_file_directory = Argument.require(
							'清单文件目录', '',
							a.manifest_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Argument.request(
							'数据文件目录', '',
							a.data_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.pam_encode')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : `版本不受支持`),
						);
						buffer_size = Argument.request(
							'内存缓冲区大小', '',
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
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
					Console.notify('s', `执行成功`, [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_number: '?input',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.decode.batch',
				description: '[批处理] PopCap-PAM 解码',
				worker(a: Entry.CFSA & {
					data_file_directory: Argument.Require<string>;
					manifest_file_directory: Argument.Request<string, true>;
					version_number: Argument.Request<bigint, false>;
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_number: [1n, 2n, 3n, 4n, 5n, 6n][number];
					{
						data_file_directory = Argument.require(
							'数据文件目录', '',
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file_directory = Argument.request(
							'清单文件目录', '',
							a.manifest_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.pam_decode')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_number = Argument.request(
							'版本编号', '',
							a.version_number,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ([1n, 2n, 3n, 4n, 5n, 6n].includes(value) ? null : `版本不受支持`),
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
					Console.notify('s', `执行成功`, [`${manifest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					manifest_file_directory: '?default',
					version_number: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.convert.flash.from.batch',
				description: '[批处理] PopCap-PAM JSON转换至Flash',
				worker(a: Entry.CFSA & {
					raw_file_directory: Argument.Require<string>;
					ripe_directory_directory: Argument.Request<string, true>;
				}) {
					let raw_file_directory: string;
					let ripe_directory_directory: string;
					{
						raw_file_directory = Argument.require(
							'原始文件目录', '',
							a.raw_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						ripe_directory_directory = Argument.request(
							'成品文件目录', '',
							a.ripe_directory_directory,
							(value) => (value),
							() => (raw_file_directory.replace(/$/i, '.pam_xfl_from')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					simple_batch_execute(
						raw_file_directory,
						['file', /.+(\.pam)(\.json)$/i],
						(item) => {
							let raw_file = `${raw_file_directory}/${item}`;
							let ripe_directory = `${ripe_directory_directory}/${item.slice(0, -5)}.xfl`;
							let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.PAM.Manifest.JS_N.Animation>(raw_file);
							Support.PopCapAnimation.Convert.Flash.From.from_fsh(raw, ripe_directory);
							Support.PopCapAnimation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
							Support.PopCapAnimation.Convert.Flash.create_xfl_content_file(ripe_directory);
						},
					);
					Console.notify('s', `执行成功`, [`${ripe_directory_directory}`]);
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
				description: '[批处理] PopCap-PAM JSON转换自Flash',
				worker(a: Entry.CFSA & {
					ripe_directory_directory: Argument.Require<string>;
					raw_file_directory: Argument.Request<string, true>;
				}) {
					let ripe_directory_directory: string;
					let raw_file_directory: string;
					{
						ripe_directory_directory = Argument.require(
							'成品目录目录', '',
							a.ripe_directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						raw_file_directory = Argument.request(
							'原始文件目录', '',
							a.raw_file_directory,
							(value) => (value),
							() => (ripe_directory_directory.replace(/$/i, '.pam_xfl_to')),
							...Argument.requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					simple_batch_execute(
						ripe_directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let ripe_directory = `${ripe_directory_directory}/${item}`;
							let raw_file = `${raw_file_directory}/${item.slice(0, -4)}.json`;
							Support.PopCapAnimation.Convert.Flash.To.to_fs(raw_file, ripe_directory);
						},
					);
					Console.notify('s', `执行成功`, [`${raw_file_directory}`]);
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
				description: '[批处理] PopCap-PAM Flash图像分辨率调整',
				worker(a: Entry.CFSA & {
					directory_directory: Argument.Require<string>;
					resolution: Argument.Request<bigint, false>;
				}) {
					let directory_directory: string;
					let resolution: bigint;
					{
						directory_directory = Argument.require(
							'目录目录', '',
							a.directory_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						resolution = Argument.request(
							'图像分辨率', '',
							a.resolution,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => (value > 0n ? null : `分辨率应大于零`),
						);
					}
					simple_batch_execute(
						directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let directory = `${directory_directory}/${item}`;
							Support.PopCapAnimation.Convert.Flash.SourceManager.resize_fs(directory, resolution);
						},
					);
					Console.notify('s', `执行成功`, [`${directory_directory}`]);
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
				description: '[批处理] PopCap-PAM Flash创建图像文件链接',
				worker(a: Entry.CFSA & {
					directory_directory: Argument.Require<string>;
				}) {
					let directory_directory: string;
					{
						directory_directory = Argument.require(
							'目录目录', '',
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
									//CoreX.FileSystem.create_hard_link(`${media_directory}/${e}`, `${directory}/../${e}`);
									CoreX.FileSystem.rename(`${directory}/../${e}`, `${media_directory}/${e}`);
								});
						},
					);
					Console.notify('s', `执行成功`, [`${directory_directory}`]);
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
	injector: TwinKleS.Entry.method.popcap.pam._injector,
});