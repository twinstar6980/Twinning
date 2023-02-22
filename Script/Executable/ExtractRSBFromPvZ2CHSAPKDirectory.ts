/** 从 PvZ-2 安卓中文版的 APK 解压目录中提取 RSB */
namespace TwinStar.Script.Executable.ExtractRSBFromPvZ2CHSAPKDirectory {

	export function execute(
	): void {
		// TODO : non-windows & echo selection
		Console.notify('i', los(`请选择需要提取的 APK 解压所得的目录`), []);
		let selected_input_directory = Shell.windows_cli_open_file_dialog(true, false);
		if (selected_input_directory.length === 0) {
			Console.notify('w', los(`已取消操作`), []);
			return;
		}
		Console.notify('t', selected_input_directory[0], []);
		Console.notify('i', los(`请选择输出目录`), []);
		let selected_output_directory = Shell.windows_cli_open_file_dialog(true, false);
		if (selected_output_directory.length === 0) {
			Console.notify('w', los(`已取消操作`), []);
			return;
		}
		Console.notify('t', selected_output_directory[0], []);
		let input_directory = selected_input_directory[0];
		let output_directory = selected_output_directory[0];
		Console.notify('i', los(`提取开始`), [los(`输入目录：{}`, input_directory), los(`输出目录：{}`, output_directory)]);
		Entry.simple_batch_execute(input_directory, ['file', /.+\.rsb\.smf$/i], (item) => {
			let input_file = `${input_directory}/${item}`;
			let output_file = `${output_directory}/${item.slice(0, -4)}`;
			CoreX.Tool.PopCap.ZLib.uncompress_fs(input_file, output_file, 15n, { variant_64: false });
		});
		Console.notify('s', los(`提取完成`), []);
		return;
	}

}

TwinStar.Script.Executable.ExtractRSBFromPvZ2CHSAPKDirectory.execute();