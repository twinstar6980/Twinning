namespace TwinStar.Script.Entry.method.image.transformation {

	// ------------------------------------------------

	// flip
	// scale
	// scale_rate

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
					horizontal: Executor.RequestArgument<boolean, false>;
					vertical: Executor.RequestArgument<boolean, false>;
				}) {
					let source_file: string;
					let destination_file: string;
					let horizontal: boolean;
					let vertical: boolean;
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
						horizontal = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'horizontal'),
							a.horizontal,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
						vertical = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'vertical'),
							a.vertical,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.Image.Transformation.flip_fs(source_file, destination_file, horizontal, vertical);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_file: '?default',
					horizontal: '?input',
					vertical: '?input',
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
					width: Executor.RequestArgument<bigint, false>;
					height: Executor.RequestArgument<bigint, false>;
				}) {
					let source_file: string;
					let destination_file: string;
					let width: bigint;
					let height: bigint;
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
						width = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'width'),
							a.width,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						);
						height = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'height'),
							a.height,
							(value) => (value),
							null,
							() => (Console.integer(null)),
							(value) => ((0n < value) ? null : los(`尺寸应大于零`)),
						);
					}
					CoreX.Tool.Image.Transformation.scale_fs(source_file, destination_file, [width, height]);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_file: '?default',
					width: '?input',
					height: '?input',
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
					size_rate: Executor.RequestArgument<number, false>;
				}) {
					let source_file: string;
					let destination_file: string;
					let size_rate: number;
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
						size_rate = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'size_rate'),
							a.size_rate,
							(value) => (value),
							null,
							() => (Console.number(null)),
							(value) => ((0.0 < value) ? null : los(`比例应大于零`)),
						);
					}
					CoreX.Tool.Image.Transformation.scale_rate_fs(source_file, destination_file, size_rate);
					Console.notify('s', los(`执行成功`), [`${destination_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					source_file: undefined!,
					destination_file: '?default',
					size_rate: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'source_file',
			}),
		);
		g_executor_method_of_batch.push(
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.image.transformation._injector,
});