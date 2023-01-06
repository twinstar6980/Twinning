/**
 * + popcap.rton.encode PopCap-RTON 编码
 * + popcap.rton.decode PopCap-RTON 解码
 * + popcap.rton.encrypt PopCap-RTON 加密
 * + popcap.rton.decrypt PopCap-RTON 解密
 * + popcap.rton.encode_then_encrypt PopCap-RTON 编码并加密
 * + popcap.rton.decrypt_then_decode PopCap-RTON 解密并解码
 * + popcap.rton.encode.batch [批处理] PopCap-RTON 编码
 * + popcap.rton.decode.batch [批处理] PopCap-RTON 解码
 * + popcap.rton.encrypt.batch [批处理] PopCap-RTON 加密
 * + popcap.rton.decrypt.batch [批处理] PopCap-RTON 解密
 * + popcap.rton.encode_then_encrypt.batch [批处理] PopCap-RTON 编码并加密
 * + popcap.rton.decrypt_then_decode.batch [批处理] PopCap-RTON 解密并解码
 */
namespace TwinStar.Entry.method.popcap.rton {

	// ------------------------------------------------

	type Config = {
		encode_buffer_size: Executor.RequestArgument<string, false>;
		crypt_key: Executor.RequestArgument<string, false>;
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.rton.encode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					value_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let value_file: string;
					let data_file: string;
					let buffer_size: bigint;
					{
						value_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'value_file'),
							a.value_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (value_file.replace(/((\.json))?$/i, '.rton')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.RTON.encode_fs(data_file, value_file, true, true, buffer_size);
					Console.notify('s', localized(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					value_file: undefined!,
					data_file: '?default',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'value_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					value_file: Executor.RequestArgument<string, true>;
				}) {
					let data_file: string;
					let value_file: string;
					{
						data_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						value_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'value_file'),
							a.value_file,
							(value) => (value),
							() => (data_file.replace(/((\.rton))?$/i, '.json')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
					}
					CoreX.Tool.PopCap.RTON.decode_fs(data_file, value_file);
					Console.notify('s', localized(`执行成功`), [`${value_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					value_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rton)$/i]]),
				input_forwarder: 'data_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.encrypt',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file: Executor.RequireArgument<string>;
					cipher_file: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let plain_file: string;
					let cipher_file: string;
					let key: string;
					{
						plain_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						cipher_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							() => (plain_file.replace(/((\.rton))?$/i, '.cipher.rton')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.RTON.encrypt_fs(plain_file, cipher_file, key);
					Console.notify('s', localized(`执行成功`), [`${cipher_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rton)$/i]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.decrypt',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					cipher_file: Executor.RequireArgument<string>;
					plain_file: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let cipher_file: string;
					let plain_file: string;
					let key: string;
					{
						cipher_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'cipher_file'),
							a.cipher_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						plain_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'plain_file'),
							a.plain_file,
							(value) => (value),
							() => (cipher_file.replace(/((\.rton))?$/i, '.plain.rton')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.RTON.decrypt_fs(cipher_file, plain_file, key);
					Console.notify('s', localized(`执行成功`), [`${plain_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					cipher_file: undefined!,
					plain_file: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rton)$/i]]),
				input_forwarder: 'cipher_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.encode_then_encrypt',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					value_file: Executor.RequireArgument<string>;
					data_file: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let value_file: string;
					let data_file: string;
					let key: string;
					let buffer_size: bigint;
					{
						value_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'value_file'),
							a.value_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						data_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							() => (value_file.replace(/((\.json))?$/i, '.rton')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.RTON.encode_then_encrypt_fs(value_file, data_file, true, true, key, buffer_size);
					Console.notify('s', localized(`执行成功`), [`${data_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					value_file: undefined!,
					data_file: '?default',
					key: config.crypt_key,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'value_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.decrypt_then_decode',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file: Executor.RequireArgument<string>;
					value_file: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let data_file: string;
					let value_file: string;
					let key: string;
					{
						data_file = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file'),
							a.data_file,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_file(value)),
						);
						value_file = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'value_file'),
							a.value_file,
							(value) => (value),
							() => (data_file.replace(/((\.rton))?$/i, '.json')),
							...Executor.argument_requester_for_path('file', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					CoreX.Tool.PopCap.RTON.decrypt_then_decode_fs(data_file, value_file, key);
					Console.notify('s', localized(`执行成功`), [`${value_file}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					value_file: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rton)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.rton.encode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					value_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let value_file_directory: string;
					let data_file_directory: string;
					let buffer_size: bigint;
					{
						value_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'value_file_directory'),
							a.value_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (value_file_directory.replace(/$/i, '.rton_encode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					let rton_data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						value_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let json_file = `${value_file_directory}/${item}`;
							let rton_file = `${data_file_directory}/${item.slice(0, -5)}.rton`;
							CoreX.Tool.PopCap.RTON.encode_fs(rton_file, json_file, true, true, rton_data_buffer.view());
						},
					);
					Console.notify('s', localized(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					value_file_directory: undefined!,
					data_file_directory: '?default',
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'value_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					value_file_directory: Executor.RequestArgument<string, true>;
				}) {
					let data_file_directory: string;
					let value_file_directory: string;
					{
						data_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						value_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'value_file_directory'),
							a.value_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.rton_decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.rton)$/i],
						(item) => {
							let rton_file = `${data_file_directory}/${item}`;
							let json_file = `${value_file_directory}/${item.slice(0, -5)}.json`;
							CoreX.Tool.PopCap.RTON.decode_fs(rton_file, json_file);
						},
					);
					Console.notify('s', localized(`执行成功`), [`${value_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					value_file_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.encrypt.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					plain_file_directory: Executor.RequireArgument<string>;
					cipher_file_directory: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let plain_file_directory: string;
					let cipher_file_directory: string;
					let key: string;
					{
						plain_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'plain_file_directory'),
							a.plain_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						cipher_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'cipher_file_directory'),
							a.cipher_file_directory,
							(value) => (value),
							() => (plain_file_directory.replace(/$/i, '.rton_encrypt')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						plain_file_directory,
						['file', /.+(\.rton)$/i],
						(item) => {
							let plain_file = `${plain_file_directory}/${item}`;
							let cipher_file = `${cipher_file_directory}/${item}`;
							CoreX.Tool.PopCap.RTON.encrypt_fs(plain_file, cipher_file, key);
						},
					);
					Console.notify('s', localized(`执行成功`), [`${cipher_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file_directory: undefined!,
					cipher_file_directory: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'plain_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.decrypt.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					cipher_file_directory: Executor.RequireArgument<string>;
					plain_file_directory: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let cipher_file_directory: string;
					let plain_file_directory: string;
					let key: string;
					{
						cipher_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'cipher_file_directory'),
							a.cipher_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						plain_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'plain_file_directory'),
							a.plain_file_directory,
							(value) => (value),
							() => (cipher_file_directory.replace(/$/i, '.rton_decrypt')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						cipher_file_directory,
						['file', /.+(\.rton)$/i],
						(item) => {
							let cipher_file = `${cipher_file_directory}/${item}`;
							let plain_file = `${plain_file_directory}/${item}`;
							CoreX.Tool.PopCap.RTON.decrypt_fs(cipher_file, plain_file, key);
						},
					);
					Console.notify('s', localized(`执行成功`), [`${plain_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					cipher_file_directory: undefined!,
					plain_file_directory: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'cipher_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.encode_then_encrypt.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					value_file_directory: Executor.RequireArgument<string>;
					data_file_directory: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
					buffer_size: Executor.RequestArgument<string, false>;
				}) {
					let value_file_directory: string;
					let data_file_directory: string;
					let key: string;
					let buffer_size: bigint;
					{
						value_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'value_file_directory'),
							a.value_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						data_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							() => (value_file_directory.replace(/$/i, '.rton_encode_then_encrypt')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
						buffer_size = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'buffer_size'),
							a.buffer_size,
							(value) => (parse_size_string(value)),
							null,
							() => (Console.size(null)),
							(value) => (null),
						);
					}
					let rton_data_buffer = Core.ByteArray.allocate(Core.Size.value(buffer_size));
					simple_batch_execute(
						value_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let json_file = `${value_file_directory}/${item}`;
							let rton_file = `${data_file_directory}/${item.slice(0, -5)}.rton`;
							CoreX.Tool.PopCap.RTON.encode_then_encrypt_fs(json_file, rton_file, true, true, key, rton_data_buffer.view());
						},
					);
					Console.notify('s', localized(`执行成功`), [`${data_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					value_file_directory: undefined!,
					data_file_directory: '?default',
					key: config.crypt_key,
					buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'value_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.decrypt_then_decode.batch',
				descriptor(
				) {
					return Executor.query_method_description(this.id);
				},
				worker(a: Entry.CFSA & {
					data_file_directory: Executor.RequireArgument<string>;
					value_file_directory: Executor.RequestArgument<string, true>;
					key: Executor.RequestArgument<string, false>;
				}) {
					let data_file_directory: string;
					let value_file_directory: string;
					let key: string;
					{
						data_file_directory = Executor.require_argument(
							...Executor.query_argument_message(this.id, 'data_file_directory'),
							a.data_file_directory,
							(value) => (value),
							(value) => (CoreX.FileSystem.exist_directory(value)),
						);
						value_file_directory = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'value_file_directory'),
							a.value_file_directory,
							(value) => (value),
							() => (data_file_directory.replace(/$/i, '.rton_decrypt_then_decode')),
							...Executor.argument_requester_for_path('directory', [false, a.fs_tactic_if_exist]),
						);
						key = Executor.request_argument(
							...Executor.query_argument_message(this.id, 'key'),
							a.key,
							(value) => (value),
							null,
							() => (Console.string(null)),
							(value) => (null),
						);
					}
					simple_batch_execute(
						data_file_directory,
						['file', /.+(\.rton)$/i],
						(item) => {
							let rton_file = `${data_file_directory}/${item}`;
							let json_file = `${value_file_directory}/${item.slice(0, -5)}.json`;
							CoreX.Tool.PopCap.RTON.decrypt_then_decode_fs(rton_file, json_file, key);
						},
					);
					Console.notify('s', localized(`执行成功`), [`${value_file_directory}`]);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file_directory: undefined!,
					value_file_directory: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'data_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinStar.Entry.method.popcap.rton._injector,
});