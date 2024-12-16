namespace Twinning.Script.Support.PvZ2.RemoteProject {

	// ------------------------------------------------

	const ActionX = [
		'launch',
		'check',
		'clear',
		'pull',
		'push',
	] as const;

	export type Action = typeof ActionX[number];

	export const ActionE = ActionX as unknown as Action[];

	// ------------------------------------------------

	const TargetX = [
		'main_package',
		'content_delivery',
		'local_profile',
		'player_profile',
	] as const;

	export type Target = typeof TargetX[number];

	export const TargetE = TargetX as unknown as Target[];

	// ------------------------------------------------

	function collect_application_information(
	): null | AndroidHelper.ApplicationInformation {
		let id: string;
		let id_list = AndroidHelper.application_find(/^com\.ea\.game\.pvz2_[a-z]+$/);
		if (id_list.length === 0) {
			Console.warning(los('support.pvz2.remote_project.execute:application_found_none'), []);
			return null;
		}
		if (id_list.length === 1) {
			Console.information(los('support.pvz2.remote_project.execute:application_found_single'), [id_list[0]]);
			id = id_list[0];
		}
		else {
			Console.information(los('support.pvz2.remote_project.execute:application_found_multi'), []);
			id = Console.enumeration(Console.option_string(id_list), null, null);
		}
		return AndroidHelper.application_query(id);
	}

	// ------------------------------------------------

	const g_rton_version = { number: 1n, native_string_encoding_use_utf8: true } as typeof Kernel.Tool.PopCap.ReflectionObjectNotation.Version.Value;

	const g_rton_encode_buffer = 4n << 20n;

	export function execute(
		project_directory: string,
		action: Action,
		target: null | Target,
	): void {
		let application = collect_application_information();
		if (application === null) {
			return;
		}
		Console.information(los('support.pvz2.remote_project.execute:collect_application_information'), [
			los('support.pvz2.remote_project.execute:version_code', application.version_code),
			los('support.pvz2.remote_project.execute:version_name', application.version_name),
		]);
		let remote_external_directory = `/storage/emulated/0`;
		let remote_application_data_directory = `${remote_external_directory}/Android/data/${application.id}`;
		let remote_application_obb_directory = `${remote_external_directory}/Android/obb/${application.id}`;
		let remote = {
			main_package: `${remote_application_obb_directory}/main.${application.version_code}.${application.id}.obb`,
			content_delivery: `${remote_application_data_directory}/files/No_Backup/CDN.${application.version_name.slice(0, not_or(application.version_name.lastIndexOf('.'), -1, undefined))}`,
			local_profile: `${remote_application_data_directory}/files/No_Backup/local_profiles`,
			player_profile: `${remote_application_data_directory}/files/No_Backup/pp.dat`,
			player_profile_snapshot_1: `${remote_application_data_directory}/files/No_Backup/snapshot1.dat`,
			player_profile_snapshot_2: `${remote_application_data_directory}/files/No_Backup/snapshot2.dat`,
		};
		let local = {
			main_package: `${project_directory}/main.rsb`,
			content_delivery: `${project_directory}/content_delivery`,
			local_profile: `${project_directory}/local.json`,
			player_profile: `${project_directory}/player.json`,
		};
		let local_temporary_directory = Home.new_temporary(null, null);
		let local_temporary = {
			content_delivery: `${local_temporary_directory}/content_delivery`,
			local_profile: `${local_temporary_directory}/local.rton`,
			player_profile: `${local_temporary_directory}/player.rton`,
		};
		switch (action) {
			case 'launch': {
				AndroidHelper.application_start(application.id, 'com.popcap.PvZ2.PvZ2GameActivity');
				break;
			}
			case 'check': {
				Console.information(los('support.pvz2.remote_project.execute:check_path'), [
					los('support.pvz2.remote_project.execute:main_package', AndroidHelper.fs_exist_file(remote.main_package)),
					los('support.pvz2.remote_project.execute:content_delivery', AndroidHelper.fs_exist_directory(remote.content_delivery)),
					los('support.pvz2.remote_project.execute:local_profile', AndroidHelper.fs_exist_file(remote.local_profile)),
					los('support.pvz2.remote_project.execute:player_profile', AndroidHelper.fs_exist_file(remote.player_profile)),
					los('support.pvz2.remote_project.execute:player_profile_snapshot_1', AndroidHelper.fs_exist_file(remote.player_profile_snapshot_1)),
					los('support.pvz2.remote_project.execute:player_profile_snapshot_2', AndroidHelper.fs_exist_file(remote.player_profile_snapshot_2)),
				]);
				break;
			}
			case 'clear': {
				assert_test(target !== null);
				switch (target) {
					case 'main_package': {
						AndroidHelper.fs_remove(remote.main_package);
						break;
					}
					case 'content_delivery': {
						AndroidHelper.fs_remove(remote.content_delivery);
						break;
					}
					case 'local_profile': {
						AndroidHelper.fs_remove(remote.local_profile);
						break;
					}
					case 'player_profile': {
						AndroidHelper.fs_remove(remote.player_profile);
						AndroidHelper.fs_remove(remote.player_profile_snapshot_1);
						AndroidHelper.fs_remove(remote.player_profile_snapshot_2);
						break;
					}
				}
				break;
			}
			case 'pull': {
				assert_test(target !== null);
				switch (target) {
					case 'main_package': {
						Console.information(los('support.pvz2.remote_project.execute:pull'), []);
						AndroidHelper.pull(local.main_package, remote.main_package);
						break;
					}
					case 'content_delivery': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:pull'), []);
						AndroidHelper.pull(local_temporary.content_delivery, remote.content_delivery);
						Console.information(los('support.pvz2.remote_project.execute:decode'), []);
						for (let sub_file of KernelX.Storage.list_file(local_temporary.content_delivery)) {
							if (sub_file.endsWith('.rton') || sub_file.endsWith('.json')) {
								KernelX.Tool.PopCap.ReflectionObjectNotation.decode_fs(`${local_temporary.content_delivery}/${sub_file}`, `${local.content_delivery}/${sub_file.replace('.rton', '.json')}`, g_rton_version);
							}
						}
						break;
					}
					case 'local_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:pull'), []);
						AndroidHelper.pull(local_temporary.local_profile, remote.local_profile);
						Console.information(los('support.pvz2.remote_project.execute:decode'), []);
						KernelX.Tool.PopCap.ReflectionObjectNotation.decode_fs(local_temporary.local_profile, local.local_profile, g_rton_version);
						break;
					}
					case 'player_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:pull'), []);
						AndroidHelper.pull(local_temporary.player_profile, remote.player_profile);
						Console.information(los('support.pvz2.remote_project.execute:decode'), []);
						KernelX.Tool.PopCap.ReflectionObjectNotation.decode_fs(local_temporary.player_profile, local.player_profile, g_rton_version);
						break;
					}
				}
				break;
			}
			case 'push': {
				assert_test(target !== null);
				switch (target) {
					case 'main_package': {
						Console.information(los('support.pvz2.remote_project.execute:push'), []);
						AndroidHelper.push(local.main_package, remote.main_package, application.user);
						break;
					}
					case 'content_delivery': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:encode'), []);
						let buffer = Kernel.ByteArray.allocate(Kernel.Size.value(g_rton_encode_buffer));
						for (let sub_file of KernelX.Storage.list_file(local.content_delivery)) {
							KernelX.Tool.PopCap.ReflectionObjectNotation.encode_fs(`${local_temporary.content_delivery}/${sub_file.replace(/(?<!(cdn_config|forceupdateconfig))\.json$/i, '.rton')}`, `${local.content_delivery}/${sub_file}`, true, true, g_rton_version, buffer.view());
						}
						Console.information(los('support.pvz2.remote_project.execute:push'), []);
						AndroidHelper.push(local_temporary.content_delivery, remote.content_delivery, application.user);
						break;
					}
					case 'local_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:encode'), []);
						KernelX.Tool.PopCap.ReflectionObjectNotation.encode_fs(local_temporary.local_profile, local.local_profile, true, true, g_rton_version, g_rton_encode_buffer);
						Console.information(los('support.pvz2.remote_project.execute:push'), []);
						AndroidHelper.push(local_temporary.local_profile, remote.local_profile, application.user);
						break;
					}
					case 'player_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:encode'), []);
						KernelX.Tool.PopCap.ReflectionObjectNotation.encode_fs(local_temporary.player_profile, local.player_profile, true, true, g_rton_version, g_rton_encode_buffer);
						Console.information(los('support.pvz2.remote_project.execute:push'), []);
						AndroidHelper.push(local_temporary.player_profile, remote.player_profile, application.user);
						AndroidHelper.fs_remove(remote.player_profile_snapshot_1);
						AndroidHelper.fs_remove(remote.player_profile_snapshot_2);
						break;
					}
				}
				break;
			}
		}
		Console.success(los('support.pvz2.remote_project.execute:finish'), []);
		if (KernelX.Storage.exist_directory(local_temporary_directory)) {
			KernelX.Storage.remove(local_temporary_directory);
		}
		return;
	}

	// ------------------------------------------------

}