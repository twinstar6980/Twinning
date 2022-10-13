/** 提供JS交互模式命令行 */
namespace TwinKleS.Script.JSShell {

	export function execute(
	): void {
		Console.notify('i', localized(`JS交互模式已开启`), [localized(`空输入以完成此轮输入，输入字符 ! 以退出`)]);
		while (true) {
			let script = '';
			while (true) {
				let sub_script = Console.string(null);
				if (sub_script.length === 0) {
					break;
				}
				script += sub_script + '\n';
			}
			if (script === '!\n') {
				break;
			}
			try {
				let result = CoreX.Misc.evaluate(script);
				Console.notify('s', `> ${result}`, []);
			} catch (e: any) {
				Console.notify_error(e);
			}
		}
		return;
	}

}

TwinKleS.Script.JSShell.execute();