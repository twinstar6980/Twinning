namespace Twinning.Script.Support.Kairosoft.Game.ModifyProgram {

	// #region platform

	type Platform = 'windows_x32' | 'android_a32' | 'android_a64';

	// ----------------

	function get_program_file_path(
		platform: Platform,
	): string {
		var path = '';
		if (platform === 'windows_x32') {
			path = 'GameAssembly.dll';
		}
		if (platform === 'android_a32') {
			path = 'lib/armeabi-v7a/libil2cpp.so';
		}
		if (platform === 'android_a64') {
			path = 'lib/arm64-v8a/libil2cpp.so';
		}
		return path;
	}

	function get_metadata_file_path(
		platform: Platform,
	): string {
		var path = '';
		if (platform === 'windows_x32') {
			path = 'KairoGames_Data/il2cpp_data/Metadata/global-metadata.dat';
		}
		if (platform === 'android_a32') {
			path = 'assets/bin/Data/Managed/Metadata/global-metadata.dat';
		}
		if (platform === 'android_a64') {
			path = 'assets/bin/Data/Managed/Metadata/global-metadata.dat';
		}
		return path;
	}

	function detect_platform(
		game_directory: string,
	): Array<Platform> {
		let result: Array<Platform> = [];
		for (let platform of ['windows_x32', 'android_a32', 'android_a64'] as Array<Platform>) {
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

	// #region modify

	function search_field_from_dump_data(
		source: Array<string>,
		class_name: string,
		field_name: string,
	): null | {
		address: number;
		access: string;
		static: boolean;
		type: string;
	} {
		let result: null | {
			address: number;
			access: string;
			static: boolean;
			type: string;
		} = null;
		for (let index = 0; index < source.length; index++) {
			let class_match = /^(private|protected|public)? class ([^ ]+)?/.exec(source[index]);
			if (class_match !== null && class_match[2] === class_name) {
				for (; index < source.length; index++) {
					if (source[index] === '}') {
						break;
					}
					let field_match = /^\t(private|protected|public)?( static)? (.+) (.+); \/\/ 0x(.+)$/.exec(source[index]);
					if (field_match !== null && field_match[4] === field_name) {
						result = {
							address: Number.parseInt(field_match[5], 16),
							access: field_match[1],
							static: field_match[2] == ' static',
							type: field_match[3],
						};
						break;
					}
				}
				break;
			}
		}
		return result;
	}

	function search_method_from_dump_data(
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
		let result: Array<{
			address: number;
			access: string;
			static: boolean;
			result: string;
			parameter: string;
		}> = [];
		for (let index = 0; index < source.length; index++) {
			let class_match = /^(private|protected|public)? class ([^ ]+)?/.exec(source[index]);
			if (class_match !== null && class_match[2] === class_name) {
				for (; index < source.length; index++) {
					if (source[index] === '}') {
						break;
					}
					let method_match = /^\t(private|protected|public)?( static)? (.+) (.+)\((.*)\) \{ \}$/.exec(source[index]);
					if (method_match !== null && method_match[4] === method_name) {
						let comment_match = /^\t\/\/ RVA: 0x(.+) Offset: 0x(.+) VA: 0x(.+)$/.exec(source[index - 1]);
						assert_test(comment_match !== null);
						result.push({
							address: Number.parseInt(comment_match[2], 16),
							access: method_match[1],
							static: method_match[2] == ' static',
							result: method_match[3],
							parameter: method_match[5],
						});
					}
				}
				break;
			}
		}
		return result;
	}

	// ----------------

	const k_instruction_code_nop_x32 = 0x90n;

	const k_instruction_code_nop_a32 = 0xE320F000n;

	const k_instruction_code_nop_a64 = 0xD503201Fn;

	function find_call_instruction(
		data: ByteStreamView,
		limit: number,
		address: Array<number>,
		overwrite: boolean,
		platform: Platform,
	): boolean {
		let state = false;
		let end = Math.min(data.size(), data.p() + limit);
		if (platform === 'windows_x32') {
			while (data.p() < end) {
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
					data.u8(k_instruction_code_nop_x32);
					data.u8(k_instruction_code_nop_x32);
					data.u8(k_instruction_code_nop_x32);
					data.u8(k_instruction_code_nop_x32);
					data.u8(k_instruction_code_nop_x32);
				}
				state = true;
				break;
			}
		}
		if (platform === 'android_a32') {
			while (data.p() < end) {
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
					data.u32(k_instruction_code_nop_a32);
				}
				state = true;
				break;
			}
		}
		if (platform === 'android_a64') {
			while (data.p() < end) {
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
					data.u32(k_instruction_code_nop_a64);
				}
				state = true;
				break;
			}
		}
		return state;
	}

	// ----------------

	function modify_program_flat(
		platform: Platform,
		program_file: string,
		metadata_file: string,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		let dump_data: Array<string> = [];
		if (disable_record_encryption || enable_debug_mode) {
			Console.information(`Phase: dump program information`, []);
			let il2cpp_dumper_program_path = ProcessHelper.search_program_ensure('Il2CppDumper.dll', false);
			let il2cpp_dumper_program_result = ProcessHelper.run_process(
				['dotnet'],
				[
					il2cpp_dumper_program_path,
					program_file,
					metadata_file,
				],
				null,
			);
			Console.warning(`The output of Il2CppDumper:`, [il2cpp_dumper_program_result.output]);
			if (!normalize_string_line_feed(il2cpp_dumper_program_result.output).endsWith(`Done!\nPress any key to exit...\n`)) {
				throw new Error(`execute failed by Il2CppDumper`);
			}
			dump_data = KernelX.Storage.read_file_s(`${PathUtility.parent(il2cpp_dumper_program_path)}/dump.cs`).split('\n');
		}
		let symbol_address = {
			CRC64: {
				GetValue: [] as Array<number>,
			},
			Encrypter: {
				Encode: [] as Array<number>,
				Decode: [] as Array<number>,
			},
			RecordStore: {
				ReadRecord: 0,
				WriteRecord: 0,
			},
			MyConfig: {
				_cctor: 0,
				DEBUG: 0,
			},
		};
		if (disable_record_encryption || enable_debug_mode) {
			Console.information(`Phase: parse symbol address`, []);
			{
				let search_result = search_method_from_dump_data(dump_data, 'CRC64', 'GetValue');
				assert_test(search_result.length === 1);
				symbol_address.CRC64.GetValue = search_result.map((value) => (value.address));
			}
			{
				let search_result = search_method_from_dump_data(dump_data, 'Encrypter', 'Encode');
				assert_test(search_result.length === 3);
				symbol_address.Encrypter.Encode = search_result.map((value) => (value.address));
			}
			{
				let search_result = search_method_from_dump_data(dump_data, 'Encrypter', 'Decode');
				assert_test(search_result.length === 3);
				symbol_address.Encrypter.Decode = search_result.map((value) => (value.address));
			}
			{
				let search_result = search_method_from_dump_data(dump_data, 'RecordStore', 'ReadRecord').filter((value) => (!value.static && value.parameter === `int rcId`));
				assert_test(search_result.length === 1);
				symbol_address.RecordStore.ReadRecord = search_result[0].address;
			}
			{
				let search_result = search_method_from_dump_data(dump_data, 'RecordStore', 'WriteRecord').filter((value) => (!value.static && value.parameter === `int rcId, byte[][] data`));
				assert_test(search_result.length === 1);
				symbol_address.RecordStore.WriteRecord = search_result[0].address;
			}
			{
				let search_result = search_method_from_dump_data(dump_data, 'MyConfig', '.cctor');
				assert_test(search_result.length === 1);
				symbol_address.MyConfig._cctor = search_result[0].address;
			}
			{
				let search_result = search_field_from_dump_data(dump_data, 'MyConfig', 'DEBUG');
				assert_test(search_result !== null);
				symbol_address.MyConfig.DEBUG = search_result.address;
			}
			Console.warning(`The symbol address was parsed`, [
				`CRC64.GetValue           ${symbol_address.CRC64.GetValue.map((value) => (value.toString(16))).join(',')}`,
				`Encrypter.Encode         ${symbol_address.Encrypter.Encode.map((value) => (value.toString(16))).join(',')}`,
				`Encrypter.Decode         ${symbol_address.Encrypter.Decode.map((value) => (value.toString(16))).join(',')}`,
				`RecordStore.ReadRecord   ${symbol_address.RecordStore.ReadRecord.toString(16)}`,
				`RecordStore.WriteRecord  ${symbol_address.RecordStore.WriteRecord.toString(16)}`,
				`MyConfig..cctor          ${symbol_address.MyConfig._cctor.toString(16)}`,
				`MyConfig.DEBUG          +${symbol_address.MyConfig.DEBUG.toString(16)}`,
			]);
		}
		Console.information(`Phase: load original program`, []);
		let program_data = KernelX.Storage.read_file(program_file);
		let program_stream = new ByteStreamView(program_data.view().value);
		if (disable_record_encryption) {
			Console.information(`Phase: modify method 'RecordStore.ReadRecord'`, []);
			program_stream.p(symbol_address.RecordStore.ReadRecord);
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Decode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Decode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.CRC64.GetValue, false, platform));
			if (platform === 'windows_x32') {
				// add esp, .. = 83 C4 XX
				assert_test(program_stream.u8() === 0x83n);
				assert_test(program_stream.u8() === 0xC4n);
				program_stream.pr(+1);
				// cmp eax, .. = 3B XX XX
				assert_test(program_stream.u8() === 0x3Bn);
				program_stream.pr(-1);
				program_stream.u8(k_instruction_code_nop_x32);
				program_stream.u8(k_instruction_code_nop_x32);
				program_stream.u8(k_instruction_code_nop_x32);
				// jnz .. = 75 XX
				assert_test(program_stream.u8() === 0x75n);
				program_stream.pr(-1);
				program_stream.u8(k_instruction_code_nop_x32);
				program_stream.u8(k_instruction_code_nop_x32);
			}
			if (platform === 'android_a32') {
				// eor; eor; orrs
				program_stream.pr(+12);
				// bne #X = 1A XX XX XX
				assert_test((program_stream.u32() & 0xFF000000n) === 0x1A000000n);
				program_stream.pr(-4);
				program_stream.u32(k_instruction_code_nop_a32);
			}
			if (platform === 'android_a64') {
				// cmp; mov
				program_stream.pr(+8);
				// bne #X = 54 XX XX XX
				assert_test((program_stream.u32() & 0xFF000000n) === 0x54000000n);
				program_stream.pr(-4);
				program_stream.u32(k_instruction_code_nop_a64);
			}
		}
		if (disable_record_encryption) {
			Console.information(`Phase: modify method 'RecordStore.WriteRecord'`, []);
			program_stream.p(symbol_address.RecordStore.WriteRecord);
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Encode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Encode, true, platform));
		}
		if (enable_debug_mode) {
			Console.information(`Phase: modify method 'MyConfig..cctor'`, []);
			program_stream.p(symbol_address.MyConfig._cctor);
			let search_limit = 512;
			if (platform === 'windows_x32') {
				while (program_stream.p() < symbol_address.MyConfig._cctor + search_limit) {
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
					if (instruction_code !== BigInt(symbol_address.MyConfig.DEBUG)) {
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
					Console.warning(`Warning: the STR instruction for 'MyConfig.DEBUG' was found at ${(program_stream.p() - 4).toString(16)}, but this modification may cause error`, []);
					break;
				}
				assert_test(program_stream.p() !== symbol_address.MyConfig._cctor + search_limit);
			}
			if (platform === 'android_a32') {
				while (program_stream.p() < symbol_address.MyConfig._cctor + search_limit) {
					// strb rX, [rY, #Z] = 111001011100 YYYY XXXX ZZZZZZZZZZZZ
					let instruction_code = program_stream.u32();
					if ((instruction_code & 0b111111111111_0000_0000_000000000000n) !== 0b111001011100_0000_0000_000000000000n) {
						continue;
					}
					if ((instruction_code & 0b000000000000_0000_0000_111111111111n) >> 0n !== BigInt(symbol_address.MyConfig.DEBUG) + 4n) {
						continue;
					}
					program_stream.pr(-4);
					program_stream.u32(instruction_code & 0b111111111111_1111_0000_000000000000n | 14n << 12n | BigInt(symbol_address.MyConfig.DEBUG) << 0n);
					Console.warning(`Warning: the STR instruction for 'MyConfig.DEBUG'+4 was found at ${(program_stream.p() - 4).toString(16)}, but this modification may cause error`, []);
					break;
				}
				assert_test(program_stream.p() !== symbol_address.MyConfig._cctor + search_limit);
			}
			if (platform === 'android_a64') {
				while (program_stream.p() < symbol_address.MyConfig._cctor + search_limit) {
					// strb wX, [xY, #Z] = 0011100100 ZZZZZZZZZZZZ YYYYY XXXXX
					let instruction_code = program_stream.u32();
					if ((instruction_code & 0b1111111111_000000000000_00000_00000n) !== 0b0011100100_000000000000_00000_00000n) {
						continue;
					}
					if ((instruction_code & 0b0000000000_111111111111_00000_00000n) >> 10n !== BigInt(symbol_address.MyConfig.DEBUG) + 4n) {
						continue;
					}
					program_stream.pr(-4);
					program_stream.u32(instruction_code & 0b1111111111_000000000000_11111_00000n | BigInt(symbol_address.MyConfig.DEBUG) << 10n | 30n << 0n);
					Console.warning(`Warning: the STR instruction for 'MyConfig.DEBUG'+4 was found at ${(program_stream.p() - 4).toString(16)}, but this modification may cause error`, []);
					break;
				}
				assert_test(program_stream.p() !== symbol_address.MyConfig._cctor + search_limit);
			}
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