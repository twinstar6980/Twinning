namespace TwinStar.Script.Check {

	// ------------------------------------------------

	export type Checker<T> = (value: T) => boolean;

	export type CheckerX<T> = (value: T) => null | string;

	// ------------------------------------------------

	export function enumeration_checker_x<T>(
		rule: Array<T>,
	): CheckerX<T> {
		return (value) => (rule.includes(value) ? null : los('check:enumeration_message', `[ ${rule.join(' , ')} ]`));
	}

	export function regexp_checker_x(
		rule: RegExp,
	): CheckerX<string> {
		return (value) => (rule.test(value) ? null : los('check:regexp_message', `${rule.source}`));
	}

	// ------------------------------------------------

	export function is_undefined(
		value: any,
	): boolean {
		return value === undefined;
	}

	export function is_null(
		value: any,
	): boolean {
		return value === null;
	}

	export function is_boolean(
		value: any,
	): boolean {
		return typeof value === 'boolean';
	}

	export function is_bigint(
		value: any,
	): boolean {
		return typeof value === 'bigint';
	}

	export function is_number(
		value: any,
	): boolean {
		return typeof value === 'number';
	}

	export function is_string(
		value: any,
	): boolean {
		return typeof value === 'string';
	}

	export function is_object_of_object(
		value: any,
	): boolean {
		return typeof value === 'object' && (value as Object).constructor.name === 'Object';
	}

	export function is_object_of_array(
		value: any,
	): boolean {
		return typeof value === 'object' && (value as Object).constructor.name === 'Array';
	}

	// ------------------------------------------------

}