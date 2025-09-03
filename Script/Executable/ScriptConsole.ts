namespace Twinning.Script.Executable.ScriptConsole {

	// #region execute

	export function execute(
	): void {
		Console.information(los('executable.script_console:start'), [
			los('executable.script_console:description'),
		]);
		while (true) {
			let script = '';
			while (true) {
				let text = Console.string(true, null);
				if (text === null) {
					break;
				}
				else if (text.endsWith('\\')) {
					script += text.substring(0, text.length - 1) + '\n';
					continue;
				}
				else {
					script += text;
					break;
				}
			}
			if (script === '') {
				break;
			}
			try {
				let result = KernelX.Miscellaneous.evaluate(script, '<anonymous>', false);
				Console.success(`> ${result}`, []);
			}
			catch (e) {
				Console.error_of(e);
			}
		}
		return;
	}

	// #endregion

}

Twinning.Script.Executable.ScriptConsole.execute();