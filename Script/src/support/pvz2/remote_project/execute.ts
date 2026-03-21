namespace Twinning.Script.Support.Pvz2.RemoteProject {

	// #region utility

	const ActionX = [
		'launch',
		'check',
		'clear',
		'pull',
		'push',
	] as const;

	export type Action = typeof ActionX[number];

	export const ActionE = ActionX as unknown as Action[];

	// ----------------

	const TargetX = [
		'main_package',
		'content_delivery',
		'local_profile',
		'player_profile',
	] as const;

	export type Target = typeof TargetX[number];

	export const TargetE = TargetX as unknown as Target[];

	// ----------------

	function collect_application_information(
	): null | AndroidHelper.ApplicationInformation {
		let identifier: string;
		let identifier_list = AndroidHelper.application_find(/^com\.ea\.game\.pvz2_[a-z]+$/);
		if (identifier_list.length === 0) {
			Console.warning(los('support.pvz2.remote_project.execute:application_found_none'), []);
			return null;
		}
		if (identifier_list.length === 1) {
			Console.information(los('support.pvz2.remote_project.execute:application_found_single'), [identifier_list[0]]);
			identifier = identifier_list[0];
		}
		else {
			Console.information(los('support.pvz2.remote_project.execute:application_found_multi'), []);
			identifier = Console.enumeration(Console.option_string(identifier_list), null, null);
		}
		return AndroidHelper.application_query(identifier);
	}

	// ----------------

	export function execute(
		project_directory: string,
		action: Action,
		target: null | Target,
		rton_version: typeof Kernel.Tool.Popcap.ReflectionObjectNotation.Version.Value,
		rton_encode_buffer_size: bigint,
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
		let remote_application_data_directory = `${remote_external_directory}/Android/data/${application.identifier}`;
		let remote_application_obb_directory = `${remote_external_directory}/Android/obb/${application.identifier}`;
		let remote = {
			main_package: `${remote_application_obb_directory}/main.${application.version_code}.${application.identifier}.obb`,
			content_delivery: `${remote_application_data_directory}/files/No_Backup/CDN.${application.version_name.slice(0, CheckHelper.not_or(application.version_name.lastIndexOf('.'), -1, undefined))}`,
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
		let local_temporary_directory = HomePath.new_temporary(null, null);
		let local_temporary = {
			content_delivery: `${local_temporary_directory}/content_delivery`,
			local_profile: `${local_temporary_directory}/local.rton`,
			player_profile: `${local_temporary_directory}/player.rton`,
		};
		switch (action) {
			case 'launch': {
				AndroidHelper.application_start(application.identifier, 'com.popcap.PvZ2.PvZ2GameActivity');
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
						for (let sub_file of KernelX.Storage.list_directory(local_temporary.content_delivery, null, true, false)) {
							if (sub_file.endsWith('.rton') || sub_file.endsWith('.json')) {
								KernelX.Tool.Popcap.ReflectionObjectNotation.decode_fs(`${local_temporary.content_delivery}/${sub_file}`, `${local.content_delivery}/${sub_file.replace('.rton', '.json')}`, rton_version);
							}
						}
						break;
					}
					case 'local_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:pull'), []);
						AndroidHelper.pull(local_temporary.local_profile, remote.local_profile);
						Console.information(los('support.pvz2.remote_project.execute:decode'), []);
						KernelX.Tool.Popcap.ReflectionObjectNotation.decode_fs(local_temporary.local_profile, local.local_profile, rton_version);
						break;
					}
					case 'player_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:pull'), []);
						AndroidHelper.pull(local_temporary.player_profile, remote.player_profile);
						Console.information(los('support.pvz2.remote_project.execute:decode'), []);
						KernelX.Tool.Popcap.ReflectionObjectNotation.decode_fs(local_temporary.player_profile, local.player_profile, rton_version);
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
						let rton_encode_buffer = Kernel.ByteArray.allocate(Kernel.Size.value(rton_encode_buffer_size));
						for (let sub_file of KernelX.Storage.list_directory(local.content_delivery, null, true, false)) {
							KernelX.Tool.Popcap.ReflectionObjectNotation.encode_fs(`${local_temporary.content_delivery}/${sub_file.replace(/(?<!(cdn_config|forceupdateconfig))\.json$/i, '.rton')}`, `${local.content_delivery}/${sub_file}`, true, true, rton_version, rton_encode_buffer.view());
						}
						Console.information(los('support.pvz2.remote_project.execute:push'), []);
						AndroidHelper.push(local_temporary.content_delivery, remote.content_delivery, application.user);
						break;
					}
					case 'local_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:encode'), []);
						let rton_encode_buffer = Kernel.ByteArray.allocate(Kernel.Size.value(rton_encode_buffer_size));
						KernelX.Tool.Popcap.ReflectionObjectNotation.encode_fs(local_temporary.local_profile, local.local_profile, true, true, rton_version, rton_encode_buffer.view());
						Console.information(los('support.pvz2.remote_project.execute:push'), []);
						AndroidHelper.push(local_temporary.local_profile, remote.local_profile, application.user);
						break;
					}
					case 'player_profile': {
						KernelX.Storage.create_directory(local_temporary_directory);
						Console.information(los('support.pvz2.remote_project.execute:encode'), []);
						let rton_encode_buffer = Kernel.ByteArray.allocate(Kernel.Size.value(rton_encode_buffer_size));
						KernelX.Tool.Popcap.ReflectionObjectNotation.encode_fs(local_temporary.player_profile, local.player_profile, true, true, rton_version, rton_encode_buffer.view());
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
		KernelX.Storage.remove_if(local_temporary_directory);
		return;
	}

	// #endregion

}
