/**
 * + popcap.reanim.encode PopCap-Reanim 编码
 * + popcap.reanim.decode PopCap-Reanim 解码
 * + popcap.reanim.encode.batch [批处理] PopCap-Reanim 编码
 * + popcap.reanim.decode.batch [批处理] PopCap-Reanim 解码
 */
namespace TwinKleS.Entry.method.popcap.reanim {

	// ------------------------------------------------

	type Config = {
		encode_buffer_size: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.reanim.encode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: boolean | '?input';
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file: string;
					let data_file: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					let buffer_size: bigint;
					{
						manifest_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (manifest_file.replace(/((\.reanim)(\.json))?$/i, '.reanim.compiled')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option(['desktop', 'phone'].map((e) => ([e])), null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : localized(`版本不受支持`)),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.Reanim.encode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 }, buffer_size);
					Console.notify('s', localized(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file: undefined!,
					data_file: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.reanim)(\.json)$/i]]),
				input_forwarder: 'manifest_file',
			}),
			Executor.method_of({
				id: 'popcap.reanim.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					manifest_file: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: boolean | '?input';
				}) {
					let data_file: string;
					let manifest_file: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					{
						data_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						manifest_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'manifest_file'),
							a.manifest_file,
							(value) => (value),
							() => (data_file.replace(/((\.reanim)(\.compiled))?$/i, '.reanim.json')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option(['desktop', 'phone'].map((e) => ([e])), null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : localized(`版本不受支持`)),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.Reanim.decode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 });
					Console.notify('s', localized(`执行成功`), [`${manifest_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					manifest_file: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.reanim)(\.compiled)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.reanim.encode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					manifest_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: boolean | '?input';
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let manifest_file_directory: string;
					let data_file_directory: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					let buffer_size: bigint;
					{
						manifest_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (manifest_file_directory.replace(/$/i, '.reanim_encode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option(['desktop', 'phone'].map((e) => ([e])), null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : localized(`版本不受支持`)),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.binary_size(null)),
							(value) => (null),
						);
					}
					let data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						manifest_file_directory,
						['file', /.+(\.reanim)(\.json)$/i],
						(item) => {
							let manifest_file = `${manifest_file_directory}/${item}`;
							let data_file = `${data_file_directory}/${item.slice(0, -9)}.compiled`;
							CoreX.Tool.PopCap.Reanim.encode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 }, data_buffer.view());
						},
					);
					Console.notify('s', localized(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					manifest_file_directory: undefined!,
					data_file_directory: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'manifest_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.reanim.decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					manifest_file_directory: Executor.RequestArgument<string, true>;
					version_platform: Executor.RequestArgument<string, false>;
					version_variant_64: boolean | '?input';
				}) {
					let data_file_directory: string;
					let manifest_file_directory: string;
					let version_platform: ['desktop', 'phone'][number];
					let version_variant_64: boolean;
					{
						data_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						manifest_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'manifest_file_directory'),
							a.manifest_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.reanim_decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						version_platform = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_platform'),
							a.version_platform,
							(value) => (value),
							null,
							() => (Console.option(['desktop', 'phone'].map((e) => ([e])), null)),
							(value) => (['desktop', 'phone'].includes(value) ? null : localized(`版本不受支持`)),
						);
						version_variant_64 = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'version_variant_64'),
							a.version_variant_64,
							(value) => (value),
							null,
							() => (Console.confirm(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.reanim)(\.compiled)$/i],
						(item) => {
							let data_file = `${data_file_directory}/${item}`;
							let manifest_file = `${manifest_file_directory}/${item.slice(0, -9)}.json`;
							CoreX.Tool.PopCap.Reanim.decode_fs(data_file, manifest_file, { platform: version_platform, variant_64: version_variant_64 });
						},
					);
					Console.notify('s', localized(`执行成功`), [`${manifest_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					manifest_file_directory: '?default',
					version_platform: '?input',
					version_variant_64: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.reanim._injector,
});