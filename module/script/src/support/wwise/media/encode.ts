namespace Twinning.Script.Support.Wwise.Media.Encode {

	// #region utility

	export function encode_fs(
		raw_file: StoragePath,
		ripe_file: StoragePath,
		format: Format,
	): void {
		let [temporary_directory, temporary_directory_finalizer] = StorageHelper.temporary();
		using temporary_directory_using = temporary_directory_finalizer;
		let wwise_project_directory = temporary_directory.join('Sample');
		let wwise_wproj_file = wwise_project_directory.join('Sample.wproj');
		while (true) {
			ExternalHelper.run_wwise_create_new_project(wwise_wproj_file, ExternalHelper.do_wwise_get_all_platform());
			if (StorageHelper.exist_file(wwise_wproj_file)) {
				break;
			}
			Console.warning(`failed to create wwise project, retry ...`, [wwise_wproj_file.emit()]);
		}
		StorageHelper.write_file_text(wwise_project_directory.join('Conversion Settings').join('Sample Conversion Settings.wwu'), ExternalHelper.do_wwise_make_sample_conversion_settings());
		let wwise_wsources_file = wwise_project_directory.join('Sample.wsources');
		StorageHelper.write_file_text(wwise_wsources_file, ExternalHelper.do_wwise_make_sample_source_list(wwise_project_directory, format));
		StorageHelper.copy(raw_file, wwise_project_directory.join('Sample.wav'), false);
		let platform = ExternalHelper.do_wwise_get_format_platform(format);
		ExternalHelper.run_wwise_convert_external_source(wwise_wproj_file, wwise_wsources_file, platform);
		StorageHelper.copy(wwise_project_directory.join('GeneratedSoundBanks').join(platform).join('Sample.wem'), ripe_file, false);
		return;
	}

	// #endregion

}
