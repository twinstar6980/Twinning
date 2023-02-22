/**
 * + image.transformation.flip
 * + image.transformation.scale
 * + image.transformation.scale_rate
 */
namespace TwinStar.Script.Entry.method.image.transformation {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'image.transformation.flip',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					source_file: Executor.RequireArgument<string>;
					destination_file: Executor.RequestArgument<string, true>;
					flip_horizontal: Executor.RequestArgument<boolean, false>;
					flip_vertical: Executor.RequestArgument<boolean, false>;
				}) {
					let source_file: string;
					let destination_file: string;
					let flip_horizontal: boolean;
					let flip_vertical: boolean;
					{
						source_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'source_file'),
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						destination_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'destination_file'),
							a.destination_file,
							(value) => (value),
							() => (source_file.replace(/((\.png))?$/i, '.flip.png')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						flip_horizontal = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'flip_horizontal'),
							a.flip_horizontal,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
						flip_vertical = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'flip_vertical'),
							a.flip_vertical,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.Image.Transformation.flip_fs(source_file, destination_file, flip_horizontal, flip_vertical);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_file: '?default',
					flip_horizontal: '?input',
					flip_vertical: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'source_file',
			}),
			Executor.method_of({
				id: 'image.transformation.scale',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					source_file: Executor.RequireArgument<string>;
					destination_file: Executor.RequestArgument<string, true>;
					scale_width: Executor.RequestArgument<bigint, false>;
					scale_height: Executor.RequestArgument<bigint, false>;
				}) {
					let source_file: string;
					let destination_file: string;
					let scale_width: bigint;
					let scale_height: bigint;
					{
						source_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'source_file'),
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						destination_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'destination_file'),
							a.destination_file,
							(value) => (value),
							() => (source_file.replace(/((\.png))?$/i, '.scale.png')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						scale_width = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'scale_width'),
							a.scale_width,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						);
						scale_height = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'scale_height'),
							a.scale_height,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						);
					}
					CoreX.Tool.Image.Transformation.scale_fs(source_file, destination_file, [scale_width, scale_height]);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_file: '?default',
					scale_width: '?input',
					scale_height: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'source_file',
			}),
			Executor.method_of({
				id: 'image.transformation.scale_rate',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					source_file: Executor.RequireArgument<string>;
					destination_file: Executor.RequestArgument<string, true>;
					scale_rate: Executor.RequestArgument<number, false>;
				}) {
					let source_file: string;
					let destination_file: string;
					let scale_rate: number;
					{
						source_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'source_file'),
							a.source_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						destination_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'destination_file'),
							a.destination_file,
							(value) => (value),
							() => (source_file.replace(/((\.png))?$/i, '.scale.png')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						scale_rate = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'scale_rate'),
							a.scale_rate,
							(value) => (value),
							null,
							() => (Console.number(null)),
							(value) => ((0.0 < value) ? null : los(`比例应大于零`)),
						);
					}
					CoreX.Tool.Image.Transformation.scale_rate_fs(source_file, destination_file, scale_rate);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_file: '?default',
					scale_rate: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'source_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.image.transformation._injector,
});