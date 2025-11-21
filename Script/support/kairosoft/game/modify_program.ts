namespace Twinning.Script.Support.Kairosoft.Game.ModifyProgram {

	// #region platform

	type Platform = 'windows_intel32' | 'android_arm32' | 'android_arm64';

	// ----------------

	function get_program_file_path(
		platform: Platform,
	): string {
		var path = '';
		if (platform === 'windows_intel32') {
			path = 'GameAssembly.dll';
		}
		if (platform === 'android_arm32') {
			path = 'lib/armeabi-v7a/libil2cpp.so';
		}
		if (platform === 'android_arm64') {
			path = 'lib/arm64-v8a/libil2cpp.so';
		}
		return path;
	}

	function get_metadata_file_path(
		platform: Platform,
	): string {
		var path = '';
		if (platform === 'windows_intel32') {
			path = 'KairoGames_Data/il2cpp_data/Metadata/global-metadata.dat';
		}
		if (platform === 'android_arm32') {
			path = 'assets/bin/Data/Managed/Metadata/global-metadata.dat';
		}
		if (platform === 'android_arm64') {
			path = 'assets/bin/Data/Managed/Metadata/global-metadata.dat';
		}
		return path;
	}

	function detect_platform(
		game_directory: string,
	): Array<Platform> {
		let result: Array<Platform> = [];
		for (let platform of ['windows_intel32', 'android_arm32', 'android_arm64'] as Array<Platform>) {
			if (!KernelX.Storage.exist_file(`${game_directory}/${get_program_file_path(platform)}`)) {
				continue;
			}
			if (!KernelX.Storage.exist_file(`${game_directory}/${get_metadata_file_path(platform)}`)) {
				continue;
			}
			result.push(platform);
		}
		return result;
	}

	// #endregion

	// #region il2cppdumper

	function run_il2cppdumper(
		program_file: string,
		metadata_file: string,
	): Array<string> {
		let dump_directory = HomePath.new_temporary(null, 'directory');
		let il2cppdumper_result = ProcessHelper.run_process(
			['dotnet'],
			[
				ProcessHelper.search_program_ensure('Il2CppDumper.dll', false),
				program_file,
				metadata_file,
				dump_directory,
			],
			null,
			null,
		);
		if (!ConvertHelper.normalize_string_line_feed(il2cppdumper_result.output).endsWith(`Done!\nPress any key to exit...\n`)) {
			throw new Error(`execute failed by Il2CppDumper`);
		}
		let dump_data = KernelX.Storage.read_file_s(`${dump_directory}/dump.cs`).split('\n');
		KernelX.Storage.remove(dump_directory);
		return dump_data;
	}

	// ----------------

	function do_il2cppdumper_search_field_from_dump_data(
		source: Array<string>,
		class_name: string,
		field_name: string,
	): Array<{
		address: number;
		access: string;
		static: boolean;
		type: string;
	}> {
		let result: ReturnType<typeof do_il2cppdumper_search_field_from_dump_data> = [];
		let class_rule = /^(private|protected|public) class ([^ ]+)/;
		let field_rule = /^\t(private|protected|public)( static)? (.+) (.+); \/\/ 0x(.+)$/;
		for (let index = 0; index < source.length; index++) {
			let class_match = class_rule.exec(source[index]);
			if (class_match === null || class_match[2] !== class_name) {
				continue;
			}
			for (; index < source.length; index++) {
				if (source[index] === '}') {
					break;
				}
				let field_match = field_rule.exec(source[index]);
				if (field_match === null || field_match[4] !== field_name) {
					continue;
				}
				result.push({
					address: Number.parseInt(field_match[5], 16),
					access: field_match[1],
					static: field_match[2] !== undefined,
					type: field_match[3],
				});
			}
			break;
		}
		return result;
	}

	function do_il2cppdumper_search_method_from_dump_data(
		source: Array<string>,
		class_name: string,
		method_name: string,
	): Array<{
		address: number;
		access: string;
		static: boolean;
		result: string;
		parameter: string;
	}> {
		let result: ReturnType<typeof do_il2cppdumper_search_method_from_dump_data> = [];
		let class_rule = /^(private|protected|public) class ([^ ]+)/;
		let method_rule = /^\t(private|protected|public)( static)? (.+) (.+)\((.*)\) \{ \}$/;
		let comment_rule = /^\t\/\/ RVA: 0x(.+) Offset: 0x(.+) VA: 0x(.+)$/;
		for (let index = 0; index < source.length; index++) {
			let class_match = class_rule.exec(source[index]);
			if (class_match === null || class_match[2] !== class_name) {
				continue;
			}
			for (; index < source.length; index++) {
				if (source[index] === '}') {
					break;
				}
				let method_match = method_rule.exec(source[index]);
				if (method_match === null || method_match[4] !== method_name) {
					continue;
				}
				let comment_match = comment_rule.exec(source[index - 1]);
				assert_test(comment_match !== null);
				result.push({
					address: Number.parseInt(comment_match[2], 16),
					access: method_match[1],
					static: method_match[2] !== undefined,
					result: method_match[3],
					parameter: method_match[5],
				});
			}
			break;
		}
		return result;
	}

	// #endregion

	// #region asm

	const k_instruction_code_nop_intel32 = 0x90n;

	const k_instruction_code_nop_arm32 = 0xE320F000n;

	const k_instruction_code_nop_arm64 = 0xD503201Fn;

	// ----------------

	function find_call_instruction(
		data: ByteStreamView,
		limit: number,
		address: Array<number>,
		overwrite: boolean,
		platform: Platform,
	): boolean {
		let state = false;
		let dataEnd = Math.min(data.size(), data.p() + limit);
		if (platform === 'windows_intel32') {
			while (data.p() < dataEnd) {
				let instruction_code = data.u8();
				// call #X = E8 XX XX XX XX
				if (instruction_code !== 0xE8n) {
					continue;
				}
				let jump_offset = data.i32();
				let jump_address = data.p() + Number(jump_offset);
				if (!address.includes(jump_address)) {
					data.pr(-4);
					continue;
				}
				if (overwrite) {
					data.pr(-5);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
					data.u8(k_instruction_code_nop_intel32);
				}
				state = true;
				break;
			}
		}
		if (platform === 'android_arm32') {
			while (data.p() < dataEnd) {
				let instruction_code = data.u32();
				// bl #X = EB XX XX XX
				if ((instruction_code & 0xFF000000n) !== 0xEB000000n) {
					continue;
				}
				let jump_offset = instruction_code & 0x00FFFFFFn;
				if ((jump_offset & 0x800000n) === 0x800000n) {
					jump_offset = -(0x1000000n - jump_offset);
				}
				let jump_address = data.p() - 4 + 8 + Number(jump_offset) * 4;
				if (!address.includes(jump_address)) {
					continue;
				}
				if (overwrite) {
					data.pr(-4);
					data.u32(k_instruction_code_nop_arm32);
				}
				state = true;
				break;
			}
		}
		if (platform === 'android_arm64') {
			while (data.p() < dataEnd) {
				let instruction_code = data.u32();
				// bl #X = 97 XX XX XX
				if ((instruction_code & 0xFF000000n) !== 0x97000000n) {
					continue;
				}
				let jump_offset = instruction_code & 0x00FFFFFFn;
				if ((jump_offset & 0x800000n) === 0x800000n) {
					jump_offset = -(0x1000000n - jump_offset);
				}
				let jump_address = data.p() - 4 + Number(jump_offset) * 4;
				if (!address.includes(jump_address)) {
					continue;
				}
				if (overwrite) {
					data.pr(-4);
					data.u32(k_instruction_code_nop_arm64);
				}
				state = true;
				break;
			}
		}
		return state;
	}

	// #endregion

	// #region modify

	function modify_program_flat(
		platform: Platform,
		program_file: string,
		metadata_file: string,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		Console.information(`Phase: dump program information`, []);
		let dump_data = run_il2cppdumper(program_file, metadata_file);
		Console.information(`Phase: parse symbol address`, []);
		let symbol_address = {
			CRC64: {
				GetValue: [] as Array<number>,
			},
			Encrypter: {
				Encode: [] as Array<number>,
				Decode: [] as Array<number>,
			},
			RecordStore: {
				ReadRecord: [] as Array<number>,
				WriteRecord: [] as Array<number>,
			},
			MyConfig: {
				_cctor: [] as Array<number>,
				DEBUG: [] as Array<number>,
			},
		};
		{
			let check_search_result = (
				search_result: Array<{address: number;}>,
				expect_count: number,
				name: string,
			): Array<number> => {
				assert_test(search_result.length === expect_count);
				let address_list = search_result.map((it) => it.address);
				Console.information(`The symbol '${name}' at ${address_list.map((it) => (it.toString(16).padStart(8, '0'))).join(',')}`, []);
				return address_list;
			};
			symbol_address.CRC64.GetValue = check_search_result(
				do_il2cppdumper_search_method_from_dump_data(dump_data, 'CRC64', 'GetValue'),
				1,
				'CRC64.GetValue',
			);
			symbol_address.Encrypter.Encode = check_search_result(
				do_il2cppdumper_search_method_from_dump_data(dump_data, 'Encrypter', 'Encode'),
				3,
				'Encrypter.Encode',
			);
			symbol_address.Encrypter.Decode = check_search_result(
				do_il2cppdumper_search_method_from_dump_data(dump_data, 'Encrypter', 'Decode'),
				3,
				'Encrypter.Decode',
			);
			symbol_address.RecordStore.ReadRecord = check_search_result(
				do_il2cppdumper_search_method_from_dump_data(dump_data, 'RecordStore', 'ReadRecord').filter((value) => (!value.static && value.parameter === `int rcId`)),
				1,
				'RecordStore.ReadRecord',
			);
			symbol_address.RecordStore.WriteRecord = check_search_result(
				do_il2cppdumper_search_method_from_dump_data(dump_data, 'RecordStore', 'WriteRecord').filter((value) => (!value.static && value.parameter === `int rcId, byte[][] data`)),
				1,
				'RecordStore.WriteRecord',
			);
			symbol_address.MyConfig._cctor = check_search_result(
				do_il2cppdumper_search_method_from_dump_data(dump_data, 'MyConfig', '.cctor'),
				1,
				'MyConfig..cctor',
			);
			symbol_address.MyConfig.DEBUG = check_search_result(
				do_il2cppdumper_search_field_from_dump_data(dump_data, 'MyConfig', 'DEBUG'),
				1,
				'MyConfig.DEBUG',
			);
		}
		Console.information(`Phase: load original program`, []);
		let program_data = KernelX.Storage.read_file(program_file);
		let program_stream = new ByteStreamView(program_data.view().value);
		if (disable_record_encryption) {
			Console.information(`Phase: modify method 'RecordStore.ReadRecord'`, []);
			program_stream.p(symbol_address.RecordStore.ReadRecord[0]);
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Decode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Decode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.CRC64.GetValue, false, platform));
			if (platform === 'windows_intel32') {
				// add esp, .. = 83 C4 XX
				assert_test(program_stream.u8() === 0x83n);
				assert_test(program_stream.u8() === 0xC4n);
				program_stream.pr(+1);
				// cmp eax, .. = 3B XX XX
				assert_test(program_stream.u8() === 0x3Bn);
				program_stream.pr(-1);
				program_stream.u8(k_instruction_code_nop_intel32);
				program_stream.u8(k_instruction_code_nop_intel32);
				program_stream.u8(k_instruction_code_nop_intel32);
				// jnz .. = 75 XX
				assert_test(program_stream.u8() === 0x75n);
				program_stream.pr(-1);
				program_stream.u8(k_instruction_code_nop_intel32);
				program_stream.u8(k_instruction_code_nop_intel32);
			}
			if (platform === 'android_arm32') {
				// eor; eor; orrs
				program_stream.pr(+12);
				// bne #X = 1A XX XX XX
				assert_test((program_stream.u32() & 0xFF000000n) === 0x1A000000n);
				program_stream.pr(-4);
				program_stream.u32(k_instruction_code_nop_arm32);
			}
			if (platform === 'android_arm64') {
				// cmp; mov
				program_stream.pr(+8);
				// bne #X = 54 XX XX XX
				assert_test((program_stream.u32() & 0xFF000000n) === 0x54000000n);
				program_stream.pr(-4);
				program_stream.u32(k_instruction_code_nop_arm64);
			}
		}
		if (disable_record_encryption) {
			Console.information(`Phase: modify method 'RecordStore.WriteRecord'`, []);
			program_stream.p(symbol_address.RecordStore.WriteRecord[0]);
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Encode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Encode, true, platform));
		}
		if (enable_debug_mode) {
			Console.information(`Phase: modify method 'MyConfig..cctor'`, []);
			program_stream.p(symbol_address.MyConfig._cctor[0]);
			let program_stream_end = program_stream.p() + 0x200;
			if (platform === 'windows_intel32') {
				while (program_stream.p() < program_stream_end) {
					// mov byte ptr [eax+X], 0 = C6 40 XX 00
					let instruction_code: bigint;
					instruction_code = program_stream.u8();
					if (instruction_code !== 0xC6n) {
						continue;
					}
					instruction_code = program_stream.u8();
					if (instruction_code !== 0x40n) {
						program_stream.pr(-1);
						continue;
					}
					instruction_code = program_stream.u8();
					if (instruction_code !== BigInt(symbol_address.MyConfig.DEBUG[0])) {
						program_stream.pr(-1);
						continue;
					}
					instruction_code = program_stream.u8();
					if (instruction_code !== 0x00n) {
						program_stream.pr(-1);
						continue;
					}
					program_stream.pr(-1);
					program_stream.u8(0x01n);
					break;
				}
			}
			if (platform === 'android_arm32') {
				while (program_stream.p() < program_stream_end) {
					// strb rX, [rY, #Z] = 111001011100 YYYY XXXX ZZZZZZZZZZZZ
					let instruction_code = program_stream.u32();
					if ((instruction_code & 0b111111111111_0000_0000_000000000000n) !== 0b111001011100_0000_0000_000000000000n) {
						continue;
					}
					if ((instruction_code & 0b000000000000_0000_0000_111111111111n) >> 0n !== BigInt(symbol_address.MyConfig.DEBUG[0]) + 4n) {
						continue;
					}
					program_stream.pr(-4);
					program_stream.u32(instruction_code & 0b111111111111_1111_0000_000000000000n | 14n << 12n | BigInt(symbol_address.MyConfig.DEBUG[0]) << 0n);
					break;
				}
			}
			if (platform === 'android_arm64') {
				while (program_stream.p() < program_stream_end) {
					// strb wX, [xY, #Z] = 0011100100 ZZZZZZZZZZZZ YYYYY XXXXX
					let instruction_code = program_stream.u32();
					if ((instruction_code & 0b1111111111_000000000000_00000_00000n) !== 0b0011100100_000000000000_00000_00000n) {
						continue;
					}
					if ((instruction_code & 0b0000000000_111111111111_00000_00000n) >> 10n !== BigInt(symbol_address.MyConfig.DEBUG[0]) + 4n) {
						continue;
					}
					program_stream.pr(-4);
					program_stream.u32(instruction_code & 0b1111111111_000000000000_11111_00000n | BigInt(symbol_address.MyConfig.DEBUG[0]) << 10n | 30n << 0n);
					break;
				}
			}
			assert_test(program_stream.p() !== program_stream_end);
			Console.warning(`Warning: the STR instruction for 'MyConfig.DEBUG'+4 was found at ${(program_stream.p() - 4).toString(16).padStart(8, '0')}, but this modification may cause error`, []);
		}
		Console.information(`Phase: save modified program`, []);
		KernelX.Storage.write_file(program_file, program_data);
		return;
	}

	function modify_program(
		target_directory: string,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		Console.information(`Phase: detect platform`, []);
		let platform_list = detect_platform(target_directory);
		Console.information(`The platform is '${platform_list.join('|')}'`, []);
		assert_test(platform_list.length !== 0);
		for (let platform of platform_list) {
			Console.information(`Phase: modify program of '${platform}'`, []);
			modify_program_flat(
				platform,
				`${target_directory}/${get_program_file_path(platform)}`,
				`${target_directory}/${get_metadata_file_path(platform)}`,
				disable_record_encryption,
				enable_debug_mode,
			);
		}
		Console.information(`Phase: done`, []);
		return;
	}

	// ----------------

	export function process_fs(
		target_directory: string,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		if (!KernelX.is_windows && !KernelX.is_linux && !KernelX.is_macintosh) {
			throw new Error(`unsupported system, this function only avaliable for windows or linux or macintosh`);
		}
		modify_program(target_directory, disable_record_encryption, enable_debug_mode);
		return;
	}

	// #endregion

}
