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
					raw_file: Argument.Require<string>;
					ripe_file: Argument.Request<string, true>;
				}) {
					let raw_file: string;
					let ripe_file: string;
					{
						raw_file = Argument.require(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						ripe_file = Argument.request(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							() => (raw_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Encode.Base64.encode_fs(raw_file, ripe_file);
					Console.notify('s', `执行成功`, [`${ripe_file}`]);
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
					ripe_file: Argument.Require<string>;
					raw_file: Argument.Request<string, true>;
				}) {
					let ripe_file: string;
					let raw_file: string;
					{
						ripe_file = Argument.require(
							'成品文件', '',
							a.ripe_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						raw_file = Argument.request(
							'原始文件', '',
							a.raw_file,
							(value) => (value),
							() => (ripe_file.replace(/()?$/i, '.bin')),
							...Argument.requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.Data.Encode.Base64.decode_fs(ripe_file, raw_file);
					Console.notify('s', `执行成功`, [`${raw_file}`]);
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