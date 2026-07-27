namespace Twinning.Script.Shell {

	// #region context

	export const name = common_query_context('name').value;

	export const version = common_query_context('version').value;

	// ----------------

	export const is_basic = name === 'basic';

	export const is_assistant = name === 'assistant';

	// #endregion

	// #region callback

	export function callback(
		argument: Array<string>,
	): Array<string> {
		return KernelX.Miscellaneous.callback(argument);
	}

	// #endregion

	// #region common

	export function common_query_context(
		name: 'name' | 'version' | 'terminate',
	): {value: string} {
		let result = callback([
			'query_context',
			name,
		]);
		return {
			value: result[0],
		};
	}

	// #endregion

	// #region basic

	export function basic_output_text(
		text: string,
	): {} {
		TerminateHelper.mark();
		let result = callback([
			'output_text',
			text,
		]);
		TerminateHelper.mark();
		return {
		};
	}

	export function basic_input_text(
	): {text: string} {
		TerminateHelper.mark();
		let result = callback([
			'input_text',
		]);
		TerminateHelper.mark();
		return {
			text: result[0],
		};
	}

	// #endregion

	// #region assistant

	export function assistant_send_message(
		type: 'verbosity' | 'information' | 'warning' | 'error' | 'success' | 'input',
		title: string,
		description: Array<string>,
	): {} {
		TerminateHelper.mark();
		let result = callback([
			'send_message',
			type,
			title,
			...description,
		]);
		TerminateHelper.mark();
		return {
		};
	}

	export function assistant_receive_submission(
		type: 'pause' | 'boolean' | 'integer' | 'floater' | 'string' | 'size' | 'path' | 'enumeration',
		option: Array<string>,
	): {value: string} {
		TerminateHelper.mark();
		let result = callback([
			'receive_submission',
			type,
			...option,
		]);
		TerminateHelper.mark();
		return {
			value: result[0],
		};
	}

	// ----------------

	export function assistant_query_storage_item(
		type: 'user_home' | 'application_shared' | 'application_persistent' | 'application_temporary' | 'application_cache',
	): {target: StoragePath} {
		TerminateHelper.mark();
		let result = callback([
			'query_storage_item',
			type,
		]);
		TerminateHelper.mark();
		return {
			target: new StoragePath(result[0]),
		};
	}

	export function assistant_reveal_storage_item(
		target: StoragePath,
	): {} {
		TerminateHelper.mark();
		let result = callback([
			'reveal_storage_item',
			target.emit(),
		]);
		TerminateHelper.mark();
		return {
		};
	}

	export function assistant_pick_storage_item(
		type: 'load_file' | 'load_directory' | 'save_file',
		multiply: boolean,
		location: StoragePath,
		name: string,
	): {target: Array<StoragePath>} {
		TerminateHelper.mark();
		let result = callback([
			'pick_storage_item',
			type,
			multiply.toString(),
			location.emit(),
			name,
		]);
		TerminateHelper.mark();
		return {
			target: result.map((it) => new StoragePath(it)),
		};
	}

	// ----------------

	export function assistant_push_system_notification(
		title: string,
		description: string,
	): {} {
		TerminateHelper.mark();
		let result = callback([
			'push_system_notification',
			title,
			description,
		]);
		TerminateHelper.mark();
		return {
		};
	}

	// #endregion

}
