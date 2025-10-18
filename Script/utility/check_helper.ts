namespace Twinning.Script.CheckHelper {

	// #region utility

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

}