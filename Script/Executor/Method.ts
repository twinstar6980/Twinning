/** 执行器功能 */
namespace TwinStar.Executor {

	// ------------------------------------------------

	/** 功能 */
	export type Method<Argument extends Command['argument'] = Command['argument'], InputForwarderKey extends keyof Argument = keyof Argument> = {
		/** ID；用于指定功能，应唯一 */
		id: string;
		/** 描述函数；返回对函数的描述文本 */
		descriptor(): string;
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
			descriptor(): string;
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
	 * 查询本地化的功能描述
	 * @param method 功能ID
	 * @returns 本地化的功能描述
	 */
	export function query_method_description(
		method: string,
	): string {
		return localized(`method#${method}:description`);
	}

	// ------------------------------------------------

}