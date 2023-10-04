namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type TypicalArgumentExpression<Value> = Value | '?automatic' | '?input';

	export type TypicalArgument<ID extends string, Value extends any, GivenValue extends any> = {
		Value: Value;
		GivenValue: GivenValue;
		id: ID;
		initial_echoer: (value: Value) => string;
		given_converter: (argument: any, given: GivenValue) => Value;
		automatic_generator: (argument: any) => null | Value;
		input_generator: (argument: any, initial: Value) => Value;
		condition: (argument: any) => null | Value;
		default: TypicalArgumentExpression<GivenValue>;
	};

	export type TypicalBatchArgument<ID extends string> = TypicalArgument<ID, string, string> & {
		item_mapper: (argument: any, value: string) => string;
	};

	export type TypicalMethod<ID extends string, Argument extends Array<TypicalArgument<string, any, any>>, BatchArgument extends Array<TypicalBatchArgument<string>>> = {
		Argument: { [Element in Argument[number]as Element['id']]: Element['Value'] };
		GivenArgument: { [Element in Argument[number]as Element['id']]: Element['GivenValue'] };
		id: ID;
		filter: ['any' | 'file' | 'directory', RegExp];
		argument: Argument;
		worker: (argument: { [Element in Argument[number]as Element['id']]: Element['Value'] }) => void;
		batch_argument: null | BatchArgument;
		batch_worker: (argument: { [Element in Argument[number]as Element['id']]: Element['Value'] }, temporary: any) => void;
	};

	// ------------------------------------------------

	export function typical_argument_boolean<ID extends string>(
		object: {
			id: ID;
			checker: null | ((argument: any, value: boolean) => null | string);
			automatic: null | ((argument: any) => null | boolean);
			condition: null | ((argument: any) => null | boolean);
			default: TypicalArgumentExpression<boolean>;
		},
	): TypicalArgument<ID, boolean, boolean> {
		return {
			id: object.id,
			initial_echoer: (value) => (make_confirmation_boolean_string(value)),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.boolean(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
		} as TypicalArgument<ID, boolean, boolean>;
	}

	export function typical_argument_integer<ID extends string>(
		object: {
			id: ID;
			option: null | Array<bigint>;
			checker: null | ((argument: any, value: bigint) => null | string);
			automatic: null | ((argument: any) => null | bigint);
			condition: null | ((argument: any) => null | bigint);
			default: TypicalArgumentExpression<bigint>;
		},
	): TypicalArgument<ID, bigint, bigint> {
		return {
			id: object.id,
			initial_echoer: (value) => (make_integer_string(value)),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.integer(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_integer(object.option), null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
		} as TypicalArgument<ID, bigint, bigint>;
	}

	export function typical_argument_floater<ID extends string>(
		object: {
			id: ID;
			option: null | Array<number>;
			checker: null | ((argument: any, value: number) => null | string);
			automatic: null | ((argument: any) => null | number);
			condition: null | ((argument: any) => null | number);
			default: TypicalArgumentExpression<number>;
		},
	): TypicalArgument<ID, number, number> {
		return {
			id: object.id,
			initial_echoer: (value) => (make_number_string(value)),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.floater(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_floater(object.option), null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
		} as TypicalArgument<ID, number, number>;
	}

	export function typical_argument_size<ID extends string>(
		object: {
			id: ID;
			checker: null | ((argument: any, value: bigint) => null | string);
			automatic: null | ((argument: any) => null | bigint);
			condition: null | ((argument: any) => null | bigint);
			default: TypicalArgumentExpression<string>;
		},
	): TypicalArgument<ID, bigint, string> {
		return {
			id: object.id,
			initial_echoer: (value) => (make_size_string(value)),
			given_converter: (argument, given) => (parse_size_string(given)),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.size(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
		} as TypicalArgument<ID, bigint, string>;
	}

	export function typical_argument_string<ID extends string>(
		object: {
			id: ID;
			option: null | Array<string>;
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
			default: TypicalArgumentExpression<string>;
		},
	): TypicalArgument<ID, string, string> {
		return {
			id: object.id,
			initial_echoer: (value) => (value),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.string(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_string(object.option), null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
		} as TypicalArgument<ID, string, string>;
	}

	export function typical_argument_path<ID extends string>(
		object: {
			id: ID;
			rule: ['any' | 'file' | 'directory', 'any' | 'input' | 'output'],
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
			default: TypicalArgumentExpression<string>;
		},
	): TypicalArgument<ID, string, string> {
		return {
			id: object.id,
			initial_echoer: (value) => (value),
			given_converter: (argument, given) => (PathUtility.regularize(given)),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.path(object.rule[0], [object.rule[1], 'none'] as any, null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
		} as TypicalArgument<ID, string, string>;
	}

	export function typical_argument_batch<ID extends string>(
		object: {
			id: ID;
			rule: 'any' | 'input' | 'output',
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
			default: TypicalArgumentExpression<string>;
			item_mapper: (argument: any, value: string) => string;
		},
	): TypicalBatchArgument<ID> {
		return {
			id: object.id,
			initial_echoer: (value) => (value),
			given_converter: (argument, given) => (PathUtility.regularize(given)),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.path('directory', [object.rule, 'none'] as any, null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.default,
			item_mapper: object.item_mapper,
		} as TypicalBatchArgument<ID>;
	}

	// ------------------------------------------------

	export function typical_method<ID extends string, Argument extends Array<TypicalArgument<string, any, any>>, BatchArgument extends Array<TypicalBatchArgument<string>>>(
		object: {
			id: ID;
			filter: ['any' | 'file' | 'directory', RegExp];
			argument: Argument;
			worker: (argument: { [Element in Argument[number]as Element['id']]: Element['Value'] }) => void;
			batch_argument: null | BatchArgument;
			batch_worker: null | ((argument: { [Element in Argument[number]as Element['id']]: Element['Value'] }, temporary: any) => void);
		},
	): TypicalMethod<ID, Argument, BatchArgument> {
		return object as TypicalMethod<ID, Argument, BatchArgument>;
	}

	// ------------------------------------------------

	export function request_typical_argument<Given, Result>(
		name: string,
		given: Given | '?automatic' | '?input',
		initial_echoer: (value: Result) => string,
		given_converter: (given: Given) => Result,
		automatic_generator: () => null | Result,
		input_generator: (initial?: Result) => Result,
		skip_value: null | Result = null,
	): Result {
		if (skip_value !== null) {
			Console.information(los('executor.typical:argument_skipped', name), []);
			return skip_value;
		}
		let initial: Result | undefined = undefined;
		if (given === '?input') {
			Console.information(los('executor.typical:argument_input', name), []);
		} else if (given === '?automatic') {
			let automatic_value = automatic_generator();
			if (automatic_value === null) {
				Console.information(los('executor.typical:argument_automatic_failed', name), []);
			} else {
				initial = automatic_value;
				Console.information(los('executor.typical:argument_automatic', name), [initial_echoer(initial!)]);
			}
		} else {
			initial = given_converter(given);
			Console.information(los('executor.typical:argument_given', name), [initial_echoer(initial!)]);
		}
		return input_generator(initial);
	}

	export function convert_typical_method<ID extends string, Argument extends Array<TypicalArgument<string, any, any>>, BatchArgument extends Array<TypicalBatchArgument<string>>>(
		source: TypicalMethod<ID, Argument, BatchArgument>,
		batch: boolean,
	): Method<typeof source.GivenArgument> {
		return {
			id: `${source.id}${!batch ? '' : '.batch'}`,
			name: () => (`${!batch ? '' : '[*] '}${los(`executor.implement:${source.id}`)}`),
			worker: (given_argument: Record<string, any>) => {
				let state = false;
				let timer = new Timer();
				let final_argument = {} as Record<string, any>;
				for (let argument_index in source.argument) {
					let argument = source.argument[argument_index];
					let batch_argument = (!batch || source.batch_argument === null) ? undefined : source.batch_argument.find((value) => (value.id === argument.id));
					if (batch_argument !== undefined) {
						argument = batch_argument;
					}
					final_argument[argument.id] = request_typical_argument(
						`${batch_argument === undefined ? '' : '[*] '}${los(`executor.implement:${source.id}:${argument.id}`)}`,
						given_argument[argument.id],
						argument.initial_echoer,
						(given) => (argument.given_converter(final_argument, given)),
						() => (argument.automatic_generator(final_argument)),
						(initial) => (argument.input_generator(final_argument, initial)),
						argument.condition(final_argument),
					);
				}
				if (!batch) {
					timer.start();
					try {
						source.worker(final_argument as any);
						state = true;
					} catch (e) {
						Console.error_of(e);
					}
					timer.stop();
				} else {
					assert_test(source.batch_argument !== null);
					timer.start();
					let all_item = KernelX.FileSystem[({ any: 'list', file: 'list_file', directory: 'list_directory' } as const)[source.filter[0]]](final_argument[source.argument[0].id]);
					let valid_item = all_item.filter((value) => (source.filter[1]!.test(value)));
					let failed_item = [] as Array<string>;
					let progress = new TextGenerator.Progress('fraction', false, 40, valid_item.length);
					let temporary = {};
					for (let item of valid_item) {
						progress.increase();
						Console.information(`${progress}`, [`${item}`]);
						try {
							let item_argument = { ...final_argument };
							for (let batch_argument of source.batch_argument) {
								item_argument[batch_argument.id] += '/' + batch_argument.item_mapper(final_argument, item);
							}
							if (source.batch_worker === null) {
								source.worker(item_argument as any);
							} else {
								source.batch_worker(item_argument as any, temporary);
							}
						} catch (e) {
							failed_item.push(item);
							Console.error_of(e);
						}
					}
					timer.stop();
					Console.warning(los('executor.typical:batch_result', all_item.length, valid_item.length, failed_item.length), failed_item);
					state = true;
				}
				return [state, timer.duration()];
			},
			default_argument: record_from_array(source.argument, (index, element) => ([element.id, element.default])) as typeof source.GivenArgument,
			input_filter: (input) => {
				let state = true;
				if (!batch) {
					state &&= KernelX.FileSystem[({ any: 'exist', file: 'exist_file', directory: 'exist_directory' } as const)[source.filter[0]]](input);
					state &&= source.filter[1].test(input);
				} else {
					state &&= KernelX.FileSystem.exist_directory(input);
				}
				return state;
			},
			input_forwarder: source.argument[0].id,
		};
	}

	// ------------------------------------------------

	export function push_typical_method(
		group_id: string,
		list: Array<TypicalMethod<string, any, any>>,
	): void {
		for (let item of list) {
			item.id = `${group_id}.${item.id}`;
			g_method.push(convert_typical_method(item, false));
			if (item.batch_argument !== null) {
				g_method_batch.push(convert_typical_method(item, true));
			}
		}
		return;
	}

	// ------------------------------------------------

}