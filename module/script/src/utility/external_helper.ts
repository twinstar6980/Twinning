namespace Twinning.Script.ExternalHelper {

	// #region common

	export let g_enable_android_termux: boolean = false;

	export let g_program_path_map: Record<string, null | StoragePath> = {};

	// ----------------

	export type LaunchingOption = {
		host: null | ProcessHelper.ExecutionHost;
	};

	// ----------------

	export function launch_process(
		program: string,
		argument: Array<string>,
		workspace: null | StoragePath,
		environment: null | Record<string, string>,
		input: null | string,
		option: undefined | LaunchingOption,
	): ProcessHelper.ExecutionResult {
		if (option === undefined) {
			option = {
				host: null,
			};
		}
		let host: ProcessHelper.ExecutionHost = 'native';
		if (option.host !== null) {
			host = option.host;
		}
		else if (KernelX.is_android && g_enable_android_termux) {
			host = 'termux';
		}
		return ProcessHelper.execute_child(program, argument, workspace, environment, input, host, g_program_path_map);
	}

	// #endregion

	// #region sh

	export function run_sh_command(
		command: string,
		option?: LaunchingOption,
	): string {
		let process_result = launch_process(
			'sh',
			[
				`-c`,
				command,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return process_result.output;
	}

	// #endregion

	// #region openssl

	export function run_openssl_generate_key(
		key_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_key_file = temporary_directory.join(`key.key`);
		let process_result = launch_process(
			'openssl',
			[
				`genrsa`,
				`-out`, `${temporary_key_file.emit_native()}`,
				`2048`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_key_file, key_file, false);
		return;
	}

	export function run_openssl_generate_certificate(
		certificate_file: StoragePath,
		key_file: StoragePath,
		subject: string,
		time: [Date, Date],
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_certificate_file = temporary_directory.join(`certificate.crt`);
		let temporary_key_file = temporary_directory.join(`key.key`);
		StorageHelper.copy(key_file, temporary_key_file, false);
		let process_result = launch_process(
			'openssl',
			[
				`req`,
				`-new`,
				`-x509`,
				`-key`, `${temporary_key_file.emit_native()}`,
				`-out`, `${temporary_certificate_file.emit_native()}`,
				`-utf8`,
				`-not_before`, `${ConvertHelper.make_date_to_string(time[0], 'YYYYMMDDhhmmss')}Z`,
				`-not_after`, `${ConvertHelper.make_date_to_string(time[1], 'YYYYMMDDhhmmss')}Z`,
				`-subj`, `${subject}`,
				`-addext`, `basicConstraints=CA:FALSE`,
				`-addext`, `keyUsage=digitalSignature`,
				`-addext`, `extendedKeyUsage=codeSigning`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_certificate_file, certificate_file, false);
		return;
	}

	export function run_openssl_generate_keystore(
		keystore_file: StoragePath,
		key_file: StoragePath,
		certificate_file: StoragePath,
		password: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_keystore_file = temporary_directory.join(`keystore.p12`);
		let temporary_key_file = temporary_directory.join(`key.key`);
		let temporary_certificate_file = temporary_directory.join(`certificate.crt`);
		StorageHelper.copy(key_file, temporary_key_file, false);
		StorageHelper.copy(certificate_file, temporary_certificate_file, false);
		let process_result = launch_process(
			'openssl',
			[
				`pkcs12`,
				`-export`,
				`-inkey`, `${temporary_key_file.emit_native()}`,
				`-in`, `${temporary_certificate_file.emit_native()}`,
				`-out`, `${temporary_keystore_file.emit_native()}`,
				`-passout`, `pass:${password}`,
				`-keypbe`, `PBE-SHA1-3DES`,
				`-certpbe`, `PBE-SHA1-3DES`,
				`-macalg`, `SHA1`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_keystore_file, keystore_file, false);
		return;
	}

	export function run_openssl_export_certificate(
		keystore_file: StoragePath,
		certificate_file: StoragePath,
		password: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_keystore_file = temporary_directory.join(`keystore.p12`);
		let temporary_certificate_file = temporary_directory.join(`certificate.crt`);
		StorageHelper.copy(keystore_file, temporary_keystore_file, false);
		let process_result = launch_process(
			'openssl',
			[
				`pkcs12`,
				`-in`, `${temporary_keystore_file.emit_native()}`,
				`-passin`, `pass:${password}`,
				`-out`, `${temporary_certificate_file.emit_native()}`,
				`-nokeys`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_certificate_file, certificate_file, false);
		return;
	}

	export function run_openssl_query_certificate(
		certificate_file: StoragePath,
		option?: LaunchingOption,
	): {subject: string} {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_certificate_file = temporary_directory.join(`certificate.crt`);
		let temporary_result_file = temporary_directory.join(`result.txt`);
		StorageHelper.copy(certificate_file, temporary_certificate_file, false);
		let process_result = launch_process(
			'openssl',
			[
				`x509`,
				`-in`, `${temporary_certificate_file.emit_native()}`,
				`-out`, `${temporary_result_file.emit_native()}`,
				`-noout`,
				`-subject`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		let result_text = StorageHelper.read_file_text(temporary_result_file);
		let result: ReturnType<typeof run_openssl_query_certificate> = {} as any;
		{
			let match = /^subject=(.*)$/m.exec(result_text);
			assert_test(match !== null);
			result.subject = match[1];
		}
		return result;
	}

	// #endregion

	// #region 7z

	export function run_7z_list_content(
		zip_file: StoragePath,
		option?: LaunchingOption,
	): Array<StoragePath> {
		let process_result = launch_process(
			'7z',
			[
				`l`,
				`-sccUTF-8`,
				`-tzip`,
				`${zip_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		let output_line_list = ConvertHelper.split_string_by_line_feed(ConvertHelper.normalize_string_line_feed(process_result.output), true);
		let content_list = [] as Array<StoragePath>;
		let content_rule = /^(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}) (.{5}) ( +\d+) ( +\d+)  (.+)$/;
		for (let output_line of output_line_list) {
			let content_match = content_rule.exec(output_line);
			if (content_match === null) {
				continue;
			}
			content_list.push(new StoragePath(content_match[5]));
		}
		return content_list;
	}

	export function run_7z_extract_content(
		zip_file: StoragePath,
		content: Array<{location: StoragePath; placement: StoragePath}>,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_content_directory = temporary_directory.join(`content`);
		let process_result = launch_process(
			'7z',
			[
				`x`,
				`-sccUTF-8`,
				`-tzip`,
				`${zip_file.emit_native()}`,
				...content.map((it) => `${it.location.emit_posix()}`).filter((it) => it.length !== 0),
				`-o${temporary_content_directory.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		for (let content_item of content) {
			StorageHelper.copy(temporary_content_directory.push(content_item.location), content_item.placement, false);
		}
		return;
	}

	export function run_7z_modify_content(
		zip_file: StoragePath,
		content: Array<{location: StoragePath; placement: StoragePath}>,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_content_directory = temporary_directory.join(`content`);
		for (let content_item of content) {
			StorageHelper.copy(content_item.placement, temporary_content_directory.push(content_item.location), false);
		}
		let process_result = launch_process(
			'7z',
			[
				`a`,
				`-sccUTF-8`,
				`-tzip`,
				`-aoa`,
				`${zip_file.emit_native()}`,
				`${temporary_content_directory.join('*').emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	// #endregion

	// #region Wwise

	export function run_wwise_create_new_project(
		project_file: StoragePath,
		platform_list: Array<string>,
		option?: LaunchingOption,
	): void {
		let process_result = launch_process(
			'WwiseConsole',
			[
				`create-new-project`,
				`${project_file.emit_native()}`,
				`--platform`, ...platform_list,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	export function run_wwise_convert_external_source(
		project_file: StoragePath,
		sources_file: StoragePath,
		platform: string,
		option?: LaunchingOption,
	): void {
		let process_result = launch_process(
			'WwiseConsole',
			[
				`convert-external-source`,
				`${project_file.emit_native()}`,
				`--platform`, `${platform}`,
				`--source-file`, `${sources_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	// ----------------

	export function do_wwise_get_all_platform(
	): Array<string> {
		return ['Android', 'iOS'];
	}

	export function do_wwise_get_format_platform(
		format: 'pcm' | 'adpcm' | 'vorbis' | 'aac' | 'opus' | 'wemopus',
		option?: LaunchingOption,
	): string {
		return ({
			'pcm': 'Android',
			'adpcm': 'Android',
			'vorbis': 'Android',
			'aac': 'iOS',
			'opus': 'Android',
			'wemopus': 'Android',
		})[format];
	}

	export function do_wwise_make_internal_path(
		raw: StoragePath,
		option?: LaunchingOption,
	): string {
		if (KernelX.is_macintosh) {
			// locate at wine drive Z
			assert_test(raw.type() === StoragePathType.absolute);
			assert_test(raw.root() === null);
			return `Z:${raw.emit_native()}`;
		}
		return raw.emit_native();
	}

	export function do_wwise_make_sample_source_list(
		project_directory: StoragePath,
		format: string,
		option?: LaunchingOption,
	): string {
		return XmlHelper.encode_text(XmlHelper.create_element_node('ExternalSourcesList', {
			SchemaVersion: '1',
			Root: ExternalHelper.do_wwise_make_internal_path(project_directory),
		}, [
			XmlHelper.create_element_node('Source', {
				Path: 'Sample.wav',
				Destination: 'Sample.wem',
				Conversion: `_${format}`,
			}, []),
		]));
	}

	export function do_wwise_make_sample_conversion_settings(
	): string {
		return `<?xml version="1.0" encoding="utf-8"?>
<WwiseDocument Type="WorkUnit" ID="{E271866C-F671-47E6-BC51-1593F78D3B68}" SchemaVersion="97">
	<Conversions>
		<WorkUnit Name="Sample Conversion Settings" ID="{E271866C-F671-47E6-BC51-1593F78D3B68}" PersistMode="Standalone">
			<ChildrenList>
				<Conversion Name="_pcm" ID="{C1C231EE-7674-41C8-A204-832F63710222}">
					<PropertyList>
						<Property Name="SRConversionQuality" Type="int32">
							<ValueList>
								<Value>1</Value>
							</ValueList>
						</Property>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Android">4</Value>
								<Value Platform="iOS">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Android">
							<ConversionPlugin Name="" ID="{E73F4FCE-2AEE-4932-9B10-81CD50CA2B54}" PluginName="PCM" CompanyID="0" PluginID="1"/>
						</ConversionPluginInfo>
						<ConversionPluginInfo Platform="iOS">
							<ConversionPlugin Name="" ID="{E4CEA53E-E20A-474D-9269-A44C574513A0}" PluginName="PCM" CompanyID="0" PluginID="1"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_adpcm" ID="{F32D4E21-F791-42DF-8492-EBD7DDD7A2A7}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Android">4</Value>
								<Value Platform="iOS">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Android">
							<ConversionPlugin Name="" ID="{99CCAA5F-FC6B-48DE-93A4-0ADE935AF45B}" PluginName="ADPCM" CompanyID="0" PluginID="2"/>
						</ConversionPluginInfo>
						<ConversionPluginInfo Platform="iOS">
							<ConversionPlugin Name="" ID="{380EB198-D923-4C0C-8DFF-54F72D7A8CA2}" PluginName="ADPCM" CompanyID="0" PluginID="2"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_vorbis" ID="{189A2F8B-DC2F-44AB-B5B7-0A4C0B2BAD78}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Android">4</Value>
								<Value Platform="iOS">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Android">
							<ConversionPlugin Name="" ID="{27A4E896-5237-4397-9B4C-EC846BD756F9}" PluginName="Vorbis" CompanyID="0" PluginID="4"/>
						</ConversionPluginInfo>
						<ConversionPluginInfo Platform="iOS">
							<ConversionPlugin Name="" ID="{6884A0F7-A19C-41CD-A705-BD54D54FACB2}" PluginName="Vorbis" CompanyID="0" PluginID="4"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_aac" ID="{6817D8E5-C538-4AE8-845F-BC95ED28CE27}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Android">4</Value>
								<Value Platform="iOS">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Android">
							<ConversionPlugin Name="" ID="{19AD82DD-C013-490D-B802-97B1402B79E1}" PluginName="PCM" CompanyID="0" PluginID="1"/>
						</ConversionPluginInfo>
						<ConversionPluginInfo Platform="iOS">
							<ConversionPlugin Name="" ID="{5CA1BE1E-77BF-4834-B70C-47727954310A}" PluginName="AAC" CompanyID="0" PluginID="10"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_opus" ID="{2D38EA40-C5FB-4F85-8CEF-06434DEFB575}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Android">4</Value>
								<Value Platform="iOS">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Android">
							<ConversionPlugin Name="" ID="{4D35DF5C-F37D-4253-B55B-5F7DB2745E96}" PluginName="Opus" CompanyID="0" PluginID="19"/>
						</ConversionPluginInfo>
						<ConversionPluginInfo Platform="iOS">
							<ConversionPlugin Name="" ID="{B81804D1-D426-41F9-A40C-1C48125FA573}" PluginName="Opus" CompanyID="0" PluginID="19"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_wemopus" ID="{8D55F635-8222-4BEA-85A1-AD019F0EDF0F}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Android">4</Value>
								<Value Platform="iOS">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Android">0</Value>
								<Value Platform="iOS">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Android">
							<ConversionPlugin Name="" ID="{534034E0-D383-42C5-A23B-3BC769B20661}" PluginName="WEM Opus (Experimental)" CompanyID="0" PluginID="20"/>
						</ConversionPluginInfo>
						<ConversionPluginInfo Platform="iOS">
							<ConversionPlugin Name="" ID="{4287F781-5A09-4E1B-8BB0-8CF6E1B4F5AC}" PluginName="WEM Opus (Experimental)" CompanyID="0" PluginID="20"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
			</ChildrenList>
		</WorkUnit>
	</Conversions>
</WwiseDocument>`;
	}

	// #endregion

	// #region vgmstream-cli

	export function run_vgmstream_decode(
		raw_file: StoragePath,
		ripe_file: StoragePath,
		extension: string,
		option?: LaunchingOption,
	): string {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_raw_file = temporary_directory.join(`raw.wav`);
		let temporary_ripe_file = temporary_directory.join(`ripe.${extension}`);
		StorageHelper.copy(ripe_file, temporary_ripe_file, false);
		let process_result = launch_process(
			'vgmstream-cli',
			[
				`-o`, `${temporary_raw_file.emit_native()}`,
				`${temporary_ripe_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		let encoding_match = /^encoding: (.+)$/m.exec(process_result.output);
		assert_test(encoding_match !== null);
		let encoding = encoding_match[1];
		StorageHelper.copy(temporary_raw_file, raw_file, false);
		return encoding;
	}

	// #endregion

	// #region Il2CppDumper

	export function run_il2cppdumper_dump(
		program_file: StoragePath,
		metadata_file: StoragePath,
		option?: LaunchingOption,
	): Array<string> {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_program_file = temporary_directory.join(`program`);
		let temporary_metadata_file = temporary_directory.join(`metadata`);
		let temporary_dump_directory = temporary_directory.join(`dump`);
		StorageHelper.copy(program_file, temporary_program_file, false);
		StorageHelper.copy(metadata_file, temporary_metadata_file, false);
		StorageHelper.create_directory(temporary_dump_directory);
		let process_result = launch_process(
			'Il2CppDumper',
			[
				`${temporary_program_file.emit_native()}`,
				`${temporary_metadata_file.emit_native()}`,
				`${temporary_dump_directory.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (!/\nDone!\n(Press any key to exit\.\.\.\n)?$/.test(ConvertHelper.normalize_string_line_feed(process_result.output))) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		let dump_data = ConvertHelper.split_string_by_line_feed(StorageHelper.read_file_text(temporary_dump_directory.join(`dump.cs`)), true);
		return dump_data;
	}

	// ----------------

	export function do_il2cppdumper_search_field_from_dump_data(
		source: Array<string>,
		class_name: string,
		field_name: string,
		option?: LaunchingOption,
	): Array<{
		address: number;
		modifier: string;
		statically: boolean;
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
					modifier: field_match[1],
					statically: field_match[2] !== undefined,
					type: field_match[3],
				});
			}
			break;
		}
		return result;
	}

	export function do_il2cppdumper_search_method_from_dump_data(
		source: Array<string>,
		class_name: string,
		method_name: string,
		option?: LaunchingOption,
	): Array<{
		address: number;
		modifier: string;
		statically: boolean;
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
					modifier: method_match[1],
					statically: method_match[2] !== undefined,
					result: method_match[3],
					parameter: method_match[5],
				});
			}
			break;
		}
		return result;
	}

	// #endregion

	// #region windows

	export function run_signtool_sign(
		target_file: StoragePath,
		target_type: 'dll' | 'exe' | 'msix',
		keystore_file: StoragePath,
		keystore_password: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_executable_file = temporary_directory.join(`executable.${target_type}`);
		let temporary_keystore_file = temporary_directory.join(`keystore.p12`);
		StorageHelper.copy(target_file, temporary_executable_file, false);
		StorageHelper.copy(keystore_file, temporary_keystore_file, false);
		let process_result = launch_process(
			'signtool',
			[
				`sign`,
				`/fd`, `SHA256`,
				`/f`, `${temporary_keystore_file.emit_native()}`,
				`/p`, `${keystore_password}`,
				`${temporary_executable_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.remove(target_file);
		StorageHelper.copy(temporary_executable_file, target_file, false);
		return;
	}

	// ----------------

	export function run_mt_import(
		executable_file: StoragePath,
		manifest_file: StoragePath,
		resource_identifier: bigint,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_executable_file = temporary_directory.join(`executable`);
		let temporary_manifest_file = temporary_directory.join(`manifest`);
		StorageHelper.copy(executable_file, temporary_executable_file, false);
		StorageHelper.copy(manifest_file, temporary_manifest_file, false);
		let process_result = launch_process(
			'mt',
			[
				`-manifest`, `${temporary_manifest_file.emit_native()}`,
				`-outputresource:${temporary_executable_file.emit_native()};#${resource_identifier}`,
				`-verbose`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.remove(executable_file);
		StorageHelper.copy(temporary_executable_file, executable_file, false);
		return;
	}

	export function run_mt_export(
		executable_file: StoragePath,
		manifest_file: StoragePath,
		resource_identifier: bigint,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_executable_file = temporary_directory.join(`executable`);
		let temporary_manifest_file = temporary_directory.join(`manifest`);
		StorageHelper.copy(executable_file, temporary_executable_file, false);
		let process_result = launch_process(
			'mt',
			[
				`-inputresource:${temporary_executable_file.emit_native()};#${resource_identifier}`,
				`-out:${temporary_manifest_file.emit_native()}`,
				`-verbose`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_manifest_file, manifest_file, false);
		return;
	}

	// ----------------

	export function run_makepri_create_configure(
		configure_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_configure_file = temporary_directory.join(`priconfig.xml`);
		let process_result = launch_process(
			'makepri',
			[
				`createconfig`,
				`/o`,
				`/cf`, `${temporary_configure_file.emit_native()}`,
				`/dq`, `en-US`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_configure_file, configure_file, false);
		return;
	}

	export function run_makepri_create_resource(
		project_directory: StoragePath,
		configure_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_project_directory = temporary_directory.join(`project`);
		let temporary_configure_file = temporary_directory.join(`priconfig.xml`);
		let temporary_resource_directory = temporary_directory.join(`resource`);
		StorageHelper.copy(project_directory, temporary_project_directory, false);
		StorageHelper.copy(configure_file, temporary_configure_file, false);
		StorageHelper.create_directory(temporary_resource_directory);
		let process_result = launch_process(
			'makepri',
			[
				`new`,
				`/o`,
				`/cf`, `${temporary_configure_file.emit_native()}`,
				`/pr`, `${temporary_project_directory.emit_native()}`,
				`/of`, `${temporary_resource_directory.join(`resources.pri`).emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		let resource_file_list = StorageHelper.list_directory(temporary_resource_directory, null, false, false, true, false);
		for (let resource_file_item of resource_file_list) {
			StorageHelper.remove_if(project_directory.push(resource_file_item));
			StorageHelper.copy(temporary_resource_directory.push(resource_file_item), project_directory.push(resource_file_item), false);
		}
		return;
	}

	// ----------------

	export function run_makeappx_pack(
		package_file: StoragePath,
		content_directory: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_package_file = temporary_directory.join(`package.msix`);
		let temporary_content_directory = temporary_directory.join(`content`);
		StorageHelper.copy(content_directory, temporary_content_directory, false);
		let process_result = launch_process(
			'makeappx',
			[
				`pack`,
				`/o`,
				`/h`, `SHA256`,
				`/d`, `${temporary_content_directory.emit_native()}`,
				`/p`, `${temporary_package_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_package_file, package_file, false);
		return;
	}

	export function run_makeappx_unpack(
		package_file: StoragePath,
		content_directory: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_package_file = temporary_directory.join(`package.msix`);
		let temporary_content_directory = temporary_directory.join(`content`);
		StorageHelper.copy(package_file, temporary_package_file, false);
		let process_result = launch_process(
			'makeappx',
			[
				`unpack`,
				`/o`,
				`/p`, `${temporary_package_file.emit_native()}`,
				`/d`, `${temporary_content_directory.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_content_directory, content_directory, false);
		return;
	}

	// #endregion

	// #region linux

	export function run_appimagetool_pack(
		package_file: StoragePath,
		content_directory: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_package_file = temporary_directory.join(`package.AppImage`);
		let temporary_content_directory = temporary_directory.join(`content.AppDir`);
		StorageHelper.copy(content_directory, temporary_content_directory, false);
		let process_result = launch_process(
			'appimagetool',
			[
				`--no-appstream`,
				`${temporary_content_directory.emit_native()}`,
				`${temporary_package_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_package_file, package_file, false);
		return;
	}

	// #endregion

	// #region macintosh

	export function run_codesign_export_entitlement(
		target_file: StoragePath,
		entitlement_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let process_result = launch_process(
			'codesign',
			[
				`-d`,
				`--entitlements`, `:${entitlement_file.emit_native()}`,
				`${target_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	export function run_codesign_sign(
		target_file: StoragePath,
		entitlement_file: StoragePath,
		keystore_name: string,
		keychain_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let process_result = launch_process(
			'codesign',
			[
				`-s`, `${keystore_name}`,
				`--keychain`, `${keychain_file.emit_native()}`,
				`--entitlements`, `${entitlement_file.emit_native()}`,
				`--force`,
				`${target_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	// ----------------

	export function run_security_create_keychain(
		keychain_file: StoragePath,
		password: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_keychain_file = temporary_directory.join(`temporary.keychain`);
		let process_result = launch_process(
			'security',
			[
				`create-keychain`,
				`-p`, `${password}`,
				`${temporary_keychain_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_keychain_file, keychain_file, false);
		return;
	}

	export function run_security_delete_keychain(
		keychain_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let process_result = launch_process(
			'security',
			[
				`delete-keychain`,
				`${keychain_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	export function run_security_unlock_keychain(
		keychain_file: StoragePath,
		password: string,
		option?: LaunchingOption,
	): void {
		let process_result = launch_process(
			'security',
			[
				`unlock-keychain`,
				`-p`, `${password}`,
				`${keychain_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	export function run_security_set_key_partition_list(
		keychain_file: StoragePath,
		password: string,
		option?: LaunchingOption,
	): void {
		let process_result = launch_process(
			'security',
			[
				`set-key-partition-list`,
				`-S`, `apple-tool:,apple:`,
				`-s`,
				`-k`, `${password}`,
				`${keychain_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	export function run_security_import(
		keychain_file: StoragePath,
		keystore_file: StoragePath,
		password: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_keystore_file = temporary_directory.join(`keystore.p12`);
		StorageHelper.copy(keystore_file, temporary_keystore_file, false);
		let process_result = launch_process(
			'security',
			[
				`import`,
				`${temporary_keystore_file.emit_native()}`,
				`-k`, `${keychain_file.emit_native()}`,
				`-P`, `${password}`,
				`-T`, `/usr/bin/codesign`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return;
	}

	export function run_security_list_keychain(
		keychain_file_list: null | Array<StoragePath>,
		option?: LaunchingOption,
	): Array<StoragePath> {
		let process_result = launch_process(
			'security',
			[
				`list-keychains`,
				...(keychain_file_list === null ? [] : [
					`-s`,
					...keychain_file_list.map((it) => it.emit_native()),
				]),
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return ConvertHelper.split_string_by_line_feed(ConvertHelper.normalize_string_line_feed(process_result.output), true).map((it) => new StoragePath(it.trim().replaceAll(/^"|"$/g, '')));
	}

	export function run_security_find_identity(
		keychain_file: StoragePath,
		option?: LaunchingOption,
	): string {
		let process_result = launch_process(
			'security',
			[
				`find-identity`,
				`${keychain_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		let identity_match = / ([0-9A-F]{40}) /m.exec(process_result.output);
		assert_test(identity_match !== null);
		let identity = identity_match[1];
		return identity;
	}

	// ----------------

	export function run_createdmg_pack(
		package_file: StoragePath,
		content_directory: StoragePath,
		name: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_package_file = temporary_directory.join(`package.dmg`);
		let temporary_content_directory = temporary_directory.join(`${name}.app`);
		StorageHelper.copy(content_directory, temporary_content_directory, false);
		let process_result = launch_process(
			'create-dmg',
			[
				`--volname`, `${name}`,
				`--window-pos`, `0`, `0`,
				`--window-size`, `500`, `300`,
				`--text-size`, `16`,
				`--icon-size`, `128`,
				`--icon`, `${temporary_content_directory.name()!}`, `150`, `100`,
				`--app-drop-link`, `350`, `100`,
				`--format`, `UDZO`,
				`--filesystem`, `APFS`,
				`${temporary_package_file.emit_native()}`,
				`${temporary_content_directory.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.copy(temporary_package_file, package_file, false);
		return;
	}

	// #endregion

	// #region android

	export function run_zipalign_align(
		zip_file: StoragePath,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_zip_file = temporary_directory.join(`file.zip`);
		let temporary_aligned_zip_file = temporary_directory.join(`aligned.zip`);
		StorageHelper.copy(zip_file, temporary_zip_file, false);
		let process_result = launch_process(
			'zipalign',
			[
				`-P`, `16`,
				`-f`,
				`4`,
				`${temporary_zip_file.emit_native()}`,
				`${temporary_aligned_zip_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.remove(zip_file);
		StorageHelper.copy(temporary_aligned_zip_file, zip_file, false);
		return;
	}

	export function run_apksigner_sign(
		apk_file: StoragePath,
		signature_version: [boolean, boolean, boolean],
		keystore_file: StoragePath,
		keystore_password: string,
		option?: LaunchingOption,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let temporary_apk_file = temporary_directory.join(`file.apk`);
		let temporary_keystore_file = temporary_directory.join(`keystore.p12`);
		StorageHelper.copy(apk_file, temporary_apk_file, false);
		StorageHelper.copy(keystore_file, temporary_keystore_file, false);
		let process_result = launch_process(
			'apksigner',
			[
				`sign`,
				`--v1-signing-enabled`, `${signature_version[0]}`,
				`--v2-signing-enabled`, `${signature_version[1]}`,
				`--v3-signing-enabled`, `${signature_version[2]}`,
				`--v4-signing-enabled`, `false`,
				`--ks`, `${temporary_keystore_file.emit_native()}`,
				`--ks-pass`, `pass:${keystore_password}`,
				`${temporary_apk_file.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		StorageHelper.remove(apk_file);
		StorageHelper.copy(temporary_apk_file, apk_file, false);
		return;
	}

	// ----------------

	export function run_adb_shell(
		command: string,
		option?: LaunchingOption,
	): string {
		let process_result = launch_process(
			'adb',
			[
				`shell`,
				command,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return process_result.output;
	}

	export function run_adb_pull(
		remote: StoragePath,
		local: StoragePath,
		option?: LaunchingOption,
	): string {
		let process_result = launch_process(
			'adb',
			[
				`pull`,
				`${remote.emit_posix(true)}`,
				`${local.emit_native()}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return process_result.output;
	}

	export function run_adb_push(
		remote: StoragePath,
		local: StoragePath,
		option?: LaunchingOption,
	): string {
		let process_result = launch_process(
			'adb',
			[
				`push`,
				`${local.emit_native()}`,
				`${remote.emit_posix(true)}`,
			],
			null,
			null,
			null,
			option,
		);
		if (process_result.code !== 0n) {
			ProcessHelper.throw_execution_exception(process_result);
		}
		return process_result.output;
	}

	// #endregion

	// #region iphone

	// #endregion

}
