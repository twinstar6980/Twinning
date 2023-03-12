namespace TwinStar.Script.Support.PvZ2.RemoteAndroidHelper {

	// ------------------------------------------------

	export const ActionE = [
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

	export type Action = typeof ActionE[number];

	// ------------------------------------------------

	function collect_application_information(
	): null | ADBHelper.ApplicationInformation {
		let id: string;
		let id_list = ADBHelper.find_application(/^com\.ea\.game\.pvz2_[a-z]+$/);
		if (id_list.length === 0) {
			Console.notify('w', los(`检测到 PvZ-2 应用不存在`), []);
			return null;
		} else if (id_list.length === 1) {
			Console.notify('i', los(`检测到 PvZ-2 应用存在`), [id_list[0]]);
			id = id_list[0];
		} else {
			Console.notify('i', los(`检测到 PvZ-2 应用存在多个，请选择目标包名`), []);
			id = Console.option(id_list.map((e) => ([e])), null);
		}
		let information = ADBHelper.get_application_information(id);
		return information;
	}

	// ------------------------------------------------

	export function execute(
		project_directory: string,
		action: Action,
	): void {
		let local_temporary_directory = HomeDirectory.new_temporary();
		let application = collect_application_information();
		if (application === null) {
			return;
		}
		Console.notify('i', los(`获取应用信息`), [
			los(`版本号：{}`, application.version_code),
			los(`版本名：{}`, application.version_name),
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
		Console.notify('i', los(`检测文件路径`), [
			los(`主数据包：{}`, ADBHelper.exist_file(remote.main_package)),
			los(`本地配置：{}`, ADBHelper.exist_file(remote.local_profile)),
			los(`玩家配置：{}`, ADBHelper.exist_file(remote.player_profile)),
			los(`内容分发：{}`, ADBHelper.exist_directory(remote.content_delivery)),
			los(`玩家配置快照1：{}`, ADBHelper.exist_file(remote.snapshot_1)),
			los(`玩家配置快照2：{}`, ADBHelper.exist_file(remote.snapshot_2)),
		]);
		switch (action) {
			case 'pull_main_package': {
				Console.notify('i', los(`拉取 ...`), [remote.main_package]);
				ADBHelper.pull(local_temporary.main_package, remote.main_package);
				Console.notify('i', los(`解包 ...`), [local_temporary.main_package]);
				CoreX.Tool.PopCap.RSB.unpack_fs(local_temporary.main_package, `${local.main_package}/manifest.json`, `${local.main_package}/description.json`, `${local.main_package}/group/{0}/{1}/resource`, `${local.main_package}/group/{0}/{1}/packet.rsgp`, { number: 4n, additional_texture_information_for_pvz_2_chinese_android: 0n });
				Console.notify('s', los(`完成`), [local.main_package]);
				break;
			}
			case 'push_main_package': {
				Console.notify('i', los(`打包 ...`), [local.main_package]);
				CoreX.Tool.PopCap.RSB.pack_fs(local_temporary.main_package, `${local.main_package}/manifest.json`, `${local.main_package}/description.json`, `${local.main_package}/group/{0}/{1}/resource`, `${local.main_package}/group/{0}/{1}/packet.rsgp`, `${local.main_package}/group/{0}/{1}/packet.rsgp`, { number: 4n, additional_texture_information_for_pvz_2_chinese_android: 0n }, 1024n << 20n);
				Console.notify('i', los(`推送 ...`), [local_temporary.main_package]);
				ADBHelper.push_secure(remote.main_package, local_temporary.main_package, application);
				Console.notify('s', los(`完成`), [remote.main_package]);
				break;
			}
			case 'pull_local_profile': {
				Console.notify('i', los(`拉取 ...`), [remote.local_profile]);
				ADBHelper.pull(local_temporary.local_profile, remote.local_profile);
				Console.notify('i', los(`解码 ...`), [local_temporary.local_profile]);
				CoreX.Tool.PopCap.RTON.decode_fs(local_temporary.local_profile, local.local_profile, false, { number: 1n });
				Console.notify('s', los(`完成`), [local.local_profile]);
				break;
			}
			case 'push_local_profile': {
				Console.notify('i', los(`编码 ...`), [local.local_profile]);
				CoreX.Tool.PopCap.RTON.encode_fs(local_temporary.local_profile, local.local_profile, true, true, { number: 1n }, 1n << 20n);
				Console.notify('i', los(`推送 ...`), [local_temporary.local_profile]);
				ADBHelper.push_secure(remote.local_profile, local_temporary.local_profile, application);
				Console.notify('s', los(`完成`), [remote.local_profile]);
				break;
			}
			case 'pull_player_profile': {
				Console.notify('i', los(`拉取 ...`), [remote.player_profile]);
				ADBHelper.pull(local_temporary.player_profile, remote.player_profile);
				Console.notify('i', los(`解码 ...`), [local_temporary.player_profile]);
				CoreX.Tool.PopCap.RTON.decode_fs(local_temporary.player_profile, local.player_profile, false, { number: 1n });
				Console.notify('s', los(`完成`), [local.player_profile]);
				break;
			}
			case 'push_player_profile': {
				Console.notify('i', los(`编码 ...`), [local.player_profile]);
				CoreX.Tool.PopCap.RTON.encode_fs(local_temporary.player_profile, local.player_profile, true, true, { number: 1n }, 1n << 20n);
				Console.notify('i', los(`推送 ...`), [local_temporary.player_profile]);
				ADBHelper.push_secure(remote.player_profile, local_temporary.player_profile, application);
				Console.notify('s', los(`完成`), [remote.player_profile]);
				break;
			}
			case 'pull_content_delivery': {
				Console.notify('i', los(`拉取 ...`), [remote.content_delivery]);
				ADBHelper.pull(local_temporary.content_delivery, remote.content_delivery);
				Console.notify('i', los(`解码 ...`), [local_temporary.content_delivery]);
				let sub_directory_list = CoreX.FileSystem.list_directory(local_temporary.content_delivery, 1n);
				let sub_file_list = CoreX.FileSystem.list_file(local_temporary.content_delivery, 1n);
				for (let sub_file of sub_file_list) {
					let json_name = sub_file.replace(/\.rton$/i, '.json');
					CoreX.Tool.PopCap.RTON.decode_fs(`${local_temporary.content_delivery}/${sub_file}`, `${local.content_delivery}/${json_name}`, false, { number: 1n });
				}
				for (let sub_directory of sub_directory_list) {
					CoreX.FileSystem.copy(`${local_temporary.content_delivery}/${sub_directory}`, `${local.content_delivery}/${sub_directory}`);
				}
				Console.notify('s', los(`完成`), [local.content_delivery]);
				break;
			}
			case 'push_content_delivery': {
				Console.notify('i', los(`编码 ...`), [local.content_delivery]);
				let buffer = Core.ByteArray.allocate(Core.Size.value(16n << 20n));
				let sub_directory_list = CoreX.FileSystem.list_directory(local.content_delivery, 1n);
				let sub_file_list = CoreX.FileSystem.list_file(local.content_delivery, 1n);
				for (let sub_file of sub_file_list) {
					let rton_name = sub_file.replace(/(?<!(cdn_config|forceupdateconfig))\.json$/i, '.rton');
					CoreX.Tool.PopCap.RTON.encode_fs(`${local_temporary.content_delivery}/${rton_name}`, `${local.content_delivery}/${sub_file}`, true, true, { number: 1n }, buffer.view());
				}
				for (let sub_directory of sub_directory_list) {
					CoreX.FileSystem.copy(`${local.content_delivery}/${sub_directory}`, `${local_temporary.content_delivery}/${sub_directory}`);
				}
				Console.notify('i', los(`推送 ...`), [local_temporary.content_delivery]);
				ADBHelper.push_secure(remote.content_delivery, local_temporary.content_delivery, application);
				Console.notify('s', los(`完成`), [remote.content_delivery]);
				break;
			}
			case 'clear_snapshot': {
				ADBHelper.remove(remote.snapshot_1);
				ADBHelper.remove(remote.snapshot_2);
				Console.notify('s', los(`完成`), [remote.snapshot_1, remote.snapshot_2]);
				break;
			}
		}
		return;
	}

	// ------------------------------------------------

}