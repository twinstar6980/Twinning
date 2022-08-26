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
namespace TwinKleS.Entry.method.popcap.rton {

	// ------------------------------------------------

	type Config = {
		encode_buffer_size: string | '?input';
		crypt_key: string | '?input';
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.rton.encode',
				description: 'PopCap-RTON 编码',
				worker(a: Entry.CFSA & {
					json_file: string;
					rton_file: string | '?default' | '?input';
					rton_data_buffer_size: string | '?input';
				}) {
					let json_file: string;
					let rton_file: string;
					let rton_data_buffer_size: bigint;
					{
						json_file = a.json_file;
						rton_file = ArgumentParser.path(a.rton_file, {
							input_message: '请输入输出路径',
							default_value: json_file.replace(/((\.json))?$/i, '.rton'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.rton_data_buffer_size === '?input') {
							rton_data_buffer_size = Input.size(`请输入用于保存rton输出的内存空间大小`)!;
						} else {
							rton_data_buffer_size = parse_size_string(a.rton_data_buffer_size);
						}
					}
					CoreX.Tool.PopCap.RTON.encode_fs(json_file, rton_file, true, true, rton_data_buffer_size);
					Output.i(`输出路径：${rton_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					json_file: undefined!,
					rton_file: '?default',
					rton_data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'json_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.decode',
				description: 'PopCap-RTON 解码',
				worker(a: Entry.CFSA & {
					rton_file: string;
					json_file: string | '?default' | '?input';
				}) {
					let rton_file: string;
					let json_file: string;
					{
						rton_file = a.rton_file;
						json_file = ArgumentParser.path(a.json_file, {
							input_message: '请输入输出路径',
							default_value: rton_file.replace(/((\.rton))?$/i, '.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					CoreX.Tool.PopCap.RTON.decode_fs(rton_file, json_file);
					Output.i(`输出路径：${json_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					rton_file: undefined!,
					json_file: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rton)$/i]]),
				input_forwarder: 'rton_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.encrypt',
				description: 'PopCap-RTON 加密',
				worker(a: Entry.CFSA & {
					plain_file: string;
					cipher_file: string | '?default' | '?input';
					key: string | '?input';
				}) {
					let plain_file: string;
					let cipher_file: string;
					let key: string;
					{
						plain_file = a.plain_file;
						cipher_file = ArgumentParser.path(a.cipher_file, {
							input_message: '请输入输出路径',
							default_value: plain_file.replace(/((\.rton))?$/i, '.cipher.rton'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
					}
					CoreX.Tool.PopCap.RTON.encrypt_fs(plain_file, cipher_file, key);
					Output.i(`输出路径：${cipher_file}`);
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
				description: 'PopCap-RTON 解密',
				worker(a: Entry.CFSA & {
					cipher_file: string;
					plain_file: string | '?default' | '?input';
					key: string | '?input';
				}) {
					let cipher_file: string;
					let plain_file: string;
					let key: string;
					{
						cipher_file = a.cipher_file;
						plain_file = ArgumentParser.path(a.plain_file, {
							input_message: '请输入输出路径',
							default_value: cipher_file.replace(/((\.rton))?$/i, '.plain.rton'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
					}
					CoreX.Tool.PopCap.RTON.decrypt_fs(cipher_file, plain_file, key);
					Output.i(`输出路径：${plain_file}`);
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
				description: 'PopCap-RTON 编码并加密',
				worker(a: Entry.CFSA & {
					json_file: string;
					rton_file: string | '?default' | '?input';
					key: string | '?input';
					rton_data_buffer_size: string | '?input';
				}) {
					let json_file: string;
					let rton_file: string;
					let key: string;
					let rton_data_buffer_size: bigint;
					{
						json_file = a.json_file;
						rton_file = ArgumentParser.path(a.rton_file, {
							input_message: '请输入输出路径',
							default_value: json_file.replace(/((\.json))?$/i, '.rton'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
						if (a.rton_data_buffer_size === '?input') {
							rton_data_buffer_size = Input.size(`请输入用于保存rton输出的内存空间大小`)!;
						} else {
							rton_data_buffer_size = parse_size_string(a.rton_data_buffer_size);
						}
					}
					CoreX.Tool.PopCap.RTON.encode_then_encrypt_fs(json_file, rton_file, true, true, key, rton_data_buffer_size);
					Output.i(`输出路径：${rton_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					json_file: undefined!,
					rton_file: '?default',
					key: config.crypt_key,
					rton_data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.json)$/i]]),
				input_forwarder: 'json_file',
			}),
			Executor.method_of({
				id: 'popcap.rton.decrypt_then_decode',
				description: 'PopCap-RTON 解密并解码',
				worker(a: Entry.CFSA & {
					rton_file: string;
					json_file: string | '?default' | '?input';
					key: string | '?input';
				}) {
					let rton_file: string;
					let json_file: string;
					let key: string;
					{
						rton_file = a.rton_file;
						json_file = ArgumentParser.path(a.json_file, {
							input_message: '请输入输出路径',
							default_value: rton_file.replace(/((\.rton))?$/i, '.json'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
					}
					CoreX.Tool.PopCap.RTON.decrypt_then_decode_fs(rton_file, json_file, key);
					Output.i(`输出路径：${json_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					rton_file: undefined!,
					json_file: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.rton)$/i]]),
				input_forwarder: 'rton_file',
			}),
		);
		g_executor_method_of_batch.push(
			Executor.method_of({
				id: 'popcap.rton.encode.batch',
				description: '[批处理] PopCap-RTON 编码',
				worker(a: Entry.CFSA & {
					json_file_directory: string;
					rton_file_directory: string | '?default' | '?input';
					rton_data_buffer_size: string | '?input';
				}) {
					let json_file_directory: string;
					let rton_file_directory: string;
					let rton_data_buffer_size: bigint;
					{
						json_file_directory = a.json_file_directory;
						rton_file_directory = ArgumentParser.path(a.rton_file_directory, {
							input_message: '请输入输出路径',
							default_value: json_file_directory.replace(/$/i, '.rton_encode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.rton_data_buffer_size === '?input') {
							rton_data_buffer_size = Input.size(`请输入用于保存rton输出的内存空间大小`)!;
						} else {
							rton_data_buffer_size = parse_size_string(a.rton_data_buffer_size);
						}
					}
					let rton_data_buffer = Core.ByteArray.allocate(Core.Size.value(rton_data_buffer_size));
					simple_batch_execute(
						json_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let json_file = `${json_file_directory}/${item}`;
							let rton_file = `${rton_file_directory}/${item.slice(0, -5)}.rton`;
							CoreX.Tool.PopCap.RTON.encode_fs(json_file, rton_file, true, true, rton_data_buffer.view());
						},
					);
					Output.i(`输出路径：${rton_file_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					json_file_directory: undefined!,
					rton_file_directory: '?default',
					rton_data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'json_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.decode.batch',
				description: '[批处理] PopCap-RTON 解码',
				worker(a: Entry.CFSA & {
					rton_file_directory: string;
					json_file_directory: string | '?default' | '?input';
				}) {
					let rton_file_directory: string;
					let json_file_directory: string;
					{
						rton_file_directory = a.rton_file_directory;
						json_file_directory = ArgumentParser.path(a.json_file_directory, {
							input_message: '请输入输出路径',
							default_value: rton_file_directory.replace(/$/i, '.rton_decode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
					}
					simple_batch_execute(
						rton_file_directory,
						['file', /.+(\.rton)$/i],
						(item) => {
							let rton_file = `${rton_file_directory}/${item}`;
							let json_file = `${json_file_directory}/${item.slice(0, -5)}.json`;
							CoreX.Tool.PopCap.RTON.decode_fs(rton_file, json_file);
						},
					);
					Output.i(`输出路径：${json_file_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					rton_file_directory: undefined!,
					json_file_directory: '?default',
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'rton_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.encrypt.batch',
				description: '[批处理] PopCap-RTON 加密',
				worker(a: Entry.CFSA & {
					plain_file_directory: string;
					cipher_file_directory: string | '?default' | '?input';
					key: string | '?input';
				}) {
					let plain_file_directory: string;
					let cipher_file_directory: string;
					let key: string;
					{
						plain_file_directory = a.plain_file_directory;
						cipher_file_directory = ArgumentParser.path(a.cipher_file_directory, {
							input_message: '请输入输出路径',
							default_value: plain_file_directory.replace(/$/i, '.rton_encrypt'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
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
					Output.i(`输出路径：${cipher_file_directory}`);
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
				description: '[批处理] PopCap-RTON 解密',
				worker(a: Entry.CFSA & {
					cipher_file_directory: string;
					plain_file_directory: string | '?default' | '?input';
					key: string | '?input';
				}) {
					let cipher_file_directory: string;
					let plain_file_directory: string;
					let key: string;
					{
						cipher_file_directory = a.cipher_file_directory;
						plain_file_directory = ArgumentParser.path(a.plain_file_directory, {
							input_message: '请输入输出路径',
							default_value: cipher_file_directory.replace(/$/i, '.rton_decrypt'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
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
					Output.i(`输出路径：${plain_file_directory}`);
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
				description: '[批处理] PopCap-RTON 编码并加密',
				worker(a: Entry.CFSA & {
					json_file_directory: string;
					rton_file_directory: string | '?default' | '?input';
					key: string | '?input';
					rton_data_buffer_size: string | '?input';
				}) {
					let json_file_directory: string;
					let rton_file_directory: string;
					let key: string;
					let rton_data_buffer_size: bigint;
					{
						json_file_directory = a.json_file_directory;
						rton_file_directory = ArgumentParser.path(a.rton_file_directory, {
							input_message: '请输入输出路径',
							default_value: json_file_directory.replace(/$/i, '.rton_encode_then_encrypt'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
						if (a.rton_data_buffer_size === '?input') {
							rton_data_buffer_size = Input.size(`请输入用于保存rton输出的内存空间大小`)!;
						} else {
							rton_data_buffer_size = parse_size_string(a.rton_data_buffer_size);
						}
					}
					let rton_data_buffer = Core.ByteArray.allocate(Core.Size.value(rton_data_buffer_size));
					simple_batch_execute(
						json_file_directory,
						['file', /.+(\.json)$/i],
						(item) => {
							let json_file = `${json_file_directory}/${item}`;
							let rton_file = `${rton_file_directory}/${item.slice(0, -5)}.rton`;
							CoreX.Tool.PopCap.RTON.encode_then_encrypt_fs(json_file, rton_file, true, true, key, rton_data_buffer.view());
						},
					);
					Output.i(`输出路径：${rton_file_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					json_file_directory: undefined!,
					rton_file_directory: '?default',
					key: config.crypt_key,
					rton_data_buffer_size: config.encode_buffer_size,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'json_file_directory',
			}),
			Executor.method_of({
				id: 'popcap.rton.decrypt_then_decode.batch',
				description: '[批处理] PopCap-RTON 解密并解码',
				worker(a: Entry.CFSA & {
					rton_file_directory: string;
					json_file_directory: string | '?default' | '?input';
					key: string | '?input';
				}) {
					let rton_file_directory: string;
					let json_file_directory: string;
					let key: string;
					{
						rton_file_directory = a.rton_file_directory;
						json_file_directory = ArgumentParser.path(a.json_file_directory, {
							input_message: '请输入输出路径',
							default_value: rton_file_directory.replace(/$/i, '.rton_decrypt_then_decode'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.string(`请输入密钥`)!;
						} else {
							key = a.key;
						}
					}
					simple_batch_execute(
						rton_file_directory,
						['file', /.+(\.rton)$/i],
						(item) => {
							let rton_file = `${rton_file_directory}/${item}`;
							let json_file = `${json_file_directory}/${item.slice(0, -5)}.json`;
							CoreX.Tool.PopCap.RTON.decrypt_then_decode_fs(rton_file, json_file, key);
						},
					);
					Output.i(`输出路径：${json_file_directory}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					rton_file_directory: undefined!,
					json_file_directory: '?default',
					key: config.crypt_key,
				},
				input_filter: Entry.file_system_path_test_generator([['directory', null]]),
				input_forwarder: 'rton_file_directory',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.rton._injector,
});