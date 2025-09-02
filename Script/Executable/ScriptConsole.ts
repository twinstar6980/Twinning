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
				let sub_script = Console.string(true, null);
				if (sub_script === null) {
					break;
				}
				script += sub_script + '\n';
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