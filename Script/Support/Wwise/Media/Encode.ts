namespace TwinStar.Script.Support.Wwise.Media.Encode {

	// ------------------------------------------------

	// base on Wwise 2022.1
	// aac format has been removed since 2022.1

	const k_sample_conversion_settings = `<?xml version="1.0" encoding="utf-8"?>
<WwiseDocument Type="WorkUnit" ID="{B827F5D9-A167-4000-9F05-F112FD547144}" SchemaVersion="110">
	<Conversions>
		<WorkUnit Name="Sample Conversion Settings" ID="{B827F5D9-A167-4000-9F05-F112FD547144}" PersistMode="Standalone">
			<ChildrenList>
				<Conversion Name="_pcm" ID="{9B64F3EB-B87E-4D71-94C7-3E12A5E5E166}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Windows">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="SRConversionQuality" Type="int32">
							<ValueList>
								<Value>1</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Windows">
							<ConversionPlugin Name="" ID="{B1D66D26-8899-411D-A393-6845E5F24848}" PluginName="PCM" CompanyID="0" PluginID="1"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_adpcm" ID="{D1731024-37FB-4EBA-A025-1D6A2093E70C}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Windows">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Windows">
							<ConversionPlugin Name="" ID="{85319D48-65B7-40DD-8C94-5179698667BB}" PluginName="ADPCM" CompanyID="0" PluginID="2"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_vorbis" ID="{EEDCCA19-53FA-4260-B167-50EAA8AEB625}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Windows">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Windows">
							<ConversionPlugin Name="" ID="{45FCFC99-1AF3-47FB-A6FF-F664FB4F385E}" PluginName="Vorbis" CompanyID="0" PluginID="4"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
				<Conversion Name="_wemopus" ID="{1765A6E4-7807-4160-92D2-AB5495727A8C}">
					<PropertyList>
						<Property Name="Channels" Type="int32">
							<ValueList>
								<Value Platform="Windows">4</Value>
							</ValueList>
						</Property>
						<Property Name="LRMix" Type="Real64">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MaxSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="MinSampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
						<Property Name="SampleRate" Type="int32">
							<ValueList>
								<Value Platform="Windows">0</Value>
							</ValueList>
						</Property>
					</PropertyList>
					<ConversionPluginInfoList>
						<ConversionPluginInfo Platform="Windows">
							<ConversionPlugin Name="" ID="{4E8EDEDE-8E4B-424E-9873-F494B2450E5C}" PluginName="WEM Opus" CompanyID="0" PluginID="20"/>
						</ConversionPluginInfo>
					</ConversionPluginInfoList>
				</Conversion>
			</ChildrenList>
		</WorkUnit>
	</Conversions>
</WwiseDocument>`;

	// ------------------------------------------------

	const FormatX = [
		'pcm',
		'adpcm',
		'vorbis',
		'wemopus',
	] as const;

	export type Format = typeof FormatX[number];

	export const FormatE = FormatX as unknown as Format[];

	// ------------------------------------------------

	function cast_wwise_internal_path(
		raw: string,
	): string {
		if (Shell.is_windows) {
			return raw;
		}
		if (Shell.is_macintosh) {
			// location at wine drive Z
			assert_test(raw.startsWith('/'));
			return `Z:${raw}`;
		}
		assert_test(false);
	};

	export function encode_fs(
		raw_file: string,
		ripe_file: string,
		format: Format,
	): void {
		assert_test(Shell.is_windows || Shell.is_macintosh, `unsupported system, this function only avaliable for windows or macintosh`);
		let wwise_program_name: string = undefined!;
		if (Shell.is_windows) {
			wwise_program_name = 'WwiseConsole.exe';
		}
		if (Shell.is_macintosh) {
			wwise_program_name = 'WwiseConsole.sh';
		}
		let wwise_program_file = ProcessHelper.search_path(wwise_program_name);
		assert_test(wwise_program_file !== null, `can not found '${wwise_program_name}' program from PATH environment`);
		let program_result: ProcessHelper.ExecuteResult;
		let temporary_directory = Home.new_temporary(null, null);
		let wwise_project_directory = `${temporary_directory}/Sample`;
		let wwise_wproj_file = `${wwise_project_directory}/Sample.wproj`;
		let wwise_wsources_file = `${wwise_project_directory}/Sample.wsources`;
		program_result = ProcessHelper.execute(
			wwise_program_file,
			[
				'create-new-project',
				wwise_wproj_file,
			],
			KernelX.Process.list_environment_variable(),
		);
		assert_test(program_result.code === 0n, `execute failed by Wwise`);
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
		KernelX.FileSystem.write_file(`${wwise_project_directory}/Conversion Settings/Sample Conversion Settings.wwu`, Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(Kernel.String.value(k_sample_conversion_settings)));
		KernelX.FileSystem.copy(raw_file, `${wwise_project_directory}/Sample.wav`);
		program_result = ProcessHelper.execute(
			wwise_program_file,
			[
				'convert-external-source',
				wwise_wproj_file,
				'--source-file',
				wwise_wsources_file,
			],
			KernelX.Process.list_environment_variable(),
		);
		assert_test(program_result.code === 0n, `execute failed by Wwise`);
		KernelX.FileSystem.copy(`${wwise_project_directory}/GeneratedSoundBanks/Windows/Sample.wem`, ripe_file);
		KernelX.FileSystem.remove(temporary_directory);
		return;
	}

	// ------------------------------------------------

}
