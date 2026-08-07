namespace Twinning.Script.TerminateHelper {

	// #region utility

	export function mark(
	): void {
		if (ConvertHelper.parse_boolean_from_string(Shell.common_query_context('terminate').value)) {
			trigger();
		}
		return;
	}

	export function check(
		error: any,
	): boolean {
		return error instanceof TerminateException;
	}

	export function trigger(
	): never {
		throw new TerminateException();
	}

	export function rethrow(
		error: any,
	): void {
		if (check(error)) {
			throw error;
		}
		return;
	}

	// #endregion

}
