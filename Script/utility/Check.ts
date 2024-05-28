namespace Twinning.Script.Check {

	// ------------------------------------------------

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

}