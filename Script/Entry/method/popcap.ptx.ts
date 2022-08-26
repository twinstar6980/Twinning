/**
 * + popcap.ptx.encode PopCap-PTX 编码
 * + popcap.ptx.decode PopCap-PTX 解码
 */
namespace TwinKleS.Entry.method.popcap.ptx {

	// ------------------------------------------------

	function input_popcap_texture_format(
		message: string,
	): Support.PopCapTexture.Encode.Format {
		let index = Input.number([
			message,
			Support.PopCapTexture.Encode.FormatE.map((e, i) => (`${make_prefix_padded_string(i + 1, ' ', 2)}. ${e}`)),
		], Check.enum_checkerx(Support.PopCapTexture.Encode.FormatE.map((e, i) => (i + 1))))!;
		return Support.PopCapTexture.Encode.FormatE[index - 1];
	}

	function input_image_size(
		message: string,
	): CoreX.Image.ImageSize {
		let width = Input.integer(message, (value) => (value > 0n ? null : `尺寸不可为0`))!;
		let height = Input.integer(null, (value) => (value > 0n ? null : `尺寸不可为0`))!;
		return [width, height];
	}

	// ------------------------------------------------

	type Config = {
	};

	export function _injector(
		config: Config,
	) {
		g_executor_method.push(
			Executor.method_of({
				id: 'popcap.ptx.encode',
				description: 'PopCap-PTX 编码',
				worker(a: Entry.CFSA & {
					image_file: string;
					data_file: string | '?default' | '?input';
					format: Support.PopCapTexture.Encode.Format | '?input';
				}) {
					let image_file: string;
					let data_file: string;
					let format: Support.PopCapTexture.Encode.Format | '?input';
					{
						image_file = a.image_file;
						data_file = ArgumentParser.path(a.data_file, {
							input_message: '请输入输出路径',
							default_value: image_file.replace(/((\.png))?$/i, '.ptx'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.format === '?input') {
							format = input_popcap_texture_format(`请输入使用的纹理格式`);
						} else {
							format = a.format;
						}
					}
					Support.PopCapTexture.Encode.encode_fs(image_file, data_file, format);
					Output.i(`输出路径：${data_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					image_file: undefined!,
					data_file: '?default',
					format: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.png)$/i]]),
				input_forwarder: 'image_file',
			}),
			Executor.method_of({
				id: 'popcap.ptx.decode',
				description: 'PopCap-PTX 解码',
				worker(a: Entry.CFSA & {
					data_file: string;
					image_file: string | '?default' | '?input';
					format: Support.PopCapTexture.Encode.Format | '?input';
					image_size: [bigint, bigint] | '?input';
				}) {
					let data_file: string;
					let image_file: string;
					let format: Support.PopCapTexture.Encode.Format | '?input';
					let image_size: [bigint, bigint];
					{
						data_file = a.data_file;
						image_file = ArgumentParser.path(a.image_file, {
							input_message: '请输入输出路径',
							default_value: data_file.replace(/((\.ptx))?$/i, '.png'),
							must_exist: false,
							if_exist: a.fs_if_exist,
						});
						if (a.format === '?input') {
							format = input_popcap_texture_format(`请输入使用的纹理格式`);
						} else {
							format = a.format;
						}
						if (a.image_size === '?input') {
							image_size = input_image_size(`请输入图像宽、高`);
						} else {
							image_size = a.image_size;
						}
					}
					Support.PopCapTexture.Encode.decode_fs(data_file, image_file, image_size, format);
					Output.i(`输出路径：${image_file}`);
				},
				default_argument: {
					...Entry.k_cfsa,
					data_file: undefined!,
					image_file: '?default',
					format: '?input',
					image_size: '?input',
				},
				input_filter: Entry.file_system_path_test_generator([['file', /.+(\.ptx)$/i]]),
				input_forwarder: 'data_file',
			}),
		);
	}

	// ------------------------------------------------

}

({
	injector: TwinKleS.Entry.method.popcap.ptx._injector,
});