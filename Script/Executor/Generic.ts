namespace Twinning.Script.Executor {

	// #region type

	export type Argument = Record<string, null | boolean | bigint | number | string>;

	export type Method<Argument extends Executor.Argument = Executor.Argument, InputForwarderKey extends keyof Argument = keyof Argument> = {
		id: string;
		name(): string;
		worker(argument: Argument): [boolean, number];
		default_argument: Argument;
		input_filter(input: string): boolean;
		input_forwarder: InputForwarderKey;
	};

	export type Command = {
		input: '?none' | string;
		method: '?filtered' | '?unfiltered' | string;
		argument: Argument;
	};

	// #endregion

	// #region basic

	export function parse(
		command_line: Array<string>,
	): Array<Command> {
		let result: Array<Command> = [];
		let command_reader = new CommandLineReader(command_line);
		while (!command_reader.done()) {
			let command: Command = {
				input: '?none',
				method: '?filtered',
				argument: {},
			};
			{
				command.input = command_reader.next_string();
			}
			if (command_reader.check('-method')) {
				command.method = command_reader.next_string();
			}
			if (command_reader.check('-argument')) {
				let argument = KernelX.JSON.read_s_js(command_reader.next_string());
				if (!is_object_of_object(argument)) {
					throw new Error(`argument must be a object`);
				}
				command.argument = argument as Argument;
			}
			result.push(command);
		}
		return result;
	}

	export function execute(
		command: Command,
		method: Array<Method>,
		fallback_method: Array<Method>,
	): null | [boolean, number] {
		let state: [boolean, number] | string = undefined!;
		do {
			let selected_method: null | string = null;
			if (command.method !== '?filtered' && command.method !== '?unfiltered') {
				selected_method = command.method;
			}
			else {
				for (let method_source of [method, fallback_method]) {
					let valid_method: Array<[bigint, Method]> = [];
					for (let index in method_source) {
						if (command.method === '?unfiltered' || command.input === '?none' || method_source[index].input_filter(command.input)) {
							valid_method.push([BigInt(index) + 1n, method_source[index]]);
						}
					}
					if (valid_method.length !== 0) {
						Console.information(los('executor.generic:method_select'), [
							los('executor.generic:method_select_null_to_skip'),
						]);
						selected_method = Console.enumeration(valid_method.map((value) => ([value[1].id, `${value[0]}`, value[1].name()])), true, null);
						if (selected_method !== null) {
							break;
						}
					}
				}
			}
			if (selected_method === null) {
				state = los('executor.generic:method_unavailable');
				break;
			}
			let target_method = method.find((value) => (value.id === selected_method));
			if (target_method === undefined) {
				target_method = fallback_method.find((value) => (value.id === selected_method));
			}
			if (target_method === undefined) {
				state = los('executor.generic:method_invalid');
				break;
			}
			let argument = { ...command.argument };
			if (command.input !== '?none') {
				argument[target_method.input_forwarder] = command.input;
			}
			for (let key in target_method.default_argument) {
				if (argument[key] === undefined) {
					argument[key] = target_method.default_argument[key];
				}
			}
			state = target_method.worker(argument);
		}
		while (false);
		if (is_string(state)) {
			Console.warning(los('executor.generic:finish_skipped'), [state]);
		}
		else {
			if (state[0] === false) {
				Console.error(los('executor.generic:finish_failed'), [los('executor.generic:duration', (state[1] / 1000).toFixed(3))]);
			}
			if (state[0] === true) {
				Console.success(los('executor.generic:finish_succeeded'), [los('executor.generic:duration', (state[1] / 1000).toFixed(3))]);
			}
		}
		return is_string(state) ? null : state;
	}

	// #endregion

	// #region global

	export let g_method: Array<Executor.Method> = [];

	export let g_method_batch: Array<Executor.Method> = [];

	// #endregion

}