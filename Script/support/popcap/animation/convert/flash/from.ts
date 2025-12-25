namespace Twinning.Script.Support.Popcap.Animation.Convert.Flash.From {

	// #region utility

	function create_image_document(
		image: Kernel.Tool.Popcap.Animation.Definition.JS_N.Image,
		index: number,
		version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
	): [Kernel.Xml.JS_Element, ExtraInformation['image'][number]] {
		let image_transform_matrix: Transform;
		if (version.number < 2n) {
			assert_test(image.transform.length === 3);
			image_transform_matrix = convert_transform_from_rotate_to_standard(image.transform);
		}
		else {
			assert_test(image.transform.length === 6);
			image_transform_matrix = image.transform;
		}
		return [Xml.create_element('DOMSymbolItem', {
			...k_xmlns_attribute,
			name: `image/image_${index + 1}`,
			symbolType: 'graphic',
		}, [
			Xml.create_element_node('timeline', {}, [
				Xml.create_element_node('DOMTimeline', {
					name: `image_${index + 1}`,
				}, [
					Xml.create_element_node('layers', {}, [
						Xml.create_element_node('DOMLayer', {}, [
							Xml.create_element_node('frames', {}, [
								Xml.create_element_node('DOMFrame', {
									index: `0`,
								}, [
									Xml.create_element_node('elements', {}, [
										Xml.create_element_node('DOMSymbolInstance', {
											libraryItemName: `source/source_${index + 1}`,
											symbolType: 'graphic',
											loop: 'loop',
										}, [
											Xml.create_element_node('matrix', {}, [
												Xml.create_element_node('Matrix', {
													a: `${image_transform_matrix[0].toFixed(6)}`,
													b: `${image_transform_matrix[1].toFixed(6)}`,
													c: `${image_transform_matrix[2].toFixed(6)}`,
													d: `${image_transform_matrix[3].toFixed(6)}`,
													tx: `${image_transform_matrix[4].toFixed(6)}`,
													ty: `${image_transform_matrix[5].toFixed(6)}`,
												}, []),
											]),
										]),
									]),
								]),
							]),
						]),
					]),
				]),
			]),
		]), {
			name: image.name,
			size: CheckHelper.not_undefined_or(image.size, [0n, 0n]),
		}];
	}

	function create_sprite_document(
		sprite: Kernel.Tool.Popcap.Animation.Definition.JS_N.Sprite,
		index: number | null,
		sub_sprite: Array<Kernel.Tool.Popcap.Animation.Definition.JS_N.Sprite>,
		version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
	): [Kernel.Xml.JS_Element, ExtraInformation['sprite'][number]] {
		let model: Record<number, {
			state: null | boolean;
			resource: bigint;
			sprite: boolean;
			transform: Transform;
			color: Color;
			frame_start: bigint;
			frame_duration: bigint;
		}> = {};
		let frame_node_list: Record<number, Array<Kernel.Xml.JS_Node>> = {};
		for (let frame_index = 0; frame_index < sprite.frame.length; frame_index++) {
			let frame = sprite.frame[frame_index];
			for (let remove of frame.remove) {
				model[Number(remove.index)].state = false;
			}
			for (let append of frame.append) {
				model[Number(append.index)] = {
					state: null,
					resource: append.resource,
					sprite: append.sprite,
					transform: k_initial_transform,
					color: k_initial_color,
					frame_start: BigInt(frame_index),
					frame_duration: BigInt(frame_index),
				};
				frame_node_list[Number(append.index)] = [];
				if (frame_index > 0) {
					frame_node_list[Number(append.index)].push(
						Xml.create_element_node('DOMFrame', {
							index: `0`,
							duration: `${frame_index}`,
						}, [
							Xml.create_element_node('elements', {}, []),
						])
					);
				}
			}
			for (let change of frame.change) {
				let layer = model[Number(change.index)];
				layer.state = true;
				layer.transform = convert_transform_from_variant_to_standard(change.transform);
				if (change.color !== null) {
					layer.color = change.color;
				}
			}
			for (let layer_index in model) {
				let layer = model[layer_index];
				let frame_node = frame_node_list[layer_index];
				if (layer.state !== null) {
					if (frame_node.length > 0) {
						(frame_node[frame_node.length - 1].value as Kernel.Xml.JS_Element).attribute.duration = `${layer.frame_duration}`;
					}
				}
				if (layer.state === true) {
					frame_node.push(
						Xml.create_element_node('DOMFrame', {
							index: `${frame_index}`,
							duration: ``,
						}, [
							Xml.create_element_node('elements', {}, [
								Xml.create_element_node('DOMSymbolInstance', !layer.sprite ? {
									libraryItemName: `image/image_${layer.resource + 1n}`,
									symbolType: 'graphic',
									loop: 'loop',
								} : {
									libraryItemName: `sprite/sprite_${layer.resource + 1n}`,
									symbolType: 'graphic',
									loop: 'loop',
									firstFrame: `${(frame_index - Number(layer.frame_start)) % Number(sub_sprite[Number(layer.resource)].frame.length)}`,
								}, [
									Xml.create_element_node('matrix', {}, [
										Xml.create_element_node('Matrix', {
											a: `${layer.transform[0].toFixed(6)}`,
											b: `${layer.transform[1].toFixed(6)}`,
											c: `${layer.transform[2].toFixed(6)}`,
											d: `${layer.transform[3].toFixed(6)}`,
											tx: `${layer.transform[4].toFixed(6)}`,
											ty: `${layer.transform[5].toFixed(6)}`,
										}, []),
									]),
									Xml.create_element_node('color', {}, [
										Xml.create_element_node('Color', {
											redMultiplier: `${layer.color[0].toFixed(6)}`,
											greenMultiplier: `${layer.color[1].toFixed(6)}`,
											blueMultiplier: `${layer.color[2].toFixed(6)}`,
											alphaMultiplier: `${layer.color[3].toFixed(6)}`,
										}, []),
									]),
								]),
							]),
						])
					);
					layer.state = null;
					layer.frame_duration = 0n;
				}
				if (layer.state === false) {
					delete model[layer_index];
				}
				layer.frame_duration++;
			}
		}
		for (let layer_index in model) {
			let layer = model[layer_index];
			let frame_node = frame_node_list[layer_index];
			(frame_node[frame_node.length - 1].value as Kernel.Xml.JS_Element).attribute.duration = `${layer.frame_duration}`;
			delete model[layer_index];
		}
		return [Xml.create_element('DOMSymbolItem', {
			...k_xmlns_attribute,
			name: index === null ? `main_sprite` : `sprite/sprite_${index + 1}`,
			symbolType: 'graphic',
		}, [
			Xml.create_element_node('timeline', {}, [
				Xml.create_element_node('DOMTimeline', {
					name: index === null ? `main_sprite` : `sprite_${index + 1}`,
				}, [
					Xml.create_element_node('layers', {}, [
						...ConvertHelper.record_to_array(frame_node_list, (layer_index, layer) => (
							Xml.create_element_node('DOMLayer', {
								name: `${layer_index + 1}`,
							}, [
								Xml.create_element_node('frames', {}, layer),
							])
						)).reverse(),
						Xml.create_element_node('DOMLayer', {
							name: `${0}`,
						}, [
							Xml.create_element_node('frames', {}, [
								Xml.create_element_node('DOMFrame', {
									index: `0`,
									duration: `${sprite.frame.length}`,
								}, [
									Xml.create_element_node('elements', {}, []),
								]),
							]),
						]),
					]),
				]),
			]),
		]), {
			name: CheckHelper.not_undefined_or(sprite.name, ''),
			frame_rate: CheckHelper.not_undefined_or(sprite.frame_rate, 0.0),
			work_area: version.number < 5n ? null : sprite.work_area![0] === 0n && sprite.work_area![1] === BigInt(sprite.frame.length - 1) ? null : sprite.work_area!,
		}];
	}

	function create_main_document(
		animation: Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation,
		version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
	): Kernel.Xml.JS_Element {
		let flow_node: Array<Kernel.Xml.JS_Node> = [];
		let command_node: Array<Kernel.Xml.JS_Node> = [];
		let instance_node: Array<Kernel.Xml.JS_Node> = [];
		if (animation.main_sprite !== null) {
			let previous_end = {
				flow: -1,
				command: -1,
			};
			for (let frame_index = 0; frame_index < animation.main_sprite.frame.length; frame_index++) {
				let frame = animation.main_sprite.frame[frame_index];
				if (frame.label !== null || frame.stop) {
					if (previous_end.flow + 1 < frame_index) {
						flow_node.push(Xml.create_element_node('DOMFrame', {
							index: `${previous_end.flow + 1}`,
							duration: `${frame_index - (previous_end.flow + 1)}`,
						}, [
							Xml.create_element_node('elements', {}, []),
						]));
					}
					let node = Xml.create_element_node('DOMFrame', {
						index: `${frame_index}`,
					}, [
						Xml.create_element_node('elements', {}, []),
					]);
					let node_element = node.value as Kernel.Xml.JS_Element;
					if (frame.label !== null) {
						node_element.attribute['name'] = `${frame.label}`;
						node_element.attribute['labelType'] = `name`;
					}
					if (frame.stop) {
						node_element.child.unshift(Xml.create_element_node('Actionscript', {}, [
							Xml.create_element_node('script', {}, [
								Xml.create_text_node(`stop();`, true),
							]),
						]));
					}
					flow_node.push(node);
					previous_end.flow = frame_index;
				}
				if (frame.command.length > 0) {
					if (previous_end.command + 1 < frame_index) {
						command_node.push(Xml.create_element_node('DOMFrame', {
							index: `${previous_end.command + 1}`,
							duration: `${frame_index - (previous_end.command + 1)}`,
						}, [
							Xml.create_element_node('elements', {}, []),
						]));
					}
					command_node.push(Xml.create_element_node('DOMFrame', {
						index: `${frame_index}`,
					}, [
						Xml.create_element_node('Actionscript', {}, [
							Xml.create_element_node('script', {}, [
								Xml.create_text_node(frame.command.map((value) => (`fscommand("${value[0]}", "${value[1]}");`)).join('\n'), true),
							]),
						]),
						Xml.create_element_node('elements', {}, []),
					]));
					previous_end.command = frame_index;
				}
			}
			if (previous_end.flow + 1 < animation.main_sprite.frame.length) {
				flow_node.push(Xml.create_element_node('DOMFrame', {
					index: `${previous_end.flow + 1}`,
					duration: `${animation.main_sprite.frame.length - (previous_end.flow + 1)}`,
				}, [
					Xml.create_element_node('elements', {}, []),
				]));
			}
			if (previous_end.command + 1 < animation.main_sprite.frame.length) {
				command_node.push(Xml.create_element_node('DOMFrame', {
					index: `${previous_end.command + 1}`,
					duration: `${animation.main_sprite.frame.length - (previous_end.command + 1)}`,
				}, [
					Xml.create_element_node('elements', {}, []),
				]));
			}
			instance_node.push(Xml.create_element_node('DOMFrame', {
				index: `0`,
				duration: `${animation.main_sprite.frame.length}`,
			}, [
				Xml.create_element_node('elements', {}, [
					Xml.create_element_node('DOMSymbolInstance', {
						libraryItemName: `main_sprite`,
						symbolType: 'graphic',
						loop: 'loop',
					}, []),
				]),
			]));
		}
		return Xml.create_element('DOMDocument', {
			...k_xmlns_attribute,
			frameRate: `${animation.frame_rate}`,
			width: `${animation.size[0]}`,
			height: `${animation.size[1]}`,
			xflVersion: `${k_xfl_version}`,
		}, [
			Xml.create_element_node('folders', {}, ['media', 'source', 'image', 'sprite'].map((value) => (
				Xml.create_element_node('DOMFolderItem', {
					name: value,
					isExpanded: `false`,
				}, [])
			))),
			Xml.create_element_node('media', {}, animation.image.map((value) => (
				Xml.create_element_node('DOMBitmapItem', {
					name: `media/${parse_image_file_name(value.name)}`,
					href: `media/${parse_image_file_name(value.name)}.png`,
				}, [])
			))),
			Xml.create_element_node('symbols', {}, [
				...animation.image.map((value, index) => (
					Xml.create_element_node('Include', {
						href: `source/source_${index + 1}.xml`,
					}, [])
				)),
				...animation.image.map((value, index) => (
					Xml.create_element_node('Include', {
						href: `image/image_${index + 1}.xml`,
					}, [])
				)),
				...animation.sprite.map((value, index) => (
					Xml.create_element_node('Include', {
						href: `sprite/sprite_${index + 1}.xml`,
					}, [])
				)),
				Xml.create_element_node('Include', {
					href: `main_sprite.xml`,
				}, []),
			]),
			Xml.create_element_node('timelines', {}, [
				Xml.create_element_node('DOMTimeline', {
					name: 'animation',
				}, [
					Xml.create_element_node('layers', {}, [
						Xml.create_element_node('DOMLayer', {
							name: 'flow',
						}, [
							Xml.create_element_node('frames', {}, flow_node),
						]),
						Xml.create_element_node('DOMLayer', {
							name: 'command',
						}, [
							Xml.create_element_node('frames', {}, command_node),
						]),
						Xml.create_element_node('DOMLayer', {
							name: 'instance',
						}, [
							Xml.create_element_node('frames', {}, instance_node),
						]),
					]),
				]),
			]),
		]);
	}

	// ----------------

	export function from(
		animation: Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation,
		version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
	): FlashPackage {
		let flash: FlashPackage = {
			extra: {
				position: undefined!,
				image: [],
				sprite: [],
				main_sprite: undefined!,
			},
			document: undefined!,
			library: {
				image: [],
				sprite: [],
				main_sprite: undefined!,
			},
		};
		flash.extra.position = animation.position;
		for (let index = 0; index < animation.image.length; ++index) {
			let value = animation.image[index];
			let [value_document, value_extra] = create_image_document(value, index, version);
			flash.library.image.push(value_document);
			flash.extra.image.push(value_extra);
		}
		for (let index = 0; index < animation.sprite.length; ++index) {
			let value = animation.sprite[index];
			let [value_document, value_extra] = create_sprite_document(value, index, animation.sprite, version);
			flash.library.sprite.push(value_document);
			flash.extra.sprite.push(value_extra);
		}
		if (animation.main_sprite === null) {
			assert_test(version.number >= 4n);
			flash.library.main_sprite = null;
			flash.extra.main_sprite = null;
		}
		else {
			let [value_document, value_extra] = create_sprite_document(animation.main_sprite, null, animation.sprite, version);
			flash.library.main_sprite = value_document;
			flash.extra.main_sprite = value_extra;
		}
		flash.document = create_main_document(animation, version);
		return flash;
	}

	// ----------------

	export function from_fsh(
		raw: Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation,
		ripe_directory: string,
		version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
	): void {
		let ripe = from(raw, version);
		save_flash_package(ripe_directory, ripe);
		return;
	}

	export function from_fs(
		raw_file: string,
		ripe_directory: string,
		version: typeof Kernel.Tool.Popcap.Animation.Version.Value,
	): void {
		let raw = KernelX.Json.read_fs_js(raw_file) as Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation;
		let ripe = from(raw, version);
		save_flash_package(ripe_directory, ripe);
		return;
	}

	// #endregion

}
