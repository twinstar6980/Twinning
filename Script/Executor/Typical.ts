namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type TypicalArgumentExpression<Value> = Value | '?automatic' | '?input';

	export type TypicalArgument<ID extends string, Value extends any, GivenValue extends any> = {
		Value: Value;
		GivenValue: GivenValue;
		id: ID;
		given_converter: null | ((argument: any, given: GivenValue) => Value);
		automatic_generator: null | ((argument: any) => null | Value);
		input_generator: ((argument: any, initial: Value) => Value);
		condition: null | ((argument: any) => null | Value);
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

	export function typical_argument_confirmation<ID extends string>(
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
			given_converter: (argument, given) => (given),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (Console.confirmation(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
			default: object.default,
		} as TypicalArgument<ID, boolean, boolean>;
	}

	export function typical_argument_number<ID extends string>(
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
			given_converter: (argument, given) => (given),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (object.option === null ? Console.number(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.option(Console.option_number(object.option), null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
			default: object.default,
		} as TypicalArgument<ID, number, number>;
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
			given_converter: (argument, given) => (given),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (object.option === null ? Console.integer(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.option(Console.option_integer(object.option), null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
			default: object.default,
		} as TypicalArgument<ID, bigint, bigint>;
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
			given_converter: (argument, given) => (parse_size_string(given)),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (Console.size(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
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
			given_converter: (argument, given) => (given),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (object.option === null ? Console.string(null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.option(Console.option_string(object.option), null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
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
			given_converter: (argument, given) => (Home.of(given)),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (Console.path(object.rule[0], [object.rule[1], 'none'] as any, null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
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
			given_converter: (argument, given) => (Home.of(given)),
			automatic_generator: object.automatic,
			input_generator: (argument, initial) => (Console.path('directory', [object.rule, 'none'] as any, null, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: object.condition,
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
		given_converter: (given: Given) => Result,
		automatic_generator: () => null | Result,
		input_generator: (initial?: Result) => Result,
		skip_value: null | Result = null,
	): Result {
		if (skip_value !== null) {
			Console.information(los('executor.typical:argument_skipped', name), [
			]);
			return skip_value;
		}
		let initial: Result | undefined = undefined;
		if (given === '?input') {
			Console.information(los('executor.typical:argument_input', name), [
			]);
		} else if (given === '?automatic') {
			let automatic_value = automatic_generator();
			if (automatic_value === null) {
				Console.information(los('executor.typical:argument_automatic_failed', name), [
				]);
			} else {
				initial = automatic_value;
				Console.information(los('executor.typical:argument_automatic', name), [
					`${initial}`,
				]);
			}
		} else {
			initial = given_converter(given);
			Console.information(los('executor.typical:argument_given', name), [
				`${initial}`,
			]);
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
				Console.information(`${batch}`, []);
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
						(given) => (argument.given_converter === null ? given : argument.given_converter(final_argument, given)),
						() => (argument.automatic_generator === null ? null : argument.automatic_generator(final_argument)),
						(initial) => (argument.input_generator(final_argument, initial)),
						argument.condition === null ? null : argument.condition(final_argument),
					);
				}
				if (!batch) {
					source.worker(final_argument as any);
				} else {
					assert_test(source.batch_argument !== null);
					let item_list = KernelX.FileSystem[({ any: 'list', file: 'list_file', directory: 'list_directory' } as const)[source.filter[0]]](final_argument[source.argument[0].id]).filter((value) => (source.filter[1]!.test(value)));;
					if (item_list.length === 0) {
						Console.warning(los('executor.typical:batch_no_item'), [
						]);
					} else {
						let progress = new TextGenerator.Progress('fraction', false, 40, item_list.length);
						let temporary = {};
						for (let item of item_list) {
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
								Console.error_of(e);
								Console.pause();
							}
						}
					}
				}
				return;
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