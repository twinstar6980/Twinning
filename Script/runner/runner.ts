namespace Twinning.Script.Runner {

	// #region interface

	export let g_command_notification_time_limit: null | bigint = null;

	// ----------------

	export function run(
		argument: Array<string>,
	): string {
		let command_line = [...argument];
		if (command_line.length === 0) {
			Console.information(los('runner:input_command'), [
				los('runner:input_finish_if_empty'),
			]);
			while (true) {
				let input = Console.path('any', 'input', true, null);
				if (input === null) {
					break;
				}
				command_line.push(input);
			}
		}
		let command = Executor.parse(command_line);
		Console.information(los('runner:all_command_parse'), [
			los('runner:all_command_count', command.length),
		]);
		let duration = 0;
		let progress = new TextGenerator.Progress('fraction', true, 40, command.length);
		let command_log = '';
		for (let item of command) {
			progress.increase();
			Console.information(los('runner:current_command_execute', progress), [
				item.input,
				item.method,
				...record_to_array(item.argument, (key, value) => (`${key}: ${KernelX.JSON.write_s_js(value, true, true, true, true)}`)),
			]);
			let state = Executor.execute(item, Executor.g_method, Executor.g_method_batch);
			if (state === null) {
				command_log += '~';
			}
			else {
				if (state[0] === false) {
					command_log += 'f';
				}
				if (state[0] === true) {
					command_log += 's';
				}
				duration += state[1];
				if (g_command_notification_time_limit !== null && g_command_notification_time_limit <= state[1]) {
					Console.push_system_notification(los('runner:current_command_finish'), los('runner:duration', (state[1] / 1000).toFixed(3)));
				}
			}
		}
		Console.success(los('runner:all_command_finish'), [
			los('runner:duration', (duration / 1000).toFixed(3)),
		]);
		return command_log;
	}

	// #endregion

}