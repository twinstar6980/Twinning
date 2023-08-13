namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type Command = {
		/** 需要应用的功能，若未指定，则需由用户选择 */
		method: string | null;
		/** 传递给所应用功能工作函数的参数 */
		argument: Record<string, Kernel.JSON.JS_Value>;
		/** 附加输入 */
		input: null | {
			/** 输入值 */
			value: string;
			/** 关闭功能的过滤功能；若指定，则（在未指定建议时）在筛选可用功能时不使用过滤功能，所有功能都会进入待选表 */
			disable_filter: boolean;
		};
	};

	// ------------------------------------------------

	export function parse(
		raw_command: Array<string>,
	): Array<Command> {
		let result: Array<Command> = [];
		let index = 0;
		while (index < raw_command.length) {
			let command: Command = {
				method: null,
				argument: {},
				input: null,
			};
			{
				let input_value = raw_command[index++];
				if (input_value !== '?') {
					command.input = {
						value: input_value,
						disable_filter: false,
					};
					if (index < raw_command.length && raw_command[index] === '-disable_filter') {
						++index;
						command.input.disable_filter = true;
					}
				}
			}
			if (index < raw_command.length && raw_command[index] === '-method') {
				++index;
				command.method = raw_command[index++];
			}
			if (index < raw_command.length && raw_command[index] === '-argument') {
				++index;
				let argument = KernelX.JSON.read_s_js(raw_command[index++]);
				assert_test(argument !== null && typeof argument === 'object' && (argument as Object).constructor.name === 'Object' && !(argument instanceof Array), `argument must be a object`);
				command.argument = argument;
			}
			result.push(command);
		}
		return result;
	}

	export function execute(
		command: Command,
		method: Array<Method>,
	): [null | boolean, number] {
		let result = [undefined!, undefined!] as [null | boolean, any];
		let timer = new Timer();
		timer.start();
		try {
			let selected_method: Method | null;
			if (command.method !== null) {
				let target_method = method.find((e) => (e.id === command.method));
				if (target_method === undefined) {
					selected_method = null;
					result = [null, los('executor.command:method_invalid')];
				} else {
					selected_method = target_method;
				}
			} else {
				if (command.input === null) {
					Console.information(los('executor.command:no_input'), [
					]);
					let input_value = Console.string(null, null);
					Console.information(los('executor.command:should_disable_filter_or_not'), [
					]);
					let input_disable_filter = Console.confirmation(null, null);
					command.input = {
						value: input_value,
						disable_filter: input_disable_filter,
					};
				}
				let valid_method: Array<[bigint, Method]> = [];
				for (let index in method) {
					if (command.input.disable_filter || method[index].input_filter(command.input.value)) {
						valid_method.push([BigInt(index) + 1n, method[index]]);
					}
				}
				if (valid_method.length === 0) {
					selected_method = null;
					result = [null, los('executor.command:method_unavaliable')];
				} else {
					Console.information(los('executor.command:method_select'), [
						los('executor.command:input_null_to_pass'),
					]);
					selected_method = Console.option(valid_method.map((e) => ([e[1], `${e[0]}`, e[1].name()])), true, null);
					if (selected_method === null) {
						result = [null, los('executor.command:method_unselect')];
					}
				}
			}
			if (selected_method !== null) {
				let argument = { ...command.argument };
				if (command.input !== null) {
					argument[selected_method.input_forwarder] = command.input.value;
				}
				for (let key in selected_method.default_argument) {
					if (argument[key] === undefined) {
						argument[key] = selected_method.default_argument[key];
					}
				}
				let message = selected_method.worker(argument);
				result = [true, message];
			}
		} catch (e) {
			result = [false, e];
		}
		timer.stop();
		if (result[0] === null) {
			Console.warning(los('executor.command:finish_skipped'), [result[1] as string]);
		}
		if (result[0] === false) {
			Console.error(los('executor.command:finish_failed', (timer.duration() / 1000).toFixed(3)), []);
			Console.error_of(result[1]);
		}
		if (result[0] === true) {
			Console.success(los('executor.command:finish_succeed', (timer.duration() / 1000).toFixed(3)), result[1] as Array<string>);
		}
		return [result[0], timer.duration()];
	}

	// ------------------------------------------------

}