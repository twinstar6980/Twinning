/** 命令执行器 */
namespace TwinKleS.Executor {

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

	/** 功能 */
	export type Method<Argument extends Command['argument'] = Command['argument'], InputForwarderKey extends keyof Argument = keyof Argument> = {
		/** ID；用于指定功能，应唯一 */
		id: string;
		/** 说明 */
		description: string;
		/** 工作函数；根据参数执行的事务 */
		worker(argument: Argument): void;
		/** 默认参数；当命令所提供参数未包含所需参数时，提供默认值，若其中属性值为undefined，则表示命令所提供参数必须包含此值 */
		default_argument: Argument;
		/** 输入过滤函数；根据输入值判断此功能是否可进入待选表 */
		input_filter(input: Exclude<Command['input'], null>['value']): boolean;
		/** 输入转发键名；输入值需要转发到工作函数参数的键名 */
		input_forwarder: InputForwarderKey;
	};

	/**
	 * 自动推导Method的类型模板
	 */
	export function method_of<Argument extends Command['argument']>(
		object: {
			id: string;
			description: string;
			worker(argument: Argument): void;
			default_argument: Argument;
			input_filter(input: Exclude<Command['input'], null>['value']): boolean;
			input_forwarder: keyof Argument;
		},
	): Method<Argument, keyof Argument> {
		return object;
	}

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
					throw new MyError(`argument must be a object`);
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
		let selected_method: Method | null = null;
		if (command.method !== null) {
			let target_method = method.find((e) => (e.id === command.method));
			if (target_method === undefined) {
				throw new MyError(`invalid method id`);
			}
			selected_method = target_method;
		} else {
			if (command.input === null) {
				throw new MyError(`input is null`);
			}
			let valid_method_index: Array<number> = [];
			for (let i in method) {
				if (command.input.disable_filter || method[i].input_filter(command.input.value)) {
					valid_method_index.push(parseInt(i));
				}
			}
			if (valid_method_index.length === 0) {
				Console.notify('w', `未筛选到可选的功能，故跳过此条命令`, []);
			} else {
				Console.notify('i', `请输入功能序号，跳过输入则跳过此条命令`, []);
				let i = Console.option(valid_method_index.map((e) => ([BigInt(e + 1), `${method[e].description}`])), true);
				if (i !== null) {
					selected_method = method[Number(i) - 1];
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
					if (selected_method.default_argument[key] === undefined) {
						throw new MyError(`argument <${key}> is required`);
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