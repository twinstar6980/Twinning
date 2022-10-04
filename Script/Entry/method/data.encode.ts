/**
 * + data.encode.base64.encode Base64 编码
 * + data.encode.base64.decode Base64 解码
 */
namespace TwinKleS.Entry.method.data.encode {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.encode.base64.encode',
				description: 'Base64 编码',
				worker(a: Entry.CFSA & {
					raw_file: string;
					ripe_file: string | '?default' | '?input';
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = a.raw_file;
						ripe_file = ArgumentParser.path(a.ripe_file, {
							input_message: '请输入输出路径',
							default_value: raw_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					CoreX.Tool.Data.Encode.Base64.encode_fs(raw_file, ripe_file);
					Console.notify('s', `输出路径：${ripe_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					raw_file: undefined!,
					ripe_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'raw_file',
			}),
			Executor.method_of({
				id: 'data.encode.base64.decode',
				description: 'Base64 解码',
				worker(a: Entry.CFSA & {
					ripe_file: string;
					raw_file: string | '?default' | '?input';
				}) {
					let ripe_file: string;
					let raw_file: string;
					{
						ripe_file = a.ripe_file;
						raw_file = ArgumentParser.path(a.raw_file, {
							input_message: '请输入输出路径',
							default_value: ripe_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					CoreX.Tool.Data.Encode.Base64.decode_fs(ripe_file, raw_file);
					Console.notify('s', `输出路径：${raw_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					ripe_file: undefined!,
					raw_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'ripe_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.data.encode._injector,
});