namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type Method<Argument extends Command['argument'] = Command['argument'], InputForwarderKey extends keyof Argument = keyof Argument> = {
		/** ID；用于指定功能，应唯一 */
		id: string;
		/** 名称函数；返回对函数的名称文本 */
		name(): string;
		/** 工作函数；根据参数执行的事务 */
		worker(argument: Argument): Array<string>;
		/** 默认参数；当命令所提供参数未包含所需参数时，提供默认值，若其中属性值为undefined，则表示命令所提供参数必须包含此值 */
		default_argument: Argument;
		/** 输入过滤函数；根据输入值判断此功能是否可进入待选表 */
		input_filter(input: Exclude<Command['input'], null>['value']): boolean;
		/** 输入转发键名；输入值需要转发到工作函数参数的键名 */
		input_forwarder: InputForwarderKey;
	};

	// ------------------------------------------------

	export function method_of<Argument extends Command['argument']>(
		object: {
			id: string;
			name(): string;
			worker(argument: Argument): Array<string>;
			default_argument: Argument;
			input_filter(input: Exclude<Command['input'], null>['value']): boolean;
			input_forwarder: keyof Argument;
		},
	): Method<Argument, keyof Argument> {
		return object;
	}

	// ------------------------------------------------

	export function query_method_name(
		method: string,
	): string {
		return los(`method:${method}:name`);
	}

	// ------------------------------------------------

}