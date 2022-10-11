/**
 * + js.evaluate JS 执行
 */
namespace TwinKleS.Entry.method.js {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'js.evaluate',
				description: 'JS 执行',
				worker(a: Entry.CFSA & {
					script_file: Argument.Require<string>;
				}) {
					let script_file: string;
					{
						script_file = Argument.require(
							'脚本文件', '',
							a.script_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
					}
					let result = CoreX.Misc.evaluate_fs(script_file);
					Console.notify('s', `执行成功`, [`${result}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					script_file: undefined!,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.js)$/i]]),
				input_forwarder: 'script_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.js._injector,
});