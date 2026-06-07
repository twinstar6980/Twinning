namespace Twinning.Script.Support.Kairosoft.Game.Program.Modify {

	// #region platform type

	const PlatformTypeX = [
		'windows_intel32',
		'android_arm32',
		'android_arm64',
	] as const;

	export type PlatformType = typeof PlatformTypeX[number];

	export const PlatformTypeE = PlatformTypeX as unknown as PlatformType[];

	// ----------------

	function get_program_file_path(
		platform_type: PlatformType,
	): StoragePath {
		let path = new StoragePath();
		if (platform_type === 'windows_intel32') {
			path.parse('GameAssembly.dll');
		}
		if (platform_type === 'android_arm32') {
			path.parse('lib/armeabi-v7a/libil2cpp.so');
		}
		if (platform_type === 'android_arm64') {
			path.parse('lib/arm64-v8a/libil2cpp.so');
		}
		return path;
	}

	function get_metadata_file_path(
		platform_type: PlatformType,
	): StoragePath {
		let path = new StoragePath();
		if (platform_type === 'windows_intel32') {
			path.parse('KairoGames_Data/il2cpp_data/Metadata/global-metadata.dat');
		}
		if (platform_type === 'android_arm32') {
			path.parse('assets/bin/Data/Managed/Metadata/global-metadata.dat');
		}
		if (platform_type === 'android_arm64') {
			path.parse('assets/bin/Data/Managed/Metadata/global-metadata.dat');
		}
		return path;
	}

	function detect_platform_type(
		game_directory: StoragePath,
	): Array<PlatformType> {
		let result: Array<PlatformType> = [];
		for (let platform_type of PlatformTypeE) {
			if (!StorageHelper.exist_file(game_directory.push(get_program_file_path(platform_type)))) {
				continue;
			}
			if (!StorageHelper.exist_file(game_directory.push(get_metadata_file_path(platform_type)))) {
				continue;
			}
			result.push(platform_type);
		}
		return result;
	}

	// #endregion

	// #region package type

	const PackageTypeX = [
		'flat',
		'zip',
		'apk',
		'apks',
	] as const;

	export type PackageType = typeof PackageTypeX[number];

	export const PackageTypeE = PackageTypeX as unknown as PackageType[];

	// #endregion

	// #region modify

	function modify_flat(
		platform_type: PlatformType,
		program_file: StoragePath,
		metadata_file: StoragePath,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		let assembly_platform_type = platform_type.split('_')[1] as AssemblyHelper.PlatformType;
		Console.information(`phase: dump program information`, []);
		let dump_data = ExternalHelper.run_il2cppdumper_dump(program_file, metadata_file);
		Console.information(`phase: parse symbol address`, []);
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
				search_result: Array<{address: number}>,
				expect_count: number,
				name: string,
			): Array<number> => {
				assert_test(search_result.length === expect_count);
				let address_list = search_result.map((it) => (it.address));
				Console.information(`tip: the symbol '${name}' at ${address_list.map((it) => (it.toString(16).padStart(8, '0'))).join(',')}`, []);
				return address_list;
			};
			symbol_address.CRC64.GetValue = check_search_result(
				ExternalHelper.do_il2cppdumper_search_method_from_dump_data(dump_data, 'CRC64', 'GetValue'),
				1,
				'CRC64.GetValue',
			);
			symbol_address.Encrypter.Encode = check_search_result(
				ExternalHelper.do_il2cppdumper_search_method_from_dump_data(dump_data, 'Encrypter', 'Encode'),
				3,
				'Encrypter.Encode',
			);
			symbol_address.Encrypter.Decode = check_search_result(
				ExternalHelper.do_il2cppdumper_search_method_from_dump_data(dump_data, 'Encrypter', 'Decode'),
				3,
				'Encrypter.Decode',
			);
			symbol_address.RecordStore.ReadRecord = check_search_result(
				ExternalHelper.do_il2cppdumper_search_method_from_dump_data(dump_data, 'RecordStore', 'ReadRecord').filter((value) => (!value.statically && value.parameter === `int rcId`)),
				1,
				'RecordStore.ReadRecord',
			);
			symbol_address.RecordStore.WriteRecord = check_search_result(
				ExternalHelper.do_il2cppdumper_search_method_from_dump_data(dump_data, 'RecordStore', 'WriteRecord').filter((value) => (!value.statically && value.parameter === `int rcId, byte[][] data`)),
				1,
				'RecordStore.WriteRecord',
			);
			symbol_address.MyConfig._cctor = check_search_result(
				ExternalHelper.do_il2cppdumper_search_method_from_dump_data(dump_data, 'MyConfig', '.cctor'),
				1,
				'MyConfig..cctor',
			);
			symbol_address.MyConfig.DEBUG = check_search_result(
				ExternalHelper.do_il2cppdumper_search_field_from_dump_data(dump_data, 'MyConfig', 'DEBUG'),
				1,
				'MyConfig.DEBUG',
			);
		}
		Console.information(`phase: load original program`, []);
		let program_data = StorageHelper.read_file(program_file);
		let program_stream = new ByteStreamView(program_data.view().value);
		if (disable_record_encryption) {
			Console.information(`phase: modify method 'RecordStore.ReadRecord'`, []);
			program_stream.p(symbol_address.RecordStore.ReadRecord[0]);
			assert_test(AssemblyHelper.find_call_instruction(assembly_platform_type, program_stream, 0x1000, symbol_address.Encrypter.Decode, true));
			assert_test(AssemblyHelper.find_call_instruction(assembly_platform_type, program_stream, 0x1000, symbol_address.Encrypter.Decode, true));
			assert_test(AssemblyHelper.find_call_instruction(assembly_platform_type, program_stream, 0x1000, symbol_address.CRC64.GetValue, false));
			if (assembly_platform_type === 'intel32') {
				// add esp, .. = 83 C4 XX
				assert_test(program_stream.u8() === 0x83n);
				assert_test(program_stream.u8() === 0xC4n);
				program_stream.pr(+1);
				// cmp eax, .. = 3B XX XX
				assert_test(program_stream.u8() === 0x3Bn);
				program_stream.pr(-1);
				program_stream.u8(AssemblyHelper.k_instruction_code_nop_intel32);
				program_stream.u8(AssemblyHelper.k_instruction_code_nop_intel32);
				program_stream.u8(AssemblyHelper.k_instruction_code_nop_intel32);
				// jnz .. = 75 XX
				assert_test(program_stream.u8() === 0x75n);
				program_stream.pr(-1);
				program_stream.u8(AssemblyHelper.k_instruction_code_nop_intel32);
				program_stream.u8(AssemblyHelper.k_instruction_code_nop_intel32);
			}
			if (assembly_platform_type === 'arm32') {
				// eor; eor; orrs
				program_stream.pr(+12);
				// bne #X = 1A XX XX XX
				assert_test((program_stream.u32() & 0xFF000000n) === 0x1A000000n);
				program_stream.pr(-4);
				program_stream.u32(AssemblyHelper.k_instruction_code_nop_arm32);
			}
			if (assembly_platform_type === 'arm64') {
				// cmp; mov
				program_stream.pr(+8);
				// bne #X = 54 XX XX XX
				assert_test((program_stream.u32() & 0xFF000000n) === 0x54000000n);
				program_stream.pr(-4);
				program_stream.u32(AssemblyHelper.k_instruction_code_nop_arm64);
			}
		}
		if (disable_record_encryption) {
			Console.information(`phase: modify method 'RecordStore.WriteRecord'`, []);
			program_stream.p(symbol_address.RecordStore.WriteRecord[0]);
			assert_test(AssemblyHelper.find_call_instruction(assembly_platform_type, program_stream, 0x1000, symbol_address.Encrypter.Encode, true));
			assert_test(AssemblyHelper.find_call_instruction(assembly_platform_type, program_stream, 0x1000, symbol_address.Encrypter.Encode, true));
		}
		if (enable_debug_mode) {
			Console.information(`phase: modify method 'MyConfig..cctor'`, []);
			program_stream.p(symbol_address.MyConfig._cctor[0]);
			let program_stream_end = program_stream.p() + 0x200;
			if (assembly_platform_type === 'intel32') {
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
			if (assembly_platform_type === 'arm32') {
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
			if (assembly_platform_type === 'arm64') {
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
			Console.warning(`warning: the STR instruction for 'MyConfig.DEBUG'+4 was found at ${(program_stream.p() - 4).toString(16).padStart(8, '0')}, but this modification may cause error`, []);
		}
		Console.information(`phase: save modified program`, []);
		StorageHelper.write_file(program_file, program_data);
		return;
	}

	function modify(
		target: StoragePath,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		let temporary_directory = StorageHelper.temporary('directory');
		Console.information(`phase: detect package type`, []);
		let package_type = null as PackageType | null;
		if (StorageHelper.exist_directory(target)) {
			package_type = 'flat';
		}
		if (StorageHelper.exist_file(target)) {
			if (target.extension()?.toLowerCase() === 'zip') {
				package_type = 'zip';
			}
			if (target.extension()?.toLowerCase() === 'apk') {
				package_type = 'apk';
			}
			if (target.extension()?.toLowerCase() === 'apks') {
				package_type = 'apks';
			}
		}
		assert_test(package_type !== null);
		Console.information(`tip: the package type is '${package_type}'`, []);
		let package_state = null as null | {
			bundle: null | StoragePath;
			part: Array<{
				path: StoragePath;
				location: null | StoragePath;
				content: Array<StoragePath>;
				platform: Array<PlatformType>;
			}>;
		};
		if (package_type !== 'flat') {
			Console.information(`phase: load package file`, []);
			let package_file = temporary_directory.join('package.zip');
			StorageHelper.copy(target, package_file, false);
			let package_content = ExternalHelper.run_7z_list_content(package_file);
			package_state = {
				bundle: null,
				part: [],
			};
			if (package_type === 'zip' || package_type === 'apk') {
				package_state.part.push({
					path: package_file,
					location: null,
					content: package_content,
					platform: [],
				});
			}
			if (package_type === 'apks') {
				package_state.bundle = package_file;
				for (let package_content_item of package_content) {
					if (!package_content_item.name()!.toLowerCase().endsWith('.apk')) {
						continue;
					}
					let package_part_file = temporary_directory.join('part').join(package_content_item.name()!);
					ExternalHelper.run_7z_extract_content(
						package_file,
						[
							{
								location: package_content_item,
								placement: package_part_file,
							},
						],
					);
					package_state.part.push({
						path: package_part_file,
						location: package_content_item,
						content: ExternalHelper.run_7z_list_content(package_part_file),
						platform: [],
					});
				}
			}
		}
		Console.information(`phase: extract necessary file`, []);
		let target_directory = temporary_directory.join('flat');
		if (package_state === null) {
			for (let platform_type of PlatformTypeE) {
				let metadata_file = get_metadata_file_path(platform_type);
				if (!StorageHelper.exist_file(target_directory.push(metadata_file))) {
					if (StorageHelper.exist_file(target.push(metadata_file))) {
						StorageHelper.copy(target.push(metadata_file), target_directory.push(metadata_file), false);
					}
				}
				let program_file = get_program_file_path(platform_type);
				if (!StorageHelper.exist_file(target_directory.push(program_file))) {
					if (StorageHelper.exist_file(target.push(program_file))) {
						StorageHelper.copy(target.push(program_file), target_directory.push(program_file), false);
					}
				}
			}
		}
		else {
			for (let package_part of package_state.part) {
				for (let platform_type of PlatformTypeE) {
					let metadata_file = get_metadata_file_path(platform_type);
					if (!StorageHelper.exist_file(target_directory.push(metadata_file))) {
						let metadata_file_in_package = package_part.content.find((it) => it.emit() === metadata_file.emit());
						if (metadata_file_in_package !== undefined) {
							ExternalHelper.run_7z_extract_content(
								package_part.path,
								[
									{
										location: metadata_file_in_package,
										placement: target_directory.push(metadata_file),
									},
								],
							);
						}
					}
					let program_file = get_program_file_path(platform_type);
					if (!StorageHelper.exist_file(target_directory.push(program_file))) {
						let program_file_in_package = package_part.content.find((it) => it.emit() === program_file.emit());
						if (program_file_in_package !== undefined) {
							ExternalHelper.run_7z_extract_content(
								package_part.path,
								[
									{
										location: program_file_in_package,
										placement: target_directory.push(program_file),
									},
								],
							);
							package_part.platform.push(platform_type);
						}
					}
				}
			}
		}
		Console.information(`phase: detect platform type`, []);
		let platform_type_list = detect_platform_type(target_directory);
		assert_test(platform_type_list.length !== 0);
		Console.information(`tip: the platform type is '${platform_type_list.join('|')}'`, []);
		for (let platform_type of platform_type_list) {
			Console.information(`phase: modify program of '${platform_type}'`, []);
			modify_flat(
				platform_type,
				target_directory.push(get_program_file_path(platform_type)),
				target_directory.push(get_metadata_file_path(platform_type)),
				disable_record_encryption,
				enable_debug_mode,
			);
		}
		if (package_state !== null) {
			Console.information(`phase: repack package file`, []);
			for (let package_part of package_state.part) {
				if (package_part.platform.length === 0) {
					continue;
				}
				ExternalHelper.run_7z_append_content(
					package_part.path,
					package_part.platform
						.map((it) => ({
							location: get_program_file_path(it),
							placement: target_directory.push(get_program_file_path(it)),
						})),
				);
			}
		}
		if (package_type === 'apk' || package_type === 'apks') {
			Console.information(`phase: post-processing apk file`, []);
			assert_test(package_state !== null);
			let enable_align = true;
			let enable_sign = false;
			// TODO
			for (let package_part of package_state.part) {
				if (enable_align) {
					ExternalHelper.run_zipalign_align(package_part.path);
				}
				if (enable_sign) {
					ExternalHelper.run_apksigner_sign(package_part.path, new StoragePath(), '');
				}
			}
		}
		Console.information(`phase: generate result`, []);
		if (package_state === null) {
			for (let platform_type of platform_type_list) {
				let program_file = get_program_file_path(platform_type);
				StorageHelper.remove(target.push(program_file));
				StorageHelper.copy(target_directory.push(program_file), target.push(program_file), false);
			}
		}
		else if (package_state.bundle === null) {
			StorageHelper.remove(target);
			StorageHelper.copy(package_state.part[0].path, target, false);
		}
		else {
			ExternalHelper.run_7z_append_content(
				package_state.bundle,
				package_state.part
					.filter((it) => it.platform.length !== 0)
					.map((it) => ({
						location: it.location!,
						placement: it.path,
					})),
			);
			StorageHelper.remove(target);
			StorageHelper.copy(package_state.bundle, target, false);
		}
		StorageHelper.remove(temporary_directory);
		Console.information(`phase: done`, []);
		return;
	}

	// ----------------

	export function process_fs(
		target: StoragePath,
		disable_record_encryption: boolean,
		enable_debug_mode: boolean,
	): void {
		modify(target, disable_record_encryption, enable_debug_mode);
		return;
	}

	// #endregion

}
