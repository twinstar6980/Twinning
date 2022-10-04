/** 提供JS交互模式命令行 */
namespace TwinKleS.Script.JSShell {

	export function execute(
	): void {
		Console.notify('i', 'JS交互模式已开启，空输入则执行之前输入的脚本，输入字符 ! 以退出', [])
		while (true) {
			let script = '';
			while (true) {
				let sub_script = Console.string()!;
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
				if (e instanceof Error) {
					Console.notify('e', `${e}\n${e.stack}`, []);
				} else {
					Console.notify('e', `${e}`, []);
				}
			}
		}
		return;
	}

}

TwinKleS.Script.JSShell.execute();