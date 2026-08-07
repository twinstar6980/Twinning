namespace Twinning.Script.ProcessHelper {

	// #region context

	export function get_workspace(
	): StoragePath {
		return KernelX.Process.get_workspace();
	}

	export function get_environment(
	): Record<string, string> {
		return KernelX.Process.get_environment();
	}

	// #endregion

	// #region child

	export function run_child(
		program: StoragePath,
		argument: Array<string>,
		command: string,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: StoragePath,
		output: StoragePath,
		error: StoragePath,
	): bigint {
		return KernelX.Process.run_child(program, argument, command, workspace, environment, input, output, error);
	}

	// #endregion

	// #region environment

	let g_environment: null | Record<string, string> = null;

	// ----------------

	export function find_environment(
		name: string,
	): null | string {
		if (g_environment === null) {
			g_environment = get_environment();
		}
		let value = null as null | string;
		for (let item_name in g_environment) {
			if (KernelX.is_windows) {
				if (item_name.toLowerCase() === name.toLowerCase()) {
					value = g_environment[item_name];
					break;
				}
			}
			if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
				if (item_name === name) {
					value = g_environment[item_name];
					break;
				}
			}
		}
		return value;
	}

	// #endregion

	// #region program

	export type ProgramPathMap = Record<string, null | StoragePath>;

	// ----------------

	export function query_program_path(
		name: string,
		map: ProgramPathMap,
	): null | StoragePath {
		return map[name] === undefined ? null : map[name];
	}

	export function search_program_path(
		name: string,
		allow_script: boolean,
	): null | StoragePath {
		let result: null | StoragePath = null;
		let item_delimiter = KernelX.is_windows ? ';' : ':';
		let path_environment = find_environment('PATH');
		assert_test(path_environment !== null);
		let path_list = path_environment.split(item_delimiter).map((it) => new StoragePath(it));
		let path_extension_list = [''];
		if (KernelX.is_windows) {
			path_extension_list.push('.exe');
			if (allow_script) {
				path_extension_list.push('.sh', '.ps1', '.cmd', '.bat');
			}
		}
		if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
			if (allow_script) {
				path_extension_list.push('.sh', '.ps1');
			}
		}
		for (let path_extension of path_extension_list) {
			for (let path of path_list) {
				let current_path = path.join(`${name}${path_extension}`);
				if (StorageHelper.exist_file(current_path)) {
					result = current_path;
					break;
				}
			}
			if (result !== null) {
				break;
			}
		}
		return result;
	}

	export function locate_program_path(
		name: string,
		map: null | ProgramPathMap,
		allow_search: boolean,
		allow_script: boolean,
	): StoragePath {
		let result = map === null ? null : query_program_path(name, map);
		if (result === null && allow_search) {
			result = search_program_path(name, allow_script);
		}
		if (result === null) {
			throw new Error(`could not find '${name}' program from 'PATH' environment`);
		}
		return result;
	}

	// #endregion

	// #region shell

	export type ShellStringStyle = 'as' | 'sh' | 'pwsh' | 'mscargv0' | 'mscargv1';

	// ----------------

	export function escape_shell_string(
		style: ShellStringStyle,
		source: string,
	): string {
		let destination = '';
		if (style === 'as') {
			destination = source;
		}
		if (style === 'sh') {
			destination = source;
			destination = destination.replaceAll(/[ \t\n\r'"|<>&;()*?\[\]$#\\]/g, `\\$&`);
		}
		if (style === 'pwsh') {
			destination = source;
			destination = destination.replaceAll(/[']/g, `''`);
			destination = `'${destination}'`;
		}
		if (style === 'mscargv0') {
			// see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
			destination = source;
			destination = `"${destination}"`;
		}
		if (style === 'mscargv1') {
			// see https://learn.microsoft.com/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
			destination += '"';
			let current_backslash_count = 0;
			for (let element of source) {
				if (element === '\\') {
					current_backslash_count++;
					continue;
				}
				if (element === '"') {
					destination += '\\'.repeat(current_backslash_count * 2 + 1);
				}
				else {
					destination += '\\'.repeat(current_backslash_count);
				}
				destination += element;
				current_backslash_count = 0;
			}
			destination += '\\'.repeat(current_backslash_count * 2);
			destination += '"';
		}
		return destination;
	}

	export function build_shell_script(
		escape: ShellStringStyle | ((source: string) => string),
		command: Array<Array<string | [string]>>,
	): string {
		if (CheckHelper.is_string(escape)) {
			let style = escape;
			escape = (it) => escape_shell_string(style, it);
		}
		return command.map((line) => line.map((part) => CheckHelper.is_string(part) ? part : escape(part[0])).join(' ')).join('\n');
	}

	// #endregion

	// #region execution

	export type ExecutionResult = {
		code: bigint;
		program: StoragePath;
		argument: Array<string>;
		input: string;
		output: string;
		error: string;
	};

	export type ExecutionHost = 'native' | 'termux';

	export type ExecutionInterpreter = 'direct' | 'sh' | 'pwsh' | 'cmd';

	// ----------------

	export function throw_execution_exception(
		result: ExecutionResult,
	): never {
		let error = new Error();
		error.name = 'ProcessExecutionException';
		error.message = [
			`code: ${result.code}`,
			`program: ${result.program.emit()}`,
			`argument:`,
			...result.argument,
			`input:`,
			result.input,
			`output:`,
			result.output,
			`error:`,
			result.error,
		].join('\n');
		throw error;
	}

	// ----------------

	function execute_child_in_native(
		program: StoragePath,
		argument: Array<string>,
		command: string,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
	): ExecutionResult {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary(true);
		using temporary_directory_using = temporary_directory_finalizer;
		let input_file = temporary_directory.join('stdin');
		let output_file = temporary_directory.join('stdout');
		let error_file = temporary_directory.join('stderr');
		StorageHelper.create_file(input_file);
		StorageHelper.create_file(output_file);
		StorageHelper.create_file(error_file);
		StorageHelper.write_file_text(input_file, input);
		let code = run_child(program, argument, command, workspace, environment, input_file, output_file, error_file);
		let get_content = (path: StoragePath): string => ConvertHelper.normalize_string_line_feed(StorageHelper.read_file_text(path));
		return {
			code: code,
			program: program,
			argument: [...argument, command],
			input: input,
			output: get_content(output_file),
			error: get_content(error_file),
		};
	}

	function execute_child_in_termux(
		program: StoragePath,
		argument: Array<string>,
		command: string,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
	): ExecutionResult {
		assert_test(KernelX.is_android);
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		let use_root_fallback = Shell.is_basic;
		using temporary_directory_using = temporary_directory_finalizer;
		if (use_root_fallback) {
			temporary_directory = AndroidHelper.make_temporary_directory('sdcard').join(temporary_directory.name()!);
			StorageHelper.create_directory(temporary_directory);
		}
		using temporary_directory_fallback_finalizer = new Finalizer(() => {
			if (use_root_fallback) {
				StorageHelper.remove(temporary_directory);
			}
		});
		let input_file = temporary_directory.join('stdin');
		let output_file = temporary_directory.join('stdout');
		let error_file = temporary_directory.join('stderr');
		let code_file = temporary_directory.join('exit_code');
		StorageHelper.create_file(input_file);
		StorageHelper.create_file(output_file);
		StorageHelper.create_file(error_file);
		StorageHelper.create_file(code_file);
		StorageHelper.write_file_text(input_file, input);
		let script = build_shell_script('sh', [[
			[`${program.emit_native()}`],
			...argument.slice(1).map((it) => [`${it}`] as [string]),
			`<`,
			[`${input_file.emit_native()}`],
		]]);
		AndroidHelper.application_start_service(
			'com.termux',
			'com.termux.app.RunCommandService',
			'com.termux.RUN_COMMAND',
			{
				'com.termux.RUN_COMMAND_PATH': '/data/data/com.termux/files/usr/bin/bash',
				'com.termux.RUN_COMMAND_STDIN': script,
				'com.termux.RUN_COMMAND_WORKDIR': temporary_directory.emit_native(),
				'com.termux.RUN_COMMAND_BACKGROUND': true,
				'com.termux.RUN_COMMAND_SESSION_ACTION': '0',
				'com.termux.RUN_COMMAND_RESULT_DIRECTORY': temporary_directory.emit_native(),
				'com.termux.RUN_COMMAND_RESULT_SINGLE_FILE': false,
			},
			use_root_fallback,
		);
		let termux_error_code_file = temporary_directory.join('err');
		let termux_error_message_file = temporary_directory.join('errmsg');
		while (!StorageHelper.exist_file(termux_error_code_file)) {
			Kernel.Miscellaneous.Thread.sleep(Kernel.Size.value(200n));
		}
		let get_content = (path: StoragePath): string => ConvertHelper.normalize_string_line_feed(StorageHelper.read_file_text(path));
		let result: ExecutionResult = {
			code: 0n,
			program: program,
			argument: argument,
			input: input,
			output: '',
			error: '',
		};
		let termux_error_code = BigInt(get_content(termux_error_code_file));
		if (termux_error_code !== -1n) {
			result.code = (1n << 32n) + termux_error_code;
			result.error = get_content(termux_error_message_file);
		}
		else {
			result.code = BigInt(get_content(code_file));
			result.output = get_content(output_file);
			result.error = get_content(error_file);
		}
		return result;
	}

	function execute_child_in(
		program: StoragePath,
		argument: Array<string>,
		command: string,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
		host: ExecutionHost,
	): ExecutionResult {
		let result: null | ExecutionResult = null;
		if (host === 'native') {
			result = execute_child_in_native(program, argument, command, workspace, environment, input);
		}
		if (host === 'termux') {
			result = execute_child_in_termux(program, argument, command, workspace, environment, input);
		}
		assert_test(result !== null);
		return result;
	}

	// ----------------

	function execute_child_by_direct(
		program: StoragePath,
		argument: Array<string>,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
		host: ExecutionHost,
		path: ProgramPathMap,
	): ExecutionResult {
		let actual_argument = [] as Array<string>;
		let actual_command = '';
		if (KernelX.is_windows) {
			actual_command = build_shell_script('as', [[
				escape_shell_string('mscargv0', program.emit_native()),
				...argument.map((it) => escape_shell_string('mscargv1', it)),
			]]);
		}
		if (KernelX.is_linux || KernelX.is_macintosh || KernelX.is_android || KernelX.is_iphone) {
			actual_argument = [
				program.emit_native(),
				...argument,
			];
		}
		return execute_child_in(
			program,
			actual_argument,
			actual_command,
			workspace,
			environment,
			input,
			host,
		);
	}

	function execute_child_by_sh(
		program: StoragePath,
		argument: Array<string>,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
		host: ExecutionHost,
		path: ProgramPathMap,
	): ExecutionResult {
		return execute_child_by_direct(
			locate_program_path('sh', path, true, false),
			[
				program.emit_native(),
				...argument,
			],
			workspace,
			environment,
			input,
			host,
			path,
		);
	}

	function execute_child_by_pwsh(
		program: StoragePath,
		argument: Array<string>,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
		host: ExecutionHost,
		path: ProgramPathMap,
	): ExecutionResult {
		return execute_child_by_direct(
			locate_program_path('pwsh', path, true, false),
			[
				`-File`,
				program.emit_native(),
				...argument,
			],
			workspace,
			environment,
			input,
			host,
			path,
		);
	}

	function execute_child_by_cmd(
		program: StoragePath,
		argument: Array<string>,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
		host: ExecutionHost,
		path: ProgramPathMap,
	): ExecutionResult {
		assert_test(KernelX.is_windows);
		return execute_child_in(
			locate_program_path('cmd', path, true, false),
			[],
			build_shell_script('as', [[
				`cmd.exe`,
				`/S`,
				`/C`,
				`"${build_shell_script('mscargv1', [[program.emit_native(), ...argument].map((it) => [it] as [string])])}"`,
			]]),
			workspace,
			environment,
			input,
			host,
		);
	}

	function execute_child_by(
		program: StoragePath,
		argument: Array<string>,
		workspace: StoragePath,
		environment: Record<string, string>,
		input: string,
		host: ExecutionHost,
		interpreter: ExecutionInterpreter,
		path: ProgramPathMap,
	): ExecutionResult {
		let result: null | ExecutionResult = null;
		if (interpreter === 'direct') {
			result = execute_child_by_direct(program, argument, workspace, environment, input, host, path);
		}
		if (interpreter === 'sh') {
			result = execute_child_by_sh(program, argument, workspace, environment, input, host, path);
		}
		if (interpreter === 'pwsh') {
			result = execute_child_by_pwsh(program, argument, workspace, environment, input, host, path);
		}
		if (interpreter === 'cmd') {
			result = execute_child_by_cmd(program, argument, workspace, environment, input, host, path);
		}
		assert_test(result !== null);
		return result;
	}

	// ----------------

	export function execute_child(
		program: string | StoragePath,
		argument: Array<string>,
		workspace: null | StoragePath,
		environment: null | Record<string, string>,
		input: null | string,
		host: ExecutionHost,
		path: ProgramPathMap,
	): ExecutionResult {
		if (workspace === null) {
			workspace = get_workspace();
		}
		if (environment === null) {
			environment = get_environment();
		}
		if (input === null) {
			input = '';
		}
		let program_path: null | StoragePath = null;
		let interpreter: ExecutionInterpreter = 'direct';
		if (program instanceof StoragePath) {
			program_path = program;
		}
		else {
			program_path = query_program_path(program, path);
		}
		if (host === 'native') {
			if (program_path === null) {
				program_path = locate_program_path(program as string, null, true, true);
			}
			let program_extension = (program_path.extension() ?? '').toLowerCase();
			if (program_extension === 'sh') {
				interpreter = 'sh';
			}
			if (program_extension === 'ps1') {
				interpreter = 'pwsh';
			}
			if (program_extension === 'cmd' || program_extension === 'bat') {
				interpreter = 'cmd';
			}
		}
		if (host === 'termux') {
			if (program_path === null) {
				program_path = new StoragePath(program as string);
			}
		}
		assert_test(program_path !== null);
		return execute_child_by(program_path, argument, workspace, environment, input, host, interpreter, path);
	}

	// #endregion

}
