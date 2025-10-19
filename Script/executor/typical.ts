namespace Twinning.Script.Executor {

	// #region common

	export type TypicalArgumentExpression<Value> = Value | '?automatic' | '?input';

	export type TypicalArgument<ID extends string, Value extends any, GivenValue extends any> = {
		Value: Value;
		GivenValue: GivenValue;
		id: ID;
		initial_echoer: (value: Value) => string;
		given_converter: (argument: any, given: GivenValue) => Value;
		automatic_generator: (argument: any) => null | Value;
		input_generator: (argument: any, initial?: Value) => null | Value;
		condition: (argument: any) => null | Value;
		default: TypicalArgumentExpression<GivenValue>;
	};

	export type TypicalBatchArgument<ID extends string> = TypicalArgument<ID, string, string> & {
		item_mapper: (argument: any, value: string) => string;
	};

	// ----------------

	export type TypicalMethod<ID extends string, Argument extends Array<TypicalArgument<string, any, any>>, Batch extends Array<TypicalBatchArgument<string>>, WorkerTemporary extends Record<string, any>> = {
		Argument: { [Element in Argument[number]as Element['id']]: Element['Value'] };
		GivenArgument: { [Element in Argument[number]as Element['id']]: Element['GivenValue'] };
		id: ID;
		filter: null | ['any' | 'file' | 'directory', RegExp];
		argument: Argument;
		batch: null | Batch;
		worker: (argument: { [Element in Argument[number]as Element['id']]: Element['Value'] }, temporary: WorkerTemporary) => void;
	};

	export type TypicalMethodConfiguration = {
		filter: null | string;
		argument: Record<string, TypicalArgumentExpression<any>>;
		batch: null | Record<string, TypicalArgumentExpression<string>>;
	};

	// ----------------

	export type TypicalMethodImplementeModule = {
		activate: () => void;
	};

	// #endregion

	// #region basic

	export function typical_argument_boolean<ID extends string>(
		object: {
			id: ID;
			checker: null | ((argument: any, value: boolean) => null | string);
			automatic: null | ((argument: any) => null | boolean);
			condition: null | ((argument: any) => null | boolean);
		},
	): TypicalArgument<ID, boolean, boolean> {
		return {
			Value: undefined!,
			GivenValue: undefined!,
			id: object.id,
			initial_echoer: (value) => (ConvertHelper.make_boolean_to_string_of_confirmation_character(value)),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.boolean(true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_integer<ID extends string>(
		object: {
			id: ID;
			option: null | Array<bigint>;
			checker: null | ((argument: any, value: bigint) => null | string);
			automatic: null | ((argument: any) => null | bigint);
			condition: null | ((argument: any) => null | bigint);
		},
	): TypicalArgument<ID, bigint, bigint> {
		return {
			Value: undefined!,
			GivenValue: undefined!,
			id: object.id,
			initial_echoer: (value) => (ConvertHelper.make_integer_to_string(value)),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.integer(true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_integer(object.option), true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_floater<ID extends string>(
		object: {
			id: ID;
			option: null | Array<number>;
			checker: null | ((argument: any, value: number) => null | string);
			automatic: null | ((argument: any) => null | number);
			condition: null | ((argument: any) => null | number);
		},
	): TypicalArgument<ID, number, number> {
		return {
			Value: undefined!,
			GivenValue: undefined!,
			id: object.id,
			initial_echoer: (value) => (ConvertHelper.make_floater_to_string(value)),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.floater(true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_floater(object.option), true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_size<ID extends string>(
		object: {
			id: ID;
			option: null | Array<bigint>;
			checker: null | ((argument: any, value: bigint) => null | string);
			automatic: null | ((argument: any) => null | bigint);
			condition: null | ((argument: any) => null | bigint);
		},
	): TypicalArgument<ID, bigint, string> {
		return {
			Value: undefined!,
			GivenValue: undefined!,
			id: object.id,
			initial_echoer: (value) => (ConvertHelper.make_size_to_string(value)),
			given_converter: (argument, given) => (ConvertHelper.parse_size_from_string(given)),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.size(true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_size(object.option), true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_string<ID extends string>(
		object: {
			id: ID;
			option: null | Array<string>;
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
		},
	): TypicalArgument<ID, string, string> {
		return {
			Value: undefined!,
			GivenValue: undefined!,
			id: object.id,
			initial_echoer: (value) => (value),
			given_converter: (argument, given) => (given),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (object.option === null ? Console.string(true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial) : Console.enumeration(Console.option_string(object.option), true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_path<ID extends string>(
		object: {
			id: ID;
			rule: ['any' | 'file' | 'directory', 'any' | 'input' | 'output'],
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
		},
	): TypicalArgument<ID, string, string> {
		return {
			Value: undefined! as string,
			GivenValue: undefined! as string,
			id: object.id,
			initial_echoer: (value) => (value),
			given_converter: (argument, given) => (StorageHelper.regularize(given)),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.path(object.rule[0], object.rule[1], true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_batch<ID extends string>(
		object: {
			id: ID;
			rule: 'any' | 'input' | 'output',
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
			item_mapper: (argument: any, value: string) => string;
		},
	): TypicalBatchArgument<ID> {
		return {
			Value: undefined!,
			GivenValue: undefined!,
			id: object.id,
			initial_echoer: (value) => (value),
			given_converter: (argument, given) => (StorageHelper.regularize(given)),
			automatic_generator: (argument) => (object.automatic === null ? null : object.automatic(argument)),
			input_generator: (argument, initial) => (Console.path('directory', object.rule, true, (value) => (object.checker === null ? null : object.checker(argument, value)), initial)),
			condition: (argument) => (object.condition === null ? null : object.condition(argument)),
			default: object.automatic === null ? '?input' : '?automatic',
			item_mapper: object.item_mapper,
		};
	}

	// ----------------

	export function typical_method<ID extends string, Argument extends Array<TypicalArgument<string, any, any>>, Batch extends Array<TypicalBatchArgument<string>>, WorkerTemporary extends Record<string, any>>(
		object: {
			id: ID;
			filter: null | ['any' | 'file' | 'directory', RegExp];
			argument: Argument;
			batch: null | Batch;
			worker: (argument: { [Element in Argument[number]as Element['id']]: Element['Value'] }, temporary: WorkerTemporary) => void;
		},
	): TypicalMethod<ID, Argument, Batch, WorkerTemporary> {
		return {
			Argument: undefined!,
			GivenArgument: undefined!,
			id: object.id,
			filter: object.filter === null ? null : object.filter,
			argument: object.argument,
			batch: object.batch,
			worker: object.worker,
		};
	}

	// #endregion

	// #region advance

	export let g_typical_method_disable_name_filter: boolean = false;

	export let g_typical_method_configuration: Record<string, Record<string, TypicalMethodConfiguration>> = {};

	// ----------------

	export function execute_typical_batch_task(
		parent: string,
		filter: ['any' | 'file' | 'directory', RegExp],
		worker: (item: string) => void,
	): void {
		let all_item = KernelX.Storage.list_directory(parent, null, filter[0] === 'any' || filter[0] === 'file', filter[0] === 'any' || filter[0] === 'directory');
		let valid_item = g_typical_method_disable_name_filter ? all_item : all_item.filter((value) => (filter[1].test(value)));
		let failed_item = [] as Array<string>;
		let progress = new TextGenerator.Progress('fraction', false, 40, valid_item.length);
		for (let item of valid_item) {
			progress.increase();
			Console.information(`${progress}`, [`${item}`]);
			try {
				worker(item);
			}
			catch (e) {
				failed_item.push(item);
				Console.error_of(e);
			}
		}
		Console.warning(los('executor.typical:batch_result', all_item.length, valid_item.length, failed_item.length), failed_item);
		return;
	}

	// ----------------

	export function request_typical_argument<Given, Result>(
		name: string,
		given: Given | '?automatic' | '?input',
		initial_echoer: (value: Result) => string,
		given_converter: (given: Given) => Result,
		automatic_generator: () => null | Result,
		input_generator: (initial?: Result) => null | Result,
		skip_value: null | Result,
	): null | Result {
		if (skip_value !== null) {
			Console.information(los('executor.typical:argument_skipped', name), []);
			return skip_value;
		}
		let initial: undefined | Result = undefined;
		if (given === '?input') {
			Console.information(los('executor.typical:argument_input', name), []);
		}
		else if (given === '?automatic') {
			let automatic_value = automatic_generator();
			if (automatic_value === null) {
				Console.information(los('executor.typical:argument_automatic_failed', name), []);
			}
			else {
				initial = automatic_value;
				Console.information(los('executor.typical:argument_automatic', name), [initial_echoer(initial!)]);
			}
		}
		else {
			initial = given_converter(given);
			Console.information(los('executor.typical:argument_given', name), [initial_echoer(initial!)]);
		}
		return input_generator(initial);
	}

	export function convert_typical_method<ID extends string, Argument extends Array<TypicalArgument<string, any, any>>, Batch extends Array<TypicalBatchArgument<string>>, WorkerTemporary extends Record<string, any>>(
		source: TypicalMethod<ID, Argument, Batch, WorkerTemporary>,
		batch: boolean,
	): Method<typeof source.GivenArgument> {
		return {
			id: `${source.id}${!batch ? '' : '!batch'}`,
			name: () => (`${!batch ? '' : '[*] '}${los(`executor.implement:${source.id}`)}`),
			worker: (given_argument: Record<string, any>) => {
				let interrupted = false;
				let final_argument = {} as Record<string, any>;
				for (let argument_index in source.argument) {
					let argument = source.argument[argument_index];
					let batch_argument = (!batch || source.batch === null) ? undefined : source.batch.find((value) => (value.id === argument.id));
					if (batch_argument !== undefined) {
						argument = batch_argument;
					}
					let argument_value = request_typical_argument(
						`${batch_argument === undefined ? '' : '[*] '}${los(`executor.implement:${source.id}:${argument.id}`)}`,
						given_argument[argument.id],
						argument.initial_echoer,
						(given) => (argument.given_converter(final_argument, given)),
						() => (argument.automatic_generator(final_argument)),
						(initial) => (argument.input_generator(final_argument, initial)),
						argument.condition(final_argument),
					);
					if (argument_value === null) {
						interrupted = true;
						break;
					}
					final_argument[argument.id] = argument_value;
				}
				if (interrupted) {
					return los('executor.typical:method_interrupted');
				}
				let state = false;
				let timer = new Timer();
				timer.start();
				let temporary_data = {} as WorkerTemporary;
				if (!batch) {
					try {
						source.worker(final_argument as any, temporary_data);
						state = true;
					}
					catch (e) {
						Console.error_of(e);
					}
				}
				else {
					assert_test(source.batch !== null && source.batch.length > 0);
					assert_test(source.filter !== null);
					execute_typical_batch_task(
						final_argument[source.argument[0].id],
						source.filter!,
						(item) => {
							let item_argument = { ...final_argument };
							for (let batch_argument of source.batch!) {
								item_argument[batch_argument.id] += '/' + batch_argument.item_mapper(final_argument, item);
							}
							source.worker(item_argument as any, temporary_data);
						},
					);
					state = true;
				}
				timer.stop();
				return [state, timer.duration()];
			},
			default_argument: ConvertHelper.record_from_array(source.argument, (index, element) => ([element.id, element.default])) as typeof source.GivenArgument,
			input_filter: (input) => {
				let state = true;
				if (source.filter === null) {
					if (input !== null) {
						state &&= false;
					}
				}
				else {
					if (input === null || input === '') {
						state &&= false;
					}
					else if (!batch) {
						state &&= KernelX.Storage[({ any: 'exist', file: 'exist_file', directory: 'exist_directory' } as const)[source.filter[0]]](input);
						if (!g_typical_method_disable_name_filter) {
							state &&= source.filter[1].test(input);
						}
					}
					else {
						state &&= KernelX.Storage.exist_directory(input);
					}
				}
				return state;
			},
			input_forwarder: source.filter === null ? null : source.argument[0].id as Exclude<keyof typeof source.GivenArgument, number>,
		};
	}

	// ----------------

	export function push_typical_method(
		group_id: string,
		group: Array<TypicalMethod<string, any, any, any>>,
	): void {
		let group_configuration = g_typical_method_configuration[group_id];
		for (let item of group) {
			let item_configuration = group_configuration[item.id];
			item.id = `${group_id}.${item.id}`;
			{
				if (item.filter !== null) {
					assert_test(item_configuration.filter !== null);
					item.filter[1] = new RegExp(item_configuration.filter, 'i');
				}
				for (let argument of item.argument as Array<TypicalArgument<string, any, any>>) {
					assert_test(item_configuration.argument[argument.id] !== undefined);
					argument.default = item_configuration.argument[argument.id];
				}
			}
			if (item.batch !== null) {
				assert_test(item_configuration.batch !== null);
				for (let argument of item.batch as Array<TypicalBatchArgument<string>>) {
					assert_test(item_configuration.batch[argument.id] !== undefined);
					argument.default = item_configuration.batch[argument.id];
				}
			}
			g_method.push(convert_typical_method(item, false));
			g_method_batch.push(item.batch === null ? null : convert_typical_method(item, true));
		}
		return;
	}

	// #endregion

}