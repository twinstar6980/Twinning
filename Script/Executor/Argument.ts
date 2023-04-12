namespace TwinStar.Script.Executor {

	// ------------------------------------------------

	export type Argument<T, ProvideDefaultValue> = T | '?input' | (ProvideDefaultValue extends true ? '?default' : never);

	// ------------------------------------------------

	export function request_argument<Given, Result>(
		name: string,
		given: Given | '?default' | '?input',
		given_converter: (value: Given) => Result,
		default_generator: (() => Result) | null,
		input_generator: (initial?: Result) => Result,
	): Result {
		let initial: Result | undefined;
		if (given === '?input') {
			Console.information(los('executor.argument:input', name), [
			]);
			initial = undefined;
		} else if (given === '?default') {
			assert_test(default_generator !== null, `?default is disabled`);
			let default_value = default_generator();
			initial = default_value;
			Console.information(los('executor.argument:default', name), [
				`${initial}`,
			]);
		} else {
			initial = given_converter(given);
			Console.information(los('executor.argument:get', name), [
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