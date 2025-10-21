namespace Twinning.Script.Executor {

	// #region type

	export type Argument = Record<string, null | boolean | bigint | number | string>;

	export type Method<Argument extends Executor.Argument = Executor.Argument, InputForwarderKey extends Exclude<keyof Argument, number> = Exclude<keyof Argument, number>> = {
		id: string;
		name(): string;
		worker(argument: Argument): string | [boolean, number];
		default_argument: Argument;
		input_filter(input: null | string): boolean;
		input_forwarder: null | InputForwarderKey;
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
				if (!CheckHelper.is_object_of_object(argument)) {
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
		candidate_method: Array<null | Method>,
		candidate_method_fallback: Array<null | Method>,
	): null | [boolean, number] {
		let state: [boolean, number] | string = undefined!;
		do {
			let actual_input: null | string = null;
			if (command.input !== '?none') {
				actual_input = command.input;
			}
			let actual_method: null | string = null;
			if (command.method !== '?filtered' && command.method !== '?unfiltered') {
				actual_method = command.method;
			}
			else {
				for (let candidate_method_current of [candidate_method, candidate_method_fallback]) {
					let available_method: Array<[bigint, Method]> = [];
					for (let index in candidate_method_current) {
						let candidate_method_item = candidate_method_current[index];
						if (candidate_method_item === null) {
							continue;
						}
						if (command.method === '?filtered' && !candidate_method_item.input_filter(actual_input)) {
							continue;
						}
						available_method.push([BigInt(index) + 1n, candidate_method_item]);
					}
					if (available_method.length !== 0) {
						Console.information(los('executor.generic:method_select'), [
							los('executor.generic:method_select_null_to_skip'),
						]);
						actual_method = Console.enumeration(available_method.map((value) => ([value[1].id, `${value[0]}`, value[1].name()])), true, null);
						if (actual_method !== null) {
							break;
						}
					}
				}
			}
			if (actual_method === null) {
				state = los('executor.generic:method_unavailable');
				break;
			}
			let target_method = candidate_method.find((value) => (value !== null && value.id === actual_method));
			if (target_method === undefined) {
				target_method = candidate_method_fallback.find((value) => (value !== null && value.id === actual_method));
			}
			if (target_method === undefined) {
				state = los('executor.generic:method_invalid');
				break;
			}
			assert_test(target_method !== null);
			let actual_argument = { ...command.argument };
			if (actual_input !== null && target_method.input_forwarder !== null) {
				actual_argument[target_method.input_forwarder] = actual_input;
			}
			for (let key in target_method.default_argument) {
				if (actual_argument[key] === undefined) {
					actual_argument[key] = target_method.default_argument[key];
				}
			}
			state = target_method.worker(actual_argument);
		}
		while (false);
		if (CheckHelper.is_string(state)) {
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
		return CheckHelper.is_string(state) ? null : state;
	}

	// #endregion

	// #region global

	export let g_method: Array<null | Method> = [];

	export let g_method_batch: Array<null | Method> = [];

	// #endregion

}
