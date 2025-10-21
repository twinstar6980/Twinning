namespace Twinning.Script.CheckHelper {

	// #region basic

	export type Checker<T> = (value: T) => null | string;

	// ----------------

	export function enumeration_checker<T>(
		rule: Array<T>,
	): Checker<T> {
		return (value) => (rule.includes(value) ? null : los('checker_helper:enumeration_message', `[ ${rule.join(', ')} ]`));
	}

	export function regexp_checker(
		rule: RegExp,
	): Checker<string> {
		return (value) => (rule.test(value) ? null : los('checker_helper:regexp_message', `${rule.source}`));
	}

	// #endregion

	// #region advanced

	export function is_boolean(
		value: unknown,
	): value is boolean {
		return typeof value === 'boolean';
	}

	export function is_bigint(
		value: unknown,
	): value is bigint {
		return typeof value === 'bigint';
	}

	export function is_number(
		value: unknown,
	): value is number {
		return typeof value === 'number';
	}

	export function is_string(
		value: unknown,
	): value is string {
		return typeof value === 'string';
	}

	export function is_object(
		value: unknown,
	): value is object {
		return typeof value === 'object' && value !== null;
	}

	export function is_object_of_object(
		value: unknown,
	): value is Record<any, any> {
		return is_object(value) && value.constructor.name === 'Object';
	}

	export function is_object_of_array(
		value: unknown,
	): value is Array<any> {
		return is_object(value) && value.constructor.name === 'Array';
	}

	// ----------------

	export function is_or<Value, Except extends Value, Fallback>(
		value: Value,
		expect: Except,
		fallback: Fallback,
	): Except | Fallback {
		return value === expect ? value as Except : fallback;
	}

	export function not_or<Value, Except extends Value, Fallback>(
		value: Value,
		expect: Except,
		fallback: Fallback,
	): Exclude<Value, Except> | Fallback {
		return value !== expect ? value as Exclude<Value, Except> : fallback;
	}

	// ----------------

	export function is_undefined_or<Value, Fallback>(
		value: Value | undefined,
		fallback: Fallback,
	): undefined | Fallback {
		return is_or(value, undefined, fallback);
	}

	export function not_undefined_or<Value, Fallback>(
		value: Value | undefined,
		fallback: Fallback,
	): Value | Fallback {
		return not_or(value, undefined, fallback);
	}

	export function is_null_or<Value, Fallback>(
		value: Value | null,
		fallback: Fallback,
	): null | Fallback {
		return is_or(value, null, fallback);
	}

	export function not_null_or<Value, Fallback>(
		value: Value | null,
		fallback: Fallback,
	): Value | Fallback {
		return not_or(value, null, fallback);
	}

	// #endregion

}
