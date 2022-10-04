/**
 * + json.format JSON 格式化
 * + json.format.batch [批处理] JSON 格式化
 */
namespace TwinKleS.Entry.method.json {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'json.format',
				description: 'JSON 格式化',
				worker(a: Entry.CFSA & {
					source_file: string;
					dest_file: string | '?default' | '?input';
					disable_trailing_comma: boolean | '?default' | '?input';
					disable_array_wrap_line: boolean | '?default' | '?input';
				}) {
					let source_file: string;
					let dest_file: string;
					let disable_trailing_comma: boolean | undefined;
					let disable_array_wrap_line: boolean | undefined;
					{
						source_file = a.source_file;
						dest_file = ArgumentParser.path(a.dest_file, {
							input_message: '请输入输出路径',
							default_value: source_file.replace(/((\.json))?$/i, '.formatted.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.disable_trailing_comma === '?input') {
							Console.notify('i', `是否禁用尾随逗号`, [`跳过输入以使用默认设置`]);
							disable_trailing_comma = nonnull_or(Console.yon(true), undefined);
						} else if (a.disable_trailing_comma === '?default') {
							disable_trailing_comma = undefined;
						} else {
							disable_trailing_comma = a.disable_trailing_comma;
						}
						if (a.disable_array_wrap_line === '?input') {
							Console.notify('i', `是否禁用数组元素换行`, [`跳过输入以使用默认设置`]);
							disable_array_wrap_line = nonnull_or(Console.yon(true), undefined);
						} else if (a.disable_array_wrap_line === '?default') {
							disable_array_wrap_line = undefined;
						} else {
							disable_array_wrap_line = a.disable_array_wrap_line;
						}
					}
					let json = CoreX.JSON.read_fs(source_file);
					CoreX.JSON.write_fs(dest_file, json, disable_trailing_comma, disable_array_wrap_line);
					Console.notify('s', `输出路径：${dest_file}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					dest_file: '?default',
					disable_trailing_comma: '?input',
					disable_array_wrap_line: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'source_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'json.format.batch',
				description: '[批处理] JSON 格式化',
				worker(a: Entry.CFSA & {
					source_file_directory: string;
					dest_file_directory: string | '?default' | '?input';
					disable_trailing_comma: boolean | '?default' | '?input';
					disable_array_wrap_line: boolean | '?default' | '?input';
				}) {
					let source_file_directory: string;
					let dest_file_directory: string;
					let disable_trailing_comma: boolean | undefined;
					let disable_array_wrap_line: boolean | undefined;
					{
						source_file_directory = a.source_file_directory;
						dest_file_directory = ArgumentParser.path(a.dest_file_directory, {
							input_message: '请输入输出路径',
							default_value: source_file_directory.replace(/$/i, '.json_format'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.disable_trailing_comma === '?input') {
							Console.notify('i', `是否禁用尾随逗号`, [`跳过输入以使用默认设置`]);
							disable_trailing_comma = nonnull_or(Console.yon(true), undefined);
						} else if (a.disable_trailing_comma === '?default') {
							disable_trailing_comma = undefined;
						} else {
							disable_trailing_comma = a.disable_trailing_comma;
						}
						if (a.disable_array_wrap_line === '?input') {
							Console.notify('i', `是否禁用数组元素换行`, [`跳过输入以使用默认设置`]);
							disable_array_wrap_line = nonnull_or(Console.yon(true), undefined);
						} else if (a.disable_array_wrap_line === '?default') {
							disable_array_wrap_line = undefined;
						} else {
							disable_array_wrap_line = a.disable_array_wrap_line;
						}
					}
					simple_batch_execute(
						source_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let source_file = `${source_file_directory}/${item}`;
							let dest_file = `${dest_file_directory}/${item}`;
							let json = CoreX.JSON.read_fs(source_file);
							CoreX.JSON.write_fs(dest_file, json, disable_trailing_comma, disable_array_wrap_line);
						},
					);
					Console.notify('s', `输出路径：${dest_file_directory}`, []);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file_directory: undefined!,
					dest_file_directory: '?default',
					disable_trailing_comma: '?input',
					disable_array_wrap_line: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'source_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.json._injector,
});