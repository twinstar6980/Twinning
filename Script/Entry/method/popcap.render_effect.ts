namespace TwinStar.Script.Entry.method.popcap.render_effect {

	// ------------------------------------------------

	// encode *
	// decode *

	type Configuration = {
		version_number: Executor.Argument<bigint, false>;
		version_variant: Executor.Argument<bigint, false>;
		encode_buffer_size: Executor.Argument<string, false>;
	};

	export function _injector(
		configuration: Configuration,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.render_effect.encode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					definition_file: Executor.Argument<string, false>;
					data_file: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
					version_variant: Executor.Argument<bigint, false>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let definition_file: string;
					let data_file: string;
					let version_number: bigint;
					let version_variant: bigint;
					let buffer_size: bigint;
					{
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (definition_file.replace(/((\.popfx)(\.json))?$/i, '.popfx')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionNumberE), null, null, initial)),
						);
						version_variant = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant'),
							a.version_variant,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionVariantE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					KernelX.Tool.PopCap.RenderEffect.encode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any }, buffer_size);
					return [`${data_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					definition_file: '?input',
					data_file: '?default',
					version_number: configuration.version_number,
					version_variant: configuration.version_variant,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.popfx)(\.json)$/i]]),
				input_forwarder: 'definition_file',
			}),
			Executor.method_of({
				id: 'popcap.render_effect.decode',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file: Executor.Argument<string, false>;
					definition_file: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
					version_variant: Executor.Argument<bigint, false>;
				}) {
					let data_file: string;
					let definition_file: string;
					let version_number: bigint;
					let version_variant: bigint;
					{
						data_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							null,
							(initial) => (Console.path('file', ['in'], null, null, initial)),
						);
						definition_file = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file'),
							a.definition_file,
							(value) => (value),
							() => (data_file.replace(/((\.popfx))?$/i, '.popfx.json')),
							(initial) => (Console.path('file', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionNumberE), null, null, initial)),
						);
						version_variant = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant'),
							a.version_variant,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionVariantE), null, null, initial)),
						);
					}
					KernelX.Tool.PopCap.RenderEffect.decode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any });
					return [`${definition_file}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file: '?input',
					definition_file: '?default',
					version_number: configuration.version_number,
					version_variant: configuration.version_variant,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.popfx)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.render_effect.encode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					definition_file_directory: Executor.Argument<string, false>;
					data_file_directory: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
					version_variant: Executor.Argument<bigint, false>;
					buffer_size: Executor.Argument<string, false>;
				}) {
					let definition_file_directory: string;
					let data_file_directory: string;
					let version_number: bigint;
					let version_variant: bigint;
					let buffer_size: bigint;
					{
						definition_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file_directory'),
							a.definition_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (definition_file_directory.replace(/$/i, '.encode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionNumberE), null, null, initial)),
						);
						version_variant = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant'),
							a.version_variant,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionVariantE), null, null, initial)),
						);
						buffer_size = Executor.request_argument(
							Executor.query_argument_name(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							(initial) => (Console.size(null, null, initial)),
						);
					}
					let data_buffer = Kernel.ByteArray.allocate(Kernel.Size.value(buffer_size));
					simple_batch_execute(
						definition_file_directory,
						['file', /.+(\.popfx)(\.json)$/i],
						(item) => {
							let definition_file = `${definition_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -5)}`;
							KernelX.Tool.PopCap.RenderEffect.encode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any }, data_buffer.view());
						},
					);
					return [`${data_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					definition_file_directory: '?input',
					data_file_directory: '?default',
					version_number: configuration.version_number,
					version_variant: configuration.version_variant,
					buffer_size: configuration.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'definition_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.render_effect.decode.batch',
				name(
				) {
					return Executor.query_method_name(this.id);
				},
				worker(a: Entry.CommonArgument & {
					data_file_directory: Executor.Argument<string, false>;
					definition_file_directory: Executor.Argument<string, true>;
					version_number: Executor.Argument<bigint, false>;
					version_variant: Executor.Argument<bigint, false>;
				}) {
					let data_file_directory: string;
					let definition_file_directory: string;
					let version_number: bigint;
					let version_variant: bigint;
					{
						data_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							null,
							(initial) => (Console.path('directory', ['in'], null, null, initial)),
						);
						definition_file_directory = Executor.request_argument(
							Executor.query_argument_name(this.id, 'definition_file_directory'),
							a.definition_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.decode')),
							(initial) => (Console.path('directory', ['out', a.path_tactic_if_out_exist], null, null, initial)),
						);
						version_number = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_number'),
							a.version_number,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionNumberE), null, null, initial)),
						);
						version_variant = Executor.request_argument(
							Executor.query_argument_name(this.id, 'version_variant'),
							a.version_variant,
							(value) => (value),
							null,
							(initial) => (Console.option(Console.option_integer(KernelX.Tool.PopCap.RenderEffect.VersionVariantE), null, null, initial)),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.popfx)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let definition_file = `${definition_file_directory}/${item}.json`;
							KernelX.Tool.PopCap.RenderEffect.decode_fs(data_file, definition_file, { number: version_number as any, variant: version_variant as any });
						},
					);
					return [`${definition_file_directory}`];
				},
				default_argument: {
					...Entry.g_common_argument,
					data_file_directory: '?input',
					definition_file_directory: '?default',
					version_number: configuration.version_number,
					version_variant: configuration.version_variant,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Script.Entry.method.popcap.render_effect._injector,
});