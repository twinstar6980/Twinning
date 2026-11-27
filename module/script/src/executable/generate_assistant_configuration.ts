namespace Twinning.Script.Executable.GenerateAssistantConfiguration {

	// #region interface

	type AssistantCoreCommandSenderPresetConfiguration = {
		name: string;
		argument: Record<string, any>;
	};

	type AssistantCoreCommandSenderArgumentType = 'boolean' | 'integer' | 'floater' | 'string' | 'size' | 'path';

	type AssistantCoreCommandSenderArgumentConfiguration = {
		identifier: string;
		name: string;
		type: AssistantCoreCommandSenderArgumentType;
		option: Array<any>;
	};

	type AssistantCoreCommandSenderMethodConfiguration = {
		identifier: string;
		name: string;
		icon: string;
		argument: Array<AssistantCoreCommandSenderArgumentConfiguration>;
		batch: null | Array<string>;
		preset: Array<null | AssistantCoreCommandSenderPresetConfiguration>;
	};

	type AssistantCoreCommandSenderMethodGroupConfiguration = {
		identifier: string;
		name: string;
		icon: string;
		item: Array<AssistantCoreCommandSenderMethodConfiguration>;
	};

	type AssistantCoreCommandSenderConfiguration = {
		method: Array<AssistantCoreCommandSenderMethodGroupConfiguration>;
	};

	// ----------------

	type AssistantCoreResourceShipperPresetConfiguration = {
		name: string;
		argument: Record<string, any>;
	};

	type AssistantCoreResourceShipperArgumentType = 'any' | 'file' | 'directory';

	type AssistantCoreResourceShipperFilterConfiguration = {
		type: AssistantCoreResourceShipperArgumentType;
		name: string;
	};

	type AssistantCoreResourceShipperOptionConfiguration = {
		name: string;
		icon: string;
		filter: null | AssistantCoreResourceShipperFilterConfiguration;
		batch: boolean;
		method: null | string;
		preset: Array<null | AssistantCoreResourceShipperPresetConfiguration>;
	};

	type AssistantCoreResourceShipperOptionGroupConfiguration = {
		name: string;
		icon: string;
		item: Array<AssistantCoreResourceShipperOptionConfiguration>;
	};

	type AssistantCoreResourceShipperConfiguration = {
		option: Array<AssistantCoreResourceShipperOptionGroupConfiguration>;
	};

	// ----------------

	type ScriptMethodProto = {
		identifier: string;
		icon: string;
		preset: Array<null | AssistantCoreCommandSenderPresetConfiguration>;
	};

	type ScriptMethodGroupTemplate = {
		identifier: string;
		icon: string;
		item: Array<ScriptMethodProto>;
	};

	type ScriptConfigurationTemplate = {
		method: Array<ScriptMethodGroupTemplate>;
	};

	// #endregion

	// #region run

	export function run(
	): void {
		Console.information('generate script method template', [
		]);
		let input_file = Console.path('file', 'input', null, null);
		let s_cfg = JsonHelper.decode_file(input_file) as ScriptConfigurationTemplate;
		let a_cs: AssistantCoreCommandSenderConfiguration = {
			method: [],
		};
		let a_rs: AssistantCoreResourceShipperConfiguration = {
			option: [
				{
					"name": "Custom",
					"icon": "star",
					"item": [
						{
							"name": "Launch",
							"icon": "send",
							"filter": null,
							"batch": true,
							"method": null,
							"preset": [
							],
						},
						{
							"name": "Send",
							"icon": "send",
							"filter": {
								"type": "any",
								"name": "()$",
							},
							"batch": true,
							"method": null,
							"preset": [
							],
						},
					],
				},
			],
		};
		for (let s_group of s_cfg.method) {
			let group_proto = Executor.g_typical_method_proto[s_group.identifier];
			if (group_proto === undefined) {
				Console.warning(`typical method group '${s_group.identifier}' not found, skipped`, []);
				continue;
			}
			let a_cs_group: AssistantCoreCommandSenderMethodGroupConfiguration = {
				identifier: s_group.identifier,
				name: los(`executor.implementation:${s_group.identifier}`),
				icon: s_group.icon,
				item: [],
			};
			let a_rs_group: AssistantCoreResourceShipperOptionGroupConfiguration = {
				name: los(`executor.implementation:${s_group.identifier}`),
				icon: s_group.icon,
				item: [],
			};
			for (let s_item of s_group.item) {
				let item_proto = group_proto.find((value) => `${value.identifier[0]}.${value.identifier[1]}` === s_item.identifier);
				if (item_proto === undefined) {
					Console.warning(`typical method item '${s_item.identifier}' not found, skipped`, []);
					continue;
				}
				a_cs_group.item.push({
					identifier: s_item.identifier,
					name: los(`executor.implementation:${s_item.identifier}`),
					icon: s_item.icon,
					argument: item_proto.argument.map((argument: Executor.TypicalArgument<string, any, any>) => {
						return {
							identifier: argument.identifier,
							name: los(`executor.implementation:${s_item.identifier}:${argument.identifier}`),
							type: argument.type as AssistantCoreCommandSenderArgumentType,
							option: argument.option,
						};
					}),
					batch: item_proto.batch === null ? null : item_proto.batch.map((argument: Executor.TypicalBatchArgument<string>) => argument.identifier),
					preset: s_item.preset,
				});
				a_rs_group.item.push({
					name: los(`executor.implementation:${s_item.identifier}`),
					icon: s_item.icon,
					filter: item_proto.filter === null ? null : {
						type: item_proto.filter[0],
						name: item_proto.filter[1].source,
					},
					batch: item_proto.batch !== null,
					method: s_item.identifier,
					preset: s_item.preset,
				});
			}
			a_cs.method.push(a_cs_group);
			a_rs.option.push(a_rs_group);
		}
		Console.information(`configuration generated`, [
		]);
		let output_directory = Console.path('directory', 'output', null, null);
		JsonHelper.encode_file(output_directory.join('core_command_sender.json'), a_cs);
		JsonHelper.encode_file(output_directory.join('core_resource_shipper.json'), a_rs);
		Console.success('finish', [
		]);
		return;
	}

	// #endregion

}
