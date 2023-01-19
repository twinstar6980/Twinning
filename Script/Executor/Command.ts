/** 执行器命令 */
namespace TwinStar.Executor {

	// ------------------------------------------------

	/** 命令 */
	export type Command = {
		/** 需要应用的功能，若未指定，则需由用户选择 */
		method: string | null;
		/** 传递给所应用功能工作函数的参数 */
		argument: Record<string, Core.JSON.JS_Value>;
		/** 附加输入 */
		input: null | {
			/** 输入值 */
			value: string;
			/** 关闭功能的过滤功能；若指定，则（在未指定建议时）在筛选可用功能时不使用过滤功能，所有功能都会进入待选表 */
			disable_filter: boolean;
		};
	};

	// ------------------------------------------------

	/**
	 * 解析字符串列表形式的命令
	 * @param raw_command 字符串
	 * @returns 解析所得的命令
	 */
	export function parse(
		raw_command: Array<string>,
	): Array<Command> {
		let result: Array<Command> = [];
		let i = 0;
		while (i < raw_command.length) {
			let command: Command = {
				method: null,
				argument: {},
				input: null,
			};
			{
				let input_value = raw_command[i++];
				if (input_value !== '?') {
					command.input = {
						value: input_value,
						disable_filter: false,
					};
					if (i < raw_command.length && raw_command[i] === '-disable_filter') {
						++i;
						command.input.disable_filter = true;
					}
				}
			}
			if (i < raw_command.length && raw_command[i] === '-method') {
				++i;
				command.method = raw_command[i++];
			}
			if (i < raw_command.length && raw_command[i] === '-argument') {
				++i;
				let argument = CoreX.JSON.read_s_js(raw_command[i++]);
				if (argument === null || typeof argument !== 'object' || argument instanceof Array) {
					throw new Error(`argument must be a object`);
				}
				command.argument = argument;
			}
			result.push(command);
		}
		return result;
	}

	/**
	 * 执行命令
	 * @param command 命令
	 * @param method 可能待选的功能列表
	 */
	export function execute(
		command: Command,
		method: Array<Method>,
	): void {
		let selected_method: Method | null;
		if (command.method !== null) {
			let target_method = method.find((e) => (e.id === command.method));
			if (target_method === undefined) {
				throw new Error(`invalid method id`);
			}
			selected_method = target_method;
		} else {
			if (command.input === null) {
				Console.notify('i', los(`此条命令未提供输入值，请输入`), []);
				let input_value = Console.string(null);
				Console.notify('i', los(`此条命令是否需禁用功能过滤`), []);
				let input_disable_filter = Console.confirm(null);
				command.input = {
					value: input_value,
					disable_filter: input_disable_filter,
				};
			}
			let method_state: Array<boolean> = [];
			for (let i in method) {
				method_state[i] = command.input.disable_filter || method[i].input_filter(command.input.value);
			}
			if (!method_state.includes(true)) {
				Console.notify('w', los(`未筛选到可选的功能，故跳过此条命令`), []);
				selected_method = null;
			} else {
				Console.notify('i', los(`请选择需要应用的功能`), [los(`输入为空则跳过此条命令`)]);
				selected_method = Console.option(method_state.map((e, i) => (e ? [method[i], `${method[i].descriptor()}`] : null)), null, true);
			}
		}
		if (selected_method !== null) {
			let argument = { ...command.argument };
			if (command.input !== null) {
				argument[selected_method.input_forwarder] = command.input.value;
			}
			for (let key in selected_method.default_argument) {
				if (argument[key] === undefined) {
					if (selected_method.default_argument[key] === undefined) {
						throw new Error(`argument <${key}> is required`);
					}
					argument[key] = selected_method.default_argument[key];
				}
			}
			selected_method.worker(argument);
		}
		return;
	}

	// ------------------------------------------------

}