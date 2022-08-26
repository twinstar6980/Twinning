/** 提供JS交互模式命令行 */
namespace TwinKleS.Script.JSShell {

	export function execute(
	): void {
		Output.i('JS交互模式已开启，空输入则执行之前输入的脚本，输入字符 ! 以退出')
		while (true) {
			let script = '';
			while (true) {
				let sub_script = Input.string(null)!;
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
				Output.d(`${result}`, 0, '>');
			} catch (e: any) {
				if (e instanceof Error) {
					Output.e(`${e}\n${e.stack}`);
				} else {
					Output.e(`${e}`);
				}
			}
		}
		return;
	}

}

TwinKleS.Script.JSShell.execute();