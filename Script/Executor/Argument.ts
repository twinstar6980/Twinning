namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type RequireArgument<T> = T;

	export type RequestArgument<T, ProvideDefaultValue> = T | '?input' | (ProvideDefaultValue extends true ? '?default' : never);

	// ------------------------------------------------

	export function require_argument<Given, Result>(
		name: string,
		given: Given,
		given_converter: (value: Given) => Result,
		checker: Check.Checker<Result>,
	): Result {
		let result: Result;
		result = given_converter(given);
		Console.message('i', los('executor.argument:get', name), [
			`${result}`,
		]);
		if (!checker(result)) {
			Console.message('w', los('executor.argument:invalid'), [
			]);
			throw new Error(`require argument is invalid`);
		}
		return result as Result;
	}

	export function request_argument<Given, Result>(
		name: string,
		given: Given | '?default' | '?input',
		given_converter: (value: Given) => Result,
		default_generator: (() => Result) | null,
		input_generator: (initial?: Result) => Result,
	): Result {
		let initial: Result | undefined;
		if (given === '?input') {
			Console.message('i', los('executor.argument:input', name), [
			]);
			initial = undefined;
		} else if (given === '?default') {
			assert_test(default_generator !== null, `?default is disabled`);
			let default_value = default_generator();
			initial = default_value;
			Console.message('i', los('executor.argument:default', name), [
				`${initial}`,
			]);
		} else {
			initial = given_converter(given);
			Console.message('i', los('executor.argument:get', name), [
				`${initial}`,
			]);
		}
		return input_generator(initial);
	}

	// ------------------------------------------------

	export function query_argument_name(
		method: string,
		argument: string,
	): string {
		return los(`method:${method}$${argument}:name`);
	}

	// ------------------------------------------------

}