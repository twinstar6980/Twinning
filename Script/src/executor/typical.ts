namespace Twinning.Script.Executor {

	// #region common

	export type TypicalArgumentExpression<TValue> = TValue | '?automatic' | '?input';

	export type TypicalArgument<TIdentifier extends string, TValue extends any, TRepresentation extends any> = {
		Value: TValue;
		Representation: TRepresentation;
		identifier: TIdentifier;
		stringify: (value: TValue) => string;
		parse: (representation: TRepresentation) => TValue;
		automatic: (argument: any) => null | TValue;
		input: (argument: any, initial?: TValue) => null | TValue;
		condition: (argument: any) => null | TValue;
		default: TypicalArgumentExpression<TRepresentation>;
	};

	export type TypicalBatchArgument<TIdentifier extends string> = TypicalArgument<TIdentifier, StoragePath, string> & {
		item_mapper: (argument: any, value: StoragePath) => StoragePath;
	};

	// ----------------

	export type TypicalMethod<TIdentifier extends string, TArgument extends Array<TypicalArgument<string, any, any>>, TBatchArgument extends Array<TypicalBatchArgument<string>>, TWorkerStore extends Record<string, any>> = {
		Argument: {[TElement in TArgument[number]as TElement['identifier']]: TElement['Value']};
		ArgumentRepresentation: {[TElement in TArgument[number]as TElement['identifier']]: TElement['Representation']};
		identifier: TIdentifier;
		filter: null | ['any' | 'file' | 'directory', RegExp];
		argument: TArgument;
		batch: null | TBatchArgument;
		worker: (argument: {[TElement in TArgument[number]as TElement['identifier']]: TElement['Value']}, store: TWorkerStore) => void;
	};

	export type TypicalMethodConfiguration = {
		filter: null | string;
		argument: Record<string, TypicalArgumentExpression<any>>;
		batch: null | Record<string, TypicalArgumentExpression<string>>;
	};

	// ----------------

	export type TypicalMethodImplementation = {
		activate: () => void;
	};

	// #endregion

	// #region basic

	export function typical_argument_boolean<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			checker: null | ((argument: any, value: boolean) => null | string);
			automatic: null | ((argument: any) => null | boolean);
			condition: null | ((argument: any) => null | boolean);
		},
	): TypicalArgument<TIdentifier, boolean, boolean> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => ConvertHelper.make_boolean_to_string_of_confirmation_character(value),
			parse: (representation) => representation,
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => Console.boolean(true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_integer<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			option: null | Array<bigint>;
			checker: null | ((argument: any, value: bigint) => null | string);
			automatic: null | ((argument: any) => null | bigint);
			condition: null | ((argument: any) => null | bigint);
		},
	): TypicalArgument<TIdentifier, bigint, bigint> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => ConvertHelper.make_integer_to_string(value),
			parse: (representation) => representation,
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => proto.option === null ? Console.integer(true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial) : Console.enumeration(Console.option_integer(proto.option), true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_floater<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			option: null | Array<number>;
			checker: null | ((argument: any, value: number) => null | string);
			automatic: null | ((argument: any) => null | number);
			condition: null | ((argument: any) => null | number);
		},
	): TypicalArgument<TIdentifier, number, number> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => ConvertHelper.make_floater_to_string(value),
			parse: (representation) => representation,
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => proto.option === null ? Console.floater(true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial) : Console.enumeration(Console.option_floater(proto.option), true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_string<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			option: null | Array<string>;
			checker: null | ((argument: any, value: string) => null | string);
			automatic: null | ((argument: any) => null | string);
			condition: null | ((argument: any) => null | string);
		},
	): TypicalArgument<TIdentifier, string, string> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => value,
			parse: (representation) => representation,
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => proto.option === null ? Console.string(true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial) : Console.enumeration(Console.option_string(proto.option), true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_size<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			option: null | Array<StorageSize>;
			checker: null | ((argument: any, value: StorageSize) => null | string);
			automatic: null | ((argument: any) => null | StorageSize);
			condition: null | ((argument: any) => null | StorageSize);
		},
	): TypicalArgument<TIdentifier, StorageSize, string> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => value.emit(),
			parse: (representation) => new StorageSize(representation),
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => proto.option === null ? Console.size(true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial) : Console.enumeration(Console.option_size(proto.option), true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_path<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			rule: ['any' | 'file' | 'directory', 'any' | 'input' | 'output'],
			checker: null | ((argument: any, value: StoragePath) => null | string);
			automatic: null | ((argument: any) => null | StoragePath);
			condition: null | ((argument: any) => null | StoragePath);
		},
	): TypicalArgument<TIdentifier, StoragePath, string> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => value.emit(),
			parse: (representation) => new StoragePath(representation),
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => Console.path(proto.rule[0], proto.rule[1], true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
		};
	}

	export function typical_argument_batch<TIdentifier extends string>(
		proto: {
			identifier: TIdentifier;
			rule: 'any' | 'input' | 'output',
			checker: null | ((argument: any, value: StoragePath) => null | string);
			automatic: null | ((argument: any) => null | StoragePath);
			condition: null | ((argument: any) => null | StoragePath);
			item_mapper: (argument: any, value: StoragePath) => StoragePath;
		},
	): TypicalBatchArgument<TIdentifier> {
		return {
			Value: undefined!,
			Representation: undefined!,
			identifier: proto.identifier,
			stringify: (value) => value.emit(),
			parse: (representation) => new StoragePath(representation),
			automatic: (argument) => proto.automatic === null ? null : proto.automatic(argument),
			input: (argument, initial) => Console.path('directory', proto.rule, true, (value) => (proto.checker === null ? null : proto.checker(argument, value)), initial),
			condition: (argument) => proto.condition === null ? null : proto.condition(argument),
			default: proto.automatic === null ? '?input' : '?automatic',
			item_mapper: proto.item_mapper,
		};
	}

	// ----------------

	export function typical_method<TIdentifier extends string, TArgument extends Array<any>, TBatchArgument extends Array<TypicalBatchArgument<string>>, TWorkerStore extends Record<string, any>>(
		proto: {
			identifier: TIdentifier;
			filter: null | ['any' | 'file' | 'directory', RegExp];
			argument: TArgument;
			batch: null | TBatchArgument;
			worker: (argument: {[TElement in TArgument[number]as TElement['identifier']]: TElement['Value']}, store: TWorkerStore) => void;
		},
	): TypicalMethod<TIdentifier, TArgument, TBatchArgument, TWorkerStore> {
		return {
			Argument: undefined!,
			ArgumentRepresentation: undefined!,
			identifier: proto.identifier,
			filter: proto.filter === null ? null : proto.filter,
			argument: proto.argument,
			batch: proto.batch,
			worker: proto.worker,
		};
	}

	// #endregion

	// #region advance

	export let g_typical_method_disable_name_filter: boolean = false;

	export let g_typical_method_configuration: Record<string, Record<string, TypicalMethodConfiguration>> = {};

	// ----------------

	export function execute_typical_batch_task(
		parent: StoragePath,
		filter: ['any' | 'file' | 'directory', RegExp],
		worker: (item: StoragePath) => void,
	): void {
		let all_item = StorageHelper.list_directory(parent, null, true, false, filter[0] === 'any' || filter[0] === 'file', filter[0] === 'any' || filter[0] === 'directory');
		let valid_item = g_typical_method_disable_name_filter ? all_item : all_item.filter((value) => (filter[1].test(value.name()!)));
		let failed_item = [] as Array<StoragePath>;
		let progress = new TextGenerator.Progress('fraction', false, 40, valid_item.length);
		for (let item of valid_item) {
			progress.increase();
			Console.information(`${progress}`, [item.emit()]);
			try {
				worker(item);
			}
			catch (e) {
				failed_item.push(item);
				Console.error_of(e);
			}
		}
		Console.warning(los('executor.typical:batch_result', all_item.length, valid_item.length, failed_item.length), failed_item.map((it) => it.emit()));
		return;
	}

	// ----------------

	export function request_typical_argument<TRepresentation, TValue>(
		name: string,
		expression: TypicalArgumentExpression<TRepresentation>,
		stringify: (value: TValue) => string,
		parse: (representation: TRepresentation) => TValue,
		automatic: () => null | TValue,
		input: (initial?: TValue) => null | TValue,
		skip: null | TValue,
	): null | TValue {
		if (skip !== null) {
			Console.information(los('executor.typical:argument_skipped', name), []);
			return skip;
		}
		let initial: undefined | TValue = undefined;
		if (expression === '?input') {
			Console.information(los('executor.typical:argument_input', name), []);
		}
		else if (expression === '?automatic') {
			let automatic_value = automatic();
			if (automatic_value === null) {
				Console.information(los('executor.typical:argument_automatic_failed', name), []);
			}
			else {
				initial = automatic_value;
				Console.information(los('executor.typical:argument_automatic', name), [stringify(initial!)]);
			}
		}
		else {
			initial = parse(expression);
			Console.information(los('executor.typical:argument_given', name), [stringify(initial!)]);
		}
		return input(initial);
	}

	export function convert_typical_method<TIdentifier extends string, TArgument extends Array<TypicalArgument<string, any, any>>, TBatchArgument extends Array<TypicalBatchArgument<string>>, TWorkerStore extends Record<string, any>>(
		proto: TypicalMethod<TIdentifier, TArgument, TBatchArgument, TWorkerStore>,
		batch: boolean,
	): Method<typeof proto.ArgumentRepresentation> {
		return {
			identifier: `${proto.identifier}${!batch ? '' : '!batch'}`,
			name: () => (`${!batch ? '' : '[*] '}${los(`executor.implement:${proto.identifier}`)}`),
			worker: (original_argument: Record<string, any>) => {
				let interrupted = false;
				let actual_argument = {} as Record<string, any>;
				for (let argument_index in proto.argument) {
					let argument = proto.argument[argument_index];
					let batch_argument = (!batch || proto.batch === null) ? undefined : proto.batch.find((value) => value.identifier === argument.identifier);
					if (batch_argument !== undefined) {
						argument = batch_argument;
					}
					let argument_value = request_typical_argument(
						`${batch_argument === undefined ? '' : '[*] '}${los(`executor.implement:${proto.identifier}:${argument.identifier}`)}`,
						original_argument[argument.identifier],
						argument.stringify,
						argument.parse,
						() => argument.automatic(actual_argument),
						(initial) => argument.input(actual_argument, initial),
						argument.condition(actual_argument),
					);
					if (argument_value === null) {
						interrupted = true;
						break;
					}
					actual_argument[argument.identifier] = argument_value;
				}
				if (interrupted) {
					return los('executor.typical:method_interrupted');
				}
				let state = false;
				let timer = new Timer();
				timer.start();
				let temporary_store = {} as TWorkerStore;
				if (!batch) {
					try {
						proto.worker(actual_argument as any, temporary_store);
						state = true;
					}
					catch (e) {
						Console.error_of(e);
					}
				}
				else {
					assert_test(proto.batch !== null && proto.batch.length > 0);
					assert_test(proto.filter !== null);
					execute_typical_batch_task(
						actual_argument[proto.argument[0].identifier],
						proto.filter!,
						(item) => {
							let item_argument = {...actual_argument};
							for (let batch_argument of proto.batch!) {
								assert_test(item_argument[batch_argument.identifier] instanceof StoragePath);
								item_argument[batch_argument.identifier] = (item_argument[batch_argument.identifier] as StoragePath).push(batch_argument.item_mapper(actual_argument, item));
							}
							proto.worker(item_argument as any, temporary_store);
						},
					);
					state = true;
				}
				timer.stop();
				return [state, timer.duration()];
			},
			default_argument: ConvertHelper.record_from_array(proto.argument, (index, element) => ([element.identifier, element.default])) as typeof proto.ArgumentRepresentation,
			input_filter: (input) => {
				let state = true;
				if (proto.filter === null) {
					if (input !== null) {
						state &&= false;
					}
				}
				else {
					if (input === null) {
						state &&= false;
					}
					else {
						let input_path = new StoragePath(input);
						if (!batch) {
							state &&= KernelX.Storage[({any: 'exist', file: 'exist_file', directory: 'exist_directory'} as const)[proto.filter[0]]](input_path);
							if (!g_typical_method_disable_name_filter) {
								state &&= proto.filter[1].test(input_path.name() ?? '');
							}
						}
						else {
							state &&= StorageHelper.exist_directory(input_path);
						}
					}
				}
				return state;
			},
			input_forwarder: proto.filter === null ? null : proto.argument[0].identifier as Exclude<keyof typeof proto.ArgumentRepresentation, number>,
		};
	}

	// ----------------

	export function push_typical_method(
		group_identifier: string,
		group: Array<TypicalMethod<string, any, any, any>>,
	): void {
		let group_configuration = g_typical_method_configuration[group_identifier];
		for (let item of group) {
			let item_configuration = group_configuration[item.identifier];
			item.identifier = `${group_identifier}.${item.identifier}`;
			{
				if (item.filter !== null) {
					assert_test(item_configuration.filter !== null);
					item.filter[1] = new RegExp(item_configuration.filter, 'i');
				}
				for (let argument of item.argument as Array<TypicalArgument<string, any, any>>) {
					assert_test(item_configuration.argument[argument.identifier] !== undefined);
					argument.default = item_configuration.argument[argument.identifier];
				}
			}
			if (item.batch !== null) {
				assert_test(item_configuration.batch !== null);
				for (let argument of item.batch as Array<TypicalBatchArgument<string>>) {
					assert_test(item_configuration.batch[argument.identifier] !== undefined);
					argument.default = item_configuration.batch[argument.identifier];
				}
			}
			g_method.push(convert_typical_method(item, false));
			g_method_batch.push(item.batch === null ? null : convert_typical_method(item, true));
		}
		return;
	}

	// #endregion

}
