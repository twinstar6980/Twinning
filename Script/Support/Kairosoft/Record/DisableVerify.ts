namespace TwinStar.Script.Support.Kairosoft.Record.DisableVerify {

	// ------------------------------------------------

	export let g_il2cpp_dumper_program_file: null | string = null;

	// ------------------------------------------------

	type MethodSignature = {
		address: number,
		access: string,
		static: boolean,
		result: string,
		parameter: string,
	};

	function search_method(
		source: Array<string>,
		class_name: string,
		method_name: string,
	): Array<MethodSignature> {
		let result: Array<MethodSignature> = [];
		for (let index = 0; index < source.length; ++index) {
			let class_match = /^(private|protected|public)? class ([^ ]+)?/.exec(source[index]);
			if (class_match !== null && class_match[2] === class_name) {
				for (; index < source.length; ++index) {
					if (source[index] === '}') {
						break;
					}
					let method_match = /^\t(private|protected|public)? (static )?(.+) (.+)\((.+)\) \{ \}$/.exec(source[index]);
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

	export function process_fs(
		game_directory: string,
	): void {
		assert_test(Shell.is_windows, `unsupported system, this function only avaliable for windows`);
		assert_test(KernelX.FileSystem.exist_file(`${game_directory}/KairoGames.exe`), `could not find 'KairoGames.exe' file`);
		if (KernelX.FileSystem.exist_file(`${game_directory}/GameAssembly.dll.bak`)) {
			Console.warning(`The 'GameAssembly.dll.bak' already exists, this program maybe patched, are you confirm?`, []);
			if (!Console.boolean(null, null)) {
				return;
			}
		}
		if (KernelX.FileSystem.exist_directory(`${game_directory}/saves`)) {
			Console.warning(`The 'saves' directory already exists, backup it before patch, then decrypt it after patch! are you confirm?`, []);
			if (!Console.boolean(null, null)) {
				return;
			}
		}
		assert_test(!KernelX.FileSystem.exist_file(`${game_directory}/saves`), `please backup 'saves' directory to other `);
		let program_file = `${game_directory}/GameAssembly.dll`;
		let program_backup_file = PathUtility.generate_suffix_path(`${program_file}.bak`);
		let dump_data: Array<string>;
		{
			Console.information(`Try to dump program information via il2cpp_dumper`, []);
			let metadata_file = `${game_directory}/KairoGames_Data/il2cpp_data/Metadata/global-metadata.dat`;
			let il2cpp_dumper_program_file = g_il2cpp_dumper_program_file !== null ? g_il2cpp_dumper_program_file : ProcessHelper.search_path('Il2CppDumper-x86');
			assert_test(il2cpp_dumper_program_file !== null, `could not find 'Il2CppDumper-x86' program from PATH environment`);
			let program_result: ProcessHelper.ExecuteResult;
			program_result = ProcessHelper.execute(
				il2cpp_dumper_program_file,
				[
					program_file,
					metadata_file,
				],
				KernelX.Process.list_environment_variable(),
			);
			Console.warning(`Il2CppDumper output`, [program_result.output]);
			assert_test(normalize_string_line_feed(program_result.output).endsWith(`Done!\nPress any key to exit...\n`), `execute failed by il2cpp_dumper`);
			dump_data = KernelX.FileSystem.read_file_s(`${PathUtility.parent(il2cpp_dumper_program_file)}/dump.cs`).split('\n');
		}
		let symbol_address = {
			CRC64: {
				GetValue: 0,
			},
			Encrypter: {
				Encode: [] as Array<number>,
				Decode: [] as Array<number>,
			},
			RecordStore: {
				ReadRecord: 0,
				WriteRecord: 0,
			},
		};
		{
			Console.information(`Try to parse symbol address`, []);
			let search_list: Array<MethodSignature> = [];
			{
				search_list = search_method(dump_data, 'CRC64', 'GetValue');
				assert_test(search_list.length === 1);
				symbol_address.CRC64.GetValue = search_list[0].address;
			}
			{
				search_list = search_method(dump_data, 'Encrypter', 'Encode');
				assert_test(search_list.length === 3);
				symbol_address.Encrypter.Encode = search_list.map((value) => (value.address));
			}
			{
				search_list = search_method(dump_data, 'Encrypter', 'Decode');
				assert_test(search_list.length === 3);
				symbol_address.Encrypter.Decode = search_list.map((value) => (value.address));
			}
			{
				search_list = search_method(dump_data, 'RecordStore', 'ReadRecord').filter((value) => (!value.static && value.parameter === `int rcId`));
				assert_test(search_list.length === 1);
				symbol_address.RecordStore.ReadRecord = search_list[0].address;
			}
			{
				search_list = search_method(dump_data, 'RecordStore', 'WriteRecord').filter((value) => (!value.static && value.parameter === `int rcId, byte[][] data`));
				assert_test(search_list.length === 1);
				symbol_address.RecordStore.WriteRecord = search_list[0].address;
			}
			Console.warning(`Symbol address`, [
				`CRC64.GetValue          ${symbol_address.CRC64.GetValue.toString(16)}`,
				`Encrypter.Encode        ${symbol_address.Encrypter.Encode.map((value) => (value.toString(16))).join(',')}`,
				`Encrypter.Decode        ${symbol_address.Encrypter.Decode.map((value) => (value.toString(16))).join(',')}`,
				`RecordStore.ReadRecord  ${symbol_address.RecordStore.ReadRecord.toString(16)}`,
				`RecordStore.WriteRecord ${symbol_address.RecordStore.WriteRecord.toString(16)}`,
			]);
		}
		let program_data = KernelX.FileSystem.read_file(program_file);
		let program_stream = new ByteStreamView(program_data.view().value);
		{
			Console.information(`Try to patch kairo.unity.io.RecordStore.ReadRecord`, []);
			program_stream.p(symbol_address.RecordStore.ReadRecord);
			// remove encrypt
			for (let found_count = 0; found_count < 2;) {
				if (program_stream.p() > symbol_address.RecordStore.ReadRecord + 0x1000) {
					throw new Error(`could not find all call of 'Encrypter' (${found_count})`);
				}
				let code_byte = program_stream.u8();
				if (code_byte === 0xE8n) {
					let call_offset = program_stream.i32();
					let call_address = program_stream.p() + Number(call_offset);
					if (!symbol_address.Encrypter.Decode.includes(call_address)) {
						program_stream.pr(-4);
					}
					else {
						found_count++;
						Console.warning(`Find the call of 'Encrypter' at ${(program_stream.p() - 5).toString(16)}`, []);
						program_stream.pr(-5);
						// call .. > nop
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
					}
				}
			}
			// remove verify
			while (true) {
				if (program_stream.p() > symbol_address.RecordStore.ReadRecord + 0x1000) {
					throw new Error(`could not find all call of 'CRC64'`);
				}
				let code_byte = program_stream.u8();
				if (code_byte === 0xE8n) {
					let call_offset = program_stream.i32();
					let call_address = program_stream.p() + Number(call_offset);
					if (symbol_address.CRC64.GetValue != call_address) {
						program_stream.pr(-4);
					}
					else {
						Console.warning(`Find the call of 'CRC64' at ${(program_stream.p() - 5).toString(16)}`, []);
						// add esp, ..
						assert_test(program_stream.u8() === 0x83n);
						assert_test(program_stream.u8() === 0xC4n);
						program_stream.pr(+1);
						// cmp eax, .. > nop
						assert_test(program_stream.u8() === 0x3Bn);
						program_stream.pr(-1);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						// jnz .. > nop
						assert_test(program_stream.u8() === 0x75n);
						program_stream.pr(-1);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						break;
					}
				}
			}
		}
		{
			Console.information(`Try to patch kairo.unity.io.RecordStore.WriteRecord`, []);
			program_stream.p(symbol_address.RecordStore.WriteRecord);
			// remove encrypt
			for (let found_count = 0; found_count < 2;) {
				if (program_stream.p() > symbol_address.RecordStore.WriteRecord + 0x1000) {
					throw new Error(`could not find all call of 'Encrypter' (${found_count})`);
				}
				let code_byte = program_stream.u8();
				if (code_byte === 0xE8n) {
					let call_offset = program_stream.i32();
					let call_address = program_stream.p() + Number(call_offset);
					if (!symbol_address.Encrypter.Encode.includes(call_address)) {
						program_stream.pr(-4);
					}
					else {
						found_count++;
						Console.warning(`Find the call of 'Encrypter' at ${(program_stream.p() - 5).toString(16)}`, []);
						program_stream.pr(-5);
						// call .. > nop
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
						program_stream.u8(0x90n);
					}
				}
			}
		}
		Console.information(`Backing original program`, [PathUtility.name(program_backup_file)]);
		KernelX.FileSystem.copy(program_file, program_backup_file);
		Console.information(`Saving patched program`, []);
		KernelX.FileSystem.write_file(program_file, program_data);
		Console.information(`Done`, []);
		return;
	}

	// ------------------------------------------------

}