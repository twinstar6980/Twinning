namespace Twinning.Script.Support.Wwise.Media.Encode {

	// ------------------------------------------------

	export let g_wwise_program_file: null | string = null;

	// ------------------------------------------------

	// base on Wwise 2019.2

	const k_sample_conversion_settings = `<?xml version="1.0" encoding="utf-8"?>
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

	// ------------------------------------------------

	function cast_wwise_internal_path(
		raw: string,
	): string {
		if (KernelX.is_windows) {
			return raw;
		}
		if (KernelX.is_macintosh) {
			// locate at wine drive Z
			assert_test(raw.startsWith('/'));
			return `Z:${raw}`;
		}
		assert_fail();
	};

	export function encode_fs(
		raw_file: string,
		ripe_file: string,
		format: Format,
	): void {
		assert_test(KernelX.is_windows || KernelX.is_macintosh, `unsupported system, this function only avaliable for windows or macintosh`);
		let wwise_program_name: string = undefined!;
		if (KernelX.is_windows) {
			wwise_program_name = 'WwiseConsole.exe';
		}
		if (KernelX.is_macintosh) {
			wwise_program_name = 'WwiseConsole.sh';
		}
		let wwise_program_file = g_wwise_program_file !== null ? g_wwise_program_file : ProcessHelper.search_path(wwise_program_name);
		assert_test(wwise_program_file !== null, `could not find '${wwise_program_name}' program from PATH environment`);
		let program_result: ProcessHelper.ExecuteResult;
		let temporary_directory = Home.new_temporary(null, null);
		let wwise_project_directory = `${temporary_directory}/Sample`;
		let wwise_wproj_file = `${wwise_project_directory}/Sample.wproj`;
		while (true) {
			program_result = ProcessHelper.execute(
				wwise_program_file,
				[
					'create-new-project',
					wwise_wproj_file,
					'--platform',
					'Android',
					'iOS',
				],
				KernelX.Process.list_environment_variable(),
			);
			assert_test(program_result.code === 0n, `execute failed by Wwise`);
			if (KernelX.Storage.exist_file(wwise_wproj_file)) {
				break;
			}
			Console.warning(`failed to create wwise project, retry ...`, [wwise_wproj_file]);
		}
		let wwise_wsources_file = `${wwise_project_directory}/Sample.wsources`;
		KernelX.XML.write_fs_js(wwise_wsources_file, XML.create_element_node('ExternalSourcesList', {
			SchemaVersion: '1',
			Root: cast_wwise_internal_path(wwise_project_directory),
		}, [
			XML.create_element_node('Source', {
				Path: 'Sample.wav',
				Destination: 'Sample.wem',
				Conversion: `_${format}`,
			}, []),
		]));
		KernelX.Storage.write_file_s(`${wwise_project_directory}/Conversion Settings/Sample Conversion Settings.wwu`, k_sample_conversion_settings);
		KernelX.Storage.copy(raw_file, `${wwise_project_directory}/Sample.wav`);
		let platform = ({
			'pcm': 'Android',
			'adpcm': 'Android',
			'vorbis': 'Android',
			'aac': 'iOS',
			'opus': 'Android',
			'wemopus': 'Android',
		})[format];
		program_result = ProcessHelper.execute(
			wwise_program_file,
			[
				'convert-external-source',
				wwise_wproj_file,
				'--platform',
				platform,
				'--source-file',
				wwise_wsources_file,
			],
			KernelX.Process.list_environment_variable(),
		);
		if (program_result.code !== 0n) {
			Console.error(`Wwise : ${program_result.code}`, [program_result.output]);
		}
		assert_test(program_result.code === 0n, `execute failed by Wwise`);
		KernelX.Storage.copy(`${wwise_project_directory}/GeneratedSoundBanks/${platform}/Sample.wem`, ripe_file);
		KernelX.Storage.remove(temporary_directory);
		return;
	}

	// ------------------------------------------------

}
