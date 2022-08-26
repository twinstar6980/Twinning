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
					script_file: string;
				}) {
					let script_file: string;
					{
						script_file = a.script_file;
					}
					CoreX.Misc.evaluate_fs(script_file);
					Output.i(`运行完毕`);
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