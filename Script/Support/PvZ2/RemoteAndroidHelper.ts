namespace TwinStar.Script.Support.PvZ2.RemoteAndroidHelper {

	// ------------------------------------------------

	const ActionX = [
		'pull_main_package',
		'push_main_package',
		'pull_local_profile',
		'push_local_profile',
		'pull_player_profile',
		'push_player_profile',
		'pull_content_delivery',
		'push_content_delivery',
		'clear_snapshot',
	] as const;

	export type Action = typeof ActionX[number];

	export const ActionE = ActionX as unknown as Action[];

	// ------------------------------------------------

	function collect_application_information(
	): null | ADBHelper.ApplicationInformation {
		let id: string;
		let id_list = ADBHelper.find_application(/^com\.ea\.game\.pvz2_[a-z]+$/);
		if (id_list.length === 0) {
			Console.warning(los('support.pvz2.remote_android_helper:application_found_none'), []);
			return null;
		} else if (id_list.length === 1) {
			Console.information(los('support.pvz2.remote_android_helper:application_found_single'), [id_list[0]]);
			id = id_list[0];
		} else {
			Console.information(los('support.pvz2.remote_android_helper:application_found_multi'), []);
			id = Console.option(Console.option_string(id_list), null, null);
		}
		let information = ADBHelper.get_application_information(id);
		return information;
	}

	// ------------------------------------------------

	export function execute(
		project_directory: string,
		action: Action,
	): void {
		let local_temporary_directory = Home.new_temporary();
		let application = collect_application_information();
		if (application === null) {
			return;
		}
		Console.information(los('support.pvz2.remote_android_helper:collect_application_information'), [
			los('support.pvz2.remote_android_helper:version_code', application.version_code),
			los('support.pvz2.remote_android_helper:version_name', application.version_name),
		]);
		let remote_external_directory = `/storage/emulated/0`;
		let remote_data_directory = `${remote_external_directory}/Android/data/${application.id}`;
		let remote_obb_directory = `${remote_external_directory}/Android/obb/${application.id}`;
		let remote = {
			main_package: `${remote_obb_directory}/main.${application.version_code}.${application.id}.obb`,
			local_profile: `${remote_data_directory}/files/No_Backup/local_profiles`,
			player_profile: `${remote_data_directory}/files/No_Backup/pp.dat`,
			content_delivery: `${remote_data_directory}/files/No_Backup/CDN.${application.version_name.substring(0, application.version_name.lastIndexOf('.'))}`,
			snapshot_1: `${remote_data_directory}/files/No_Backup/snapshot1.dat`,
			snapshot_2: `${remote_data_directory}/files/No_Backup/snapshot2.dat`,
			save: `${remote_data_directory}/files/No_Backup/save`,
			active_quests: `${remote_data_directory}/files/No_Backup/activequests`,
			daily_quests: `${remote_data_directory}/files/No_Backup/dailyquests`,
		};
		let local = {
			main_package: `${project_directory}/main.rsb.bundle`,
			local_profile: `${project_directory}/local.json`,
			player_profile: `${project_directory}/player.json`,
			content_delivery: `${project_directory}/content_delivery`,
		};
		let local_temporary = {
			main_package: `${local_temporary_directory}/main.rsb`,
			local_profile: `${local_temporary_directory}/local.rton`,
			player_profile: `${local_temporary_directory}/player.rton`,
			content_delivery: `${local_temporary_directory}/content_delivery`,
		};
		Console.information(los('support.pvz2.remote_android_helper:test_path'), [
			los('support.pvz2.remote_android_helper:main_package', ADBHelper.exist_file(remote.main_package)),
			los('support.pvz2.remote_android_helper:local_profile', ADBHelper.exist_file(remote.local_profile)),
			los('support.pvz2.remote_android_helper:player_profile', ADBHelper.exist_file(remote.player_profile)),
			los('support.pvz2.remote_android_helper:content_delivery', ADBHelper.exist_directory(remote.content_delivery)),
			los('support.pvz2.remote_android_helper:snapshot_1', ADBHelper.exist_file(remote.snapshot_1)),
			los('support.pvz2.remote_android_helper:snapshot_2', ADBHelper.exist_file(remote.snapshot_2)),
		]);
		switch (action) {
			case 'pull_main_package': {
				Console.information(los('support.pvz2.remote_android_helper:pull'), [remote.main_package]);
				ADBHelper.pull(local_temporary.main_package, remote.main_package);
				Console.information(los('support.pvz2.remote_android_helper:unpack'), [local_temporary.main_package]);
				CoreX.Tool.PopCap.ResourceStreamBundle.unpack_fs(local_temporary.main_package, `${local.main_package}/definition.json`, `${local.main_package}/description.json`, `${local.main_package}/group/{0}/{1}/resource`, `${local.main_package}/group/{0}/{1}/packet.rsg`, { number: 4n, extended_texture_information_for_pvz2_cn: 0n });
				Console.success(los('support.pvz2.remote_android_helper:finish'), [local.main_package]);
				break;
			}
			case 'push_main_package': {
				Console.information(los('support.pvz2.remote_android_helper:pack'), [local.main_package]);
				CoreX.Tool.PopCap.ResourceStreamBundle.pack_fs(local_temporary.main_package, `${local.main_package}/definition.json`, `${local.main_package}/description.json`, `${local.main_package}/group/{0}/{1}/resource`, `${local.main_package}/group/{0}/{1}/packet.rsg`, `${local.main_package}/group/{0}/{1}/packet.rsg`, { number: 4n, extended_texture_information_for_pvz2_cn: 0n }, 1024n << 20n);
				Console.information(los('support.pvz2.remote_android_helper:push'), [local_temporary.main_package]);
				ADBHelper.push_secure(remote.main_package, local_temporary.main_package, application);
				Console.success(los('support.pvz2.remote_android_helper:finish'), [remote.main_package]);
				break;
			}
			case 'pull_local_profile': {
				Console.information(los('support.pvz2.remote_android_helper:pull'), [remote.local_profile]);
				ADBHelper.pull(local_temporary.local_profile, remote.local_profile);
				Console.information(los('support.pvz2.remote_android_helper:decode'), [local_temporary.local_profile]);
				CoreX.Tool.PopCap.ReflectionObjectNotation.decode_fs(local_temporary.local_profile, local.local_profile, { number: 1n, native_string_encoding_use_utf8: true });
				Console.success(los('support.pvz2.remote_android_helper:finish'), [local.local_profile]);
				break;
			}
			case 'push_local_profile': {
				Console.information(los('support.pvz2.remote_android_helper:encode'), [local.local_profile]);
				CoreX.Tool.PopCap.ReflectionObjectNotation.encode_fs(local_temporary.local_profile, local.local_profile, true, true, { number: 1n, native_string_encoding_use_utf8: true }, 1n << 20n);
				Console.information(los('support.pvz2.remote_android_helper:push'), [local_temporary.local_profile]);
				ADBHelper.push_secure(remote.local_profile, local_temporary.local_profile, application);
				Console.success(los('support.pvz2.remote_android_helper:finish'), [remote.local_profile]);
				break;
			}
			case 'pull_player_profile': {
				Console.information(los('support.pvz2.remote_android_helper:pull'), [remote.player_profile]);
				ADBHelper.pull(local_temporary.player_profile, remote.player_profile);
				Console.information(los('support.pvz2.remote_android_helper:decode'), [local_temporary.player_profile]);
				CoreX.Tool.PopCap.ReflectionObjectNotation.decode_fs(local_temporary.player_profile, local.player_profile, { number: 1n, native_string_encoding_use_utf8: true });
				Console.success(los('support.pvz2.remote_android_helper:finish'), [local.player_profile]);
				break;
			}
			case 'push_player_profile': {
				Console.information(los('support.pvz2.remote_android_helper:encode'), [local.player_profile]);
				CoreX.Tool.PopCap.ReflectionObjectNotation.encode_fs(local_temporary.player_profile, local.player_profile, true, true, { number: 1n, native_string_encoding_use_utf8: true }, 1n << 20n);
				Console.information(los('support.pvz2.remote_android_helper:push'), [local_temporary.player_profile]);
				ADBHelper.push_secure(remote.player_profile, local_temporary.player_profile, application);
				Console.success(los('support.pvz2.remote_android_helper:finish'), [remote.player_profile]);
				break;
			}
			case 'pull_content_delivery': {
				Console.information(los('support.pvz2.remote_android_helper:pull'), [remote.content_delivery]);
				ADBHelper.pull(local_temporary.content_delivery, remote.content_delivery);
				Console.information(los('support.pvz2.remote_android_helper:decode'), [local_temporary.content_delivery]);
				let sub_directory_list = CoreX.FileSystem.list_directory(local_temporary.content_delivery, 1n);
				let sub_file_list = CoreX.FileSystem.list_file(local_temporary.content_delivery, 1n);
				for (let sub_file of sub_file_list) {
					let json_name = sub_file.replace(/\.rton$/i, '.json');
					CoreX.Tool.PopCap.ReflectionObjectNotation.decode_fs(`${local_temporary.content_delivery}/${sub_file}`, `${local.content_delivery}/${json_name}`, { number: 1n, native_string_encoding_use_utf8: true });
				}
				for (let sub_directory of sub_directory_list) {
					CoreX.FileSystem.copy(`${local_temporary.content_delivery}/${sub_directory}`, `${local.content_delivery}/${sub_directory}`);
				}
				Console.success(los('support.pvz2.remote_android_helper:finish'), [local.content_delivery]);
				break;
			}
			case 'push_content_delivery': {
				Console.information(los('support.pvz2.remote_android_helper:encode'), [local.content_delivery]);
				let buffer = Core.ByteArray.allocate(Core.Size.value(16n << 20n));
				let sub_directory_list = CoreX.FileSystem.list_directory(local.content_delivery, 1n);
				let sub_file_list = CoreX.FileSystem.list_file(local.content_delivery, 1n);
				for (let sub_file of sub_file_list) {
					let rton_name = sub_file.replace(/(?<!(cdn_config|forceupdateconfig))\.json$/i, '.rton');
					CoreX.Tool.PopCap.ReflectionObjectNotation.encode_fs(`${local_temporary.content_delivery}/${rton_name}`, `${local.content_delivery}/${sub_file}`, true, true, { number: 1n, native_string_encoding_use_utf8: true }, buffer.view());
				}
				for (let sub_directory of sub_directory_list) {
					CoreX.FileSystem.copy(`${local.content_delivery}/${sub_directory}`, `${local_temporary.content_delivery}/${sub_directory}`);
				}
				Console.information(los('support.pvz2.remote_android_helper:push'), [local_temporary.content_delivery]);
				ADBHelper.push_secure(remote.content_delivery, local_temporary.content_delivery, application);
				Console.success(los('support.pvz2.remote_android_helper:finish'), [remote.content_delivery]);
				break;
			}
			case 'clear_snapshot': {
				ADBHelper.remove(remote.snapshot_1);
				ADBHelper.remove(remote.snapshot_2);
				Console.success(los('support.pvz2.remote_android_helper:finish'), [remote.snapshot_1, remote.snapshot_2]);
				break;
			}
		}
		return;
	}

	// ------------------------------------------------

}