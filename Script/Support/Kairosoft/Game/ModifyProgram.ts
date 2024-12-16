namespace Twinning.Script.Support.Kairosoft.Game.ModifyProgram {

	// ------------------------------------------------

	export let g_il2cpp_dumper_program_file: null | string = null;

	// ------------------------------------------------

	type DumpedFieldSignature = {
		address: number,
		access: string,
		static: boolean,
		type: string;
	};

	function search_field_from_dump_data(
		source: Array<string>,
		class_name: string,
		field_name: string,
	): null | DumpedFieldSignature {
		let result: null | DumpedFieldSignature = null;
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
							static: field_match[2] == 'static',
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

	type DumpedMethodSignature = {
		address: number,
		access: string,
		static: boolean,
		result: string,
		parameter: string,
	};

	function search_method_from_dump_data(
		source: Array<string>,
		class_name: string,
		method_name: string,
	): Array<DumpedMethodSignature> {
		let result: Array<DumpedMethodSignature> = [];
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
							static: method_match[2] == 'static',
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

	// ------------------------------------------------

	type Platform = 'windows_x32' | 'android_a32' | 'android_a64';

	function detect_platform(
		game_directory: string,
	): null | Platform {
		let type: null | Platform = null;
		if (KernelX.Storage.exist_file(`${game_directory}/KairoGames.exe`)) {
			if (KernelX.Storage.exist_file(`${game_directory}/GameAssembly.dll`)) {
				type = 'windows_x32';
			}
		}
		if (KernelX.Storage.exist_file(`${game_directory}/AndroidManifest.xml`)) {
			if (KernelX.Storage.exist_file(`${game_directory}/lib/armeabi-v7a/libil2cpp.so`)) {
				type = 'android_a32';
			}
			if (KernelX.Storage.exist_file(`${game_directory}/lib/arm64-v8a/libil2cpp.so`)) {
				type = 'android_a64';
			}
		}
		return type;
	}

	// ------------------------------------------------

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

	// ------------------------------------------------

	export function process_fs(
		target_directory: string,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		assert_test(KernelX.is_windows, `unsupported system, this function only avaliable for windows`);
		Console.information(`Phase : detect game platform`, []);
		let platform = detect_platform(target_directory);
		assert_test(platform !== null);
		Console.information(`The game platform is '${platform}'`, []);
		let program_file: string;
		let metadata_file: string;
		if (platform === 'windows_x32') {
			program_file = `${target_directory}/GameAssembly.dll`;
			metadata_file = `${target_directory}/KairoGames_Data/il2cpp_data/Metadata/global-metadata.dat`;
		}
		else if (platform === 'android_a32') {
			program_file = `${target_directory}/lib/armeabi-v7a/libil2cpp.so`;
			metadata_file = `${target_directory}/assets/bin/Data/Managed/Metadata/global-metadata.dat`;
		}
		else if (platform === 'android_a64') {
			program_file = `${target_directory}/lib/arm64-v8a/libil2cpp.so`;
			metadata_file = `${target_directory}/assets/bin/Data/Managed/Metadata/global-metadata.dat`;
		}
		else {
			assert_fail();
		}
		let program_backup_file = `${target_directory}/.backup/program`;
		Console.information(`Phase : check game file`, []);
		assert_test(KernelX.Storage.exist_file(program_file) || KernelX.Storage.exist_file(program_backup_file));
		assert_test(KernelX.Storage.exist_file(metadata_file));
		if (!KernelX.Storage.exist_file(program_backup_file)) {
			Console.information(`Phase : backup original program`, []);
			KernelX.Storage.copy(program_file, program_backup_file);
		}
		let dump_data: Array<string> = [];
		Console.information(`Phase : dump program information via Il2CppDumper`, []);
		if (disable_record_encryption || enable_debug_mode) {
			let il2cpp_dumper_program_file = g_il2cpp_dumper_program_file !== null ? g_il2cpp_dumper_program_file : ProcessHelper.search_path('Il2CppDumper-x86');
			assert_test(il2cpp_dumper_program_file !== null, `could not find 'Il2CppDumper-x86' program from PATH environment`);
			let dump_result = ProcessHelper.execute(
				il2cpp_dumper_program_file,
				[
					program_backup_file,
					metadata_file,
				],
				KernelX.Process.list_environment_variable(),
			);
			Console.warning(`The output of Il2CppDumper :`, [dump_result.output]);
			assert_test(normalize_string_line_feed(dump_result.output).endsWith(`Done!\nPress any key to exit...\n`), `execute failed by il2cpp_dumper`);
			dump_data = KernelX.Storage.read_file_s(`${PathUtility.parent(il2cpp_dumper_program_file)}/dump.cs`).split('\n');
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
			Console.information(`Phase : parse symbol address from program information`, []);
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
		Console.information(`Phase : load original program`, []);
		let program_data = KernelX.Storage.read_file(program_backup_file);
		let program_stream = new ByteStreamView(program_data.view().value);
		if (disable_record_encryption) {
			Console.information(`Phase : modify method 'RecordStore.ReadRecord'`, []);
			program_stream.p(symbol_address.RecordStore.ReadRecord);
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Decode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Decode, true, platform));
			assert_test (find_call_instruction(program_stream, 0x1000, symbol_address.CRC64.GetValue, false, platform));
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
			Console.information(`Phase : modify method 'RecordStore.WriteRecord'`, []);
			program_stream.p(symbol_address.RecordStore.WriteRecord);
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Encode, true, platform));
			assert_test(find_call_instruction(program_stream, 0x1000, symbol_address.Encrypter.Encode, true, platform));
		}
		if (enable_debug_mode) {
			Console.information(`Phase : modify method 'MyConfig..cctor'`, []);
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
					Console.warning(`Warning : the STR instruction for 'MyConfig.DEBUG' was found at ${(program_stream.p() - 4).toString(16)}, but this modification may cause error`, []);
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
					Console.warning(`Warning : the STR instruction for 'MyConfig.DEBUG'+4 was found at ${(program_stream.p() - 4).toString(16)}, but this modification may cause error`, []);
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
					Console.warning(`Warning : the STR instruction for 'MyConfig.DEBUG'+4 was found at ${(program_stream.p() - 4).toString(16)}, but this modification may cause error`, []);
					break;
				}
				assert_test(program_stream.p() !== symbol_address.MyConfig._cctor + search_limit);
			}
		}
		Console.information(`Phase : save modified program`, []);
		KernelX.Storage.write_file(program_file, program_data);
		return;
	}

	// ------------------------------------------------

}