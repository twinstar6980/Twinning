/** 从 PvZ-2 安卓中国版的 APK 解压目录中提取经过压缩的 RSB 文件 */
namespace TwinStar.Script.Executable.ExtractRSBFromPvZ2CNAPKDirectory {

	export function execute(
	): void {
		// TODO : non-windows & echo selection
		Console.message('i', los('executable.extract_rsb_from_pvz2_cn_apk_directory:select_input_directory'), [
		]);
		let selected_input_directory = ShellExtension.select_path(true, false, '');
		if (selected_input_directory.length === 0) {
			Console.message('w', los('executable.extract_rsb_from_pvz2_cn_apk_directory:cancle'), [
			]);
			return;
		}
		Console.message('t', selected_input_directory[0], []);
		Console.message('i', los('executable.extract_rsb_from_pvz2_cn_apk_directory:select_output_directory'), []);
		let selected_output_directory = ShellExtension.select_path(true, false, '');
		if (selected_output_directory.length === 0) {
			Console.message('w', los('executable.extract_rsb_from_pvz2_cn_apk_directory:cancle'), [
			]);
			return;
		}
		Console.message('t', selected_output_directory[0], []);
		let input_directory = selected_input_directory[0];
		let output_directory = selected_output_directory[0];
		Console.message('i', los('executable.extract_rsb_from_pvz2_cn_apk_directory:extract_start'), [
			los('executable.extract_rsb_from_pvz2_cn_apk_directory:input_directory', input_directory),
			los('executable.extract_rsb_from_pvz2_cn_apk_directory:output_directory', output_directory),
		]);
		Entry.simple_batch_execute(input_directory, ['file', /.+\.rsb\.smf$/i], (item) => {
			let input_file = `${input_directory}/${item}`;
			let output_file = `${output_directory}/${item.slice(0, -4)}`;
			CoreX.Tool.PopCap.ZLib.uncompress_fs(input_file, output_file, 15n, { variant_64: false });
		});
		Console.message('s', los('executable.extract_rsb_from_pvz2_cn_apk_directory:extract_finish'), [
		]);
		return;
	}

}

TwinStar.Script.Executable.ExtractRSBFromPvZ2CNAPKDirectory.execute();