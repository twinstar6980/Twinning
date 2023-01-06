/** 提供JS交互模式命令行 */
namespace TwinStar.Script.JSShell {

	export function execute(
	): void {
		Console.notify('i', localized(`JS交互模式已开启`), [localized(`输入为空则完成一轮输入，若一轮输入为空则结束交互`)]);
		while (true) {
			let script = '';
			while (true) {
				let sub_script = Console.string(null, true);
				if (sub_script === null) {
					break;
				}
				script += sub_script + '\n';
			}
			if (script === '') {
				break;
			}
			try {
				let result = CoreX.Miscellaneous.evaluate(script);
				Console.notify('s', `> ${result}`, []);
			} catch (e: any) {
				Console.notify_error(e);
			}
		}
		return;
	}

}

TwinStar.Script.JSShell.execute();