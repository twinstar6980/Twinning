/**
 * + data.hash.md5 MD5 哈希
 */
namespace TwinKleS.Entry.method.data.hash {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.hash.md5',
				description: 'MD5 哈希',
				worker(a: Entry.CFSA & {
					file: string;
				}) {
					let file: string;
					{
						file = a.file;
					}
					let result = CoreX.Tool.Data.Hash.MD5.hash_fs(file);
					Console.notify('s', `MD5：${result}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					file: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.data.hash._injector,
});