/** 从 PvZ-2 安卓中文版的 APK 解压目录中提取 RSB */
namespace TwinKleS.Script.ExtractRSBFromPvZ2CHSAPKDirectory {

	export function execute(
	): void {
		Console.notify('i', `请选择需要提取的 APK 解压所得的目录`, []);
		let selected_input_directory = Shell.windows_cli_open_file_dialog(true, false);
		if (selected_input_directory.length === 0) {
			Console.notify('w', `已取消操作。`, []);
			return;
		}
		Console.notify('i', `请选择输出目录`, []);
		let selected_output_directory = Shell.windows_cli_open_file_dialog(true, false);
		if (selected_output_directory.length === 0) {
			Console.notify('w', `已取消操作。`, []);
			return;
		}
		let input_directory = selected_input_directory[0];
		let output_directory = selected_output_directory[0];
		Console.notify('i', `输入目录：${input_directory}`, []);
		Console.notify('i', `输出目录：${output_directory}`, []);
		let input_file_list = CoreX.FileSystem.list_file(input_directory).filter((e) => (/.+\.rsb\.smf$/i.test(e)));
		Console.notify('i', `共找到${input_file_list.length}个RSB文件`, []);
		for (let input_file of input_file_list) {
			Console.notify('i', `解压RSB：${input_file} ...`, []);
			let output_file = input_file.slice(0, -4);
			try {
				CoreX.Tool.PopCap.ZLib.uncompress_fs(`${input_directory}/${input_file}`, `${output_directory}/${output_file}`, 15n, false);
				Console.notify('s', `成功`, []);
			} catch (e: any) {
				Console.notify('e', `失败`, []);
				continue;
			}
		}
		Console.notify('i', `提取完毕`, []);
		return;
	}

}

TwinKleS.Script.ExtractRSBFromPvZ2CHSAPKDirectory.execute();