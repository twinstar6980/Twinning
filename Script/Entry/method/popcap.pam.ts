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
		encode_buffer_size: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.pam.encode',
				description: 'PopCap-PAM 编码',
				worker(a: Entry.CFSA & {
					manifest_file: string;
					data_file: string | '?default' | '?input';
					version_number: bigint | '?input';
					data_buffer_size: string | '?input';
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_number: bigint;
					let data_buffer_size: bigint;
					{
						manifest_file = a.manifest_file;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: manifest_file.replace(/((\.pam)(\.json))?$/i, '.pam'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（1~6）`, Check.enum_checkerx([1n, 2n, 3n, 4n, 5n, 6n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.data_buffer_size === '?input') {
							data_buffer_size = Input.size(`请输入用于保存PAM数据的内存空间大小`)!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					CoreX.Tool.PopCap.PAM.encode_fs(data_file, manifest_file, { number: version_number as any }, data_buffer_size);
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_number: '?input',
					data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.pam)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.pam.decode',
				description: 'PopCap-PAM 解码',
				worker(a: Entry.CFSA & {
					data_file: string;
					manifest_file: string | '?default' | '?input';
					version_number: bigint | '?input';
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_number: bigint;
					{
						data_file = a.data_file;
						manifest_file = ArgumentParser.path(a.manifest_file, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.pam))?$/i, '.pam.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（1~6）`, Check.enum_checkerx([1n, 2n, 3n, 4n, 5n, 6n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					CoreX.Tool.PopCap.PAM.decode_fs(data_file, manifest_file, { number: version_number as any });
					Output.i(`输出路径：${manifest_file}`);
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
					raw_file: string;
					ripe_directory: string | '?default' | '?input';
				}) {
					let raw_file: string;
					let ripe_directory: string;
					{
						raw_file = a.raw_file;
						ripe_directory = ArgumentParser.path(a.ripe_directory, {
							input_message: '请输入输出路径',
							default_value: raw_file.replace(/((\.pam)(\.json))?$/i, '.pam.xfl'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.PAM.Manifest.JS_N.Animation>(raw_file);
					Support.PopCapAnimation.Convert.Flash.From.from_fsh(raw, ripe_directory);
					Support.PopCapAnimation.Convert.Flash.SourceManager.create_fsh(ripe_directory, raw);
					Support.PopCapAnimation.Convert.Flash.create_xfl_content_file(ripe_directory);
					Output.i(`输出路径：${ripe_directory}`);
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
					ripe_directory: string;
					raw_file: string | '?default' | '?input';
				}) {
					let ripe_directory: string;
					let raw_file: string;
					{
						ripe_directory = a.ripe_directory;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_directory.replace(/((\.pam)(\.xfl))?$/i, '.pam.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					Support.PopCapAnimation.Convert.Flash.To.to_fs(raw_file, ripe_directory);
					Output.i(`输出路径：${raw_file}`);
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
					directory: string;
					resolution: bigint | '?input';
				}) {
					let directory: string;
					let resolution: bigint;
					{
						directory = a.directory;
						if (a.resolution === '?input') {
							resolution = Input.integer(`请输入图像分辨率`, (value) => (value > 0n ? null : `分辨率必须大于0`))!;
						} else {
							resolution = a.resolution;
						}
					}
					Support.PopCapAnimation.Convert.Flash.SourceManager.resize_fs(directory, resolution);
					Output.i(`原位输出：${directory}`);
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
					directory: string;
				}) {
					let directory: string;
					{
						directory = a.directory;
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
					Output.i(`原位输出：${directory}`);
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
					manifest_file_directory: string;
					data_file_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
					data_buffer_size: string | '?input';
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_number: bigint;
					let data_buffer_size: bigint;
					{
						manifest_file_directory = a.manifest_file_directory;
						data_file_directory = ArgumentParser.path(a.data_file_directory, {
							input_message: '请输入输出路径',
							default_value: manifest_file_directory.replace(/$/i, '.pam_encode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（1~6）`, Check.enum_checkerx([1n, 2n, 3n, 4n, 5n, 6n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
						if (a.data_buffer_size === '?input') {
							data_buffer_size = Input.size(`请输入用于保存PAM数据的内存空间大小`)!;
						} else {
							data_buffer_size = parse_size_string(a.data_buffer_size);
						}
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(data_buffer_size));
					simple_batch_execute(
						manifest_file_directory,
						['file', /.+(\.pam)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -5)}`;
							CoreX.Tool.PopCap.PAM.encode_fs(data_file, manifest_file, { number: version_number as any }, data_buffer.view());
						},
					);
					Output.i(`输出路径：${data_file_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_number: '?input',
					data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.pam.decode.batch',
				description: '[批处理] PopCap-PAM 解码',
				worker(a: Entry.CFSA & {
					data_file_directory: string;
					manifest_file_directory: string | '?default' | '?input';
					version_number: bigint | '?input';
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_number: bigint;
					{
						data_file_directory = a.data_file_directory;
						manifest_file_directory = ArgumentParser.path(a.manifest_file_directory, {
							input_message: '请输入输出路径',
							default_value: data_file_directory.replace(/$/i, '.pam_decode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.version_number === '?input') {
							version_number = Input.integer(`请输入版本编号（1~6）`, Check.enum_checkerx([1n, 2n, 3n, 4n, 5n, 6n]))!;
						} else {
							version_number = BigInt(a.version_number);
						}
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.pam)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let manifest_file = `${manifest_file_directory}/${item}.json`;
							CoreX.Tool.PopCap.PAM.decode_fs(data_file, manifest_file, { number: version_number as any });
						},
					);
					Output.i(`输出路径：${manifest_file_directory}`);
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
					raw_file_directory: string;
					ripe_directory_directory: string | '?default' | '?input';
				}) {
					let raw_file_directory: string;
					let ripe_directory_directory: string;
					{
						raw_file_directory = a.raw_file_directory;
						ripe_directory_directory = ArgumentParser.path(a.ripe_directory_directory, {
							input_message: '请输入输出路径',
							default_value: raw_file_directory.replace(/$/i, '.pam_xfl_from'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
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
					Output.i(`输出路径：${ripe_directory_directory}`);
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
					ripe_directory_directory: string;
					raw_file_directory: string | '?default' | '?input';
				}) {
					let ripe_directory_directory: string;
					let raw_file_directory: string;
					{
						ripe_directory_directory = a.ripe_directory_directory;
						raw_file_directory = ArgumentParser.path(a.raw_file_directory, {
							input_message: '请输入输出路径',
							default_value: ripe_directory_directory.replace(/$/i, '.pam_xfl_to'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
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
					Output.i(`输出路径：${raw_file_directory}`);
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
					directory_directory: string;
					resolution: bigint | '?input';
				}) {
					let directory_directory: string;
					let resolution: bigint;
					{
						directory_directory = a.directory_directory;
						if (a.resolution === '?input') {
							resolution = Input.integer(`请输入图像分辨率`, (value) => (value > 0n ? null : `分辨率必须大于0`))!;
						} else {
							resolution = a.resolution;
						}
					}
					simple_batch_execute(
						directory_directory,
						['directory', /.+(\.pam)(\.xfl)$/i],
						(item) => {
							let directory = `${directory_directory}/${item}`;
							Support.PopCapAnimation.Convert.Flash.SourceManager.resize_fs(directory, resolution);
						},
					);
					Output.i(`原位输出：${directory_directory}`);
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
					directory_directory: string;
				}) {
					let directory_directory: string;
					{
						directory_directory = a.directory_directory;
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
					Output.i(`原位输出：${directory_directory}`);
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