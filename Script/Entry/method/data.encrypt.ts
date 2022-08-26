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
					CoreX.Tool.Data.Encrypt.XOR.Crypt.crypt_fs(plain_file, cipher_file, key);
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
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.data.encrypt._injector,
});