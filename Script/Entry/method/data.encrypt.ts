/**
 * + data.encrypt.xor.crypt XOR 加解密
 */
namespace TwinKleS.Entry.method.data.encrypt {

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'data.encrypt.xor.crypt',
				description: 'XOR 加解密',
				worker(a: Entry.CFSA & {
					plain_file: string;
					cipher_file: string | '?default' | '?input';
					key: bigint | '?input';
				}) {
					let plain_file: string;
					let cipher_file: string;
					let key: bigint;
					{
						plain_file = a.plain_file;
						cipher_file = ArgumentParser.path(a.cipher_file, {
							input_message: '请输入输出路径',
							default_value: plain_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.key === '?input') {
							key = Input.integer(`请输入密钥（八位无符号整数）`, (value) => ((value >= 0x00n && value <= 0xFFn) ? null : `必须为八位无符号整数`))!;
						} else {
							key = a.key;
						}
					}
					CoreX.Tool.Data.Encrypt.XOR.crypt_fs(plain_file, cipher_file, key);
					Output.i(`输出路径：${cipher_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
					key: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'data.encrypt.rijndael.encrypt',
				description: 'Rijndael 加密',
				worker(a: Entry.CFSA & {
					plain_file: string;
					cipher_file: string | '?default' | '?input';
					mode: string | '?input';
					block_size: bigint | '?input';
					key: string | '?input';
					iv: string | '?input';
				}) {
					let plain_file: string;
					let cipher_file: string;
					let mode: Core.Tool.Data.Encrypt.Rijndael.JS_Mode;
					let block_size: bigint;
					let key: string;
					let iv: string;
					{
						plain_file = a.plain_file;
						cipher_file = ArgumentParser.path(a.cipher_file, {
							input_message: '请输入输出路径',
							default_value: plain_file.replace(/()?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.mode === '?input') {
							mode = Input.string(`请选择算法模式（ecb、cbc、cfb）`, Check.enum_checkerx(['ecb', 'cbc', 'cfb']))! as any;
						} else {
							mode = a.mode as any;
						}
						if (a.block_size === '?input') {
							block_size = Input.integer(`请输入块大小（16、24、32）`, Check.enum_checkerx([16n, 24n, 32n]))!;
						} else {
							block_size = a.block_size;
						}
						if (a.key === '?input') {
							key = Input.string(`请输入密钥（长度为16、24、32）`, (value) => ((value.length === 16 || value.length === 24 || value.length === 32) ? null : `长度必须为16、24、32`))!;
						} else {
							key = a.key;
						}
						if (mode === 'cbc' || mode === 'cfb') {
							if (a.iv === '?input') {
								iv = Input.string(`请输入初始向量（长度与块大小相同）`, (value) => ((value.length === Number(block_size)) ? null : `长度必须与块大小相同`))!;
							} else {
								iv = a.iv;
							}
						} else {
							iv = '';
						}
					}
					CoreX.Tool.Data.Encrypt.Rijndael.encrypt_fs(plain_file, cipher_file, mode, block_size, BigInt(key.length), key, iv);
					Output.i(`输出路径：${cipher_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					plain_file: undefined!,
					cipher_file: '?default',
					mode: '?input',
					block_size: '?input',
					key: '?input',
					iv: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'plain_file',
			}),
			Executor.method_of({
				id: 'data.encrypt.rijndael.decrypt',
				description: 'Rijndael 解密',
				worker(a: Entry.CFSA & {
					cipher_file: string;
					plain_file: string | '?default' | '?input';
					mode: string | '?input';
					block_size: bigint | '?input';
					key: string | '?input';
					iv: string | '?input';
				}) {
					let cipher_file: string;
					let plain_file: string;
					let mode: Core.Tool.Data.Encrypt.Rijndael.JS_Mode;
					let block_size: bigint;
					let key: string;
					let iv: string;
					{
						cipher_file = a.cipher_file;
						plain_file = ArgumentParser.path(a.plain_file, {
							input_message: '请输入输出路径',
							default_value: cipher_file.replace(/((\.rton))?$/i, '.bin'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.mode === '?input') {
							mode = Input.string(`请选择算法模式（ecb、cbc、cfb）`, Check.enum_checkerx(['ecb', 'cbc', 'cfb']))! as any;
						} else {
							mode = a.mode as any;
						}
						if (a.block_size === '?input') {
							block_size = Input.integer(`请输入块大小（16、24、32）`, Check.enum_checkerx([16n, 24n, 32n]))!;
						} else {
							block_size = a.block_size;
						}
						if (a.key === '?input') {
							key = Input.string(`请输入密钥（长度为16、24、32）`, (value) => ((value.length === 16 || value.length === 24 || value.length === 32) ? null : `长度必须为16、24、32`))!;
						} else {
							key = a.key;
						}
						if (mode === 'cbc' || mode === 'cfb') {
							if (a.iv === '?input') {
								iv = Input.string(`请输入初始向量（长度与块大小相同）`, (value) => ((value.length === Number(block_size)) ? null : `长度必须与块大小相同`))!;
							} else {
								iv = a.iv;
							}
						} else {
							iv = '';
						}
					}
					CoreX.Tool.Data.Encrypt.Rijndael.decrypt_fs(cipher_file, plain_file, mode, block_size, BigInt(key.length), key, iv);
					Output.i(`输出路径：${cipher_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					cipher_file: undefined!,
					plain_file: '?default',
					mode: '?input',
					block_size: '?input',
					key: '?input',
					iv: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', null]]),
				input_forwarder: 'cipher_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.data.encrypt._injector,
});