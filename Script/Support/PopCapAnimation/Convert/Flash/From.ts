/** PAM转换至Flash */
namespace TwinKleS.Support.PopCapAnimation.Convert.Flash.From {

	// ------------------------------------------------

	function make_image_document(
		index: number,
		image: Core.Tool.PopCap.PAM.Manifest.JS_N.Image,
	): Core.XML.JS_Element {
		if (image.transform.length !== 6) {
			throw new MyError('');
		}
		return XML.create_element('DOMSymbolItem', {
			...k_xmlns_attribute,
			name: `image/image_${index + 1}`,
			symbolType: 'graphic',
		}, [
			XML.create_element_node('timeline', {}, [
				XML.create_element_node('DOMTimeline', {
					name: `image_${index + 1}`,
				}, [
					XML.create_element_node('layers', {}, [
						XML.create_element_node('DOMLayer', {}, [
							XML.create_element_node('frames', {}, [
								XML.create_element_node('DOMFrame', {
									index: `0`,
								}, [
									XML.create_element_node('elements', {}, [
										XML.create_element_node('DOMSymbolInstance', {
											libraryItemName: `source/source_${index + 1}`,
											symbolType: 'graphic',
											loop: 'loop',
										}, [
											XML.create_element_node('matrix', {}, [
												XML.create_element_node('Matrix', {
													a: `${image.transform[0].toFixed(6)}`,
													b: `${image.transform[1].toFixed(6)}`,
													c: `${image.transform[2].toFixed(6)}`,
													d: `${image.transform[3].toFixed(6)}`,
													tx: `${image.transform[4].toFixed(6)}`,
													ty: `${image.transform[5].toFixed(6)}`,
												}, []),
											]),
											XML.create_element_node('transformationPoint', {}, [
												XML.create_element_node('Point', {
													x: `${(-image.transform[4]).toFixed(6)}`,
													y: `${(-image.transform[5]).toFixed(6)}`,
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
		]);
	}

	function make_sprite_document(
		index: number | 'main',
		sprite: Core.Tool.PopCap.PAM.Manifest.JS_N.Sprite,
		sub_sprite: Array<Core.Tool.PopCap.PAM.Manifest.JS_N.Sprite>,
	): Core.XML.JS_Element {
		let model: Record<number, {
			state: null | boolean;
			resource: bigint;
			sprite: boolean;
			transform: Transform;
			color: Color;
			frame_start: bigint;
			frame_duration: bigint;
		}> = {};
		let frame_node_list: Record<number, Array<Core.XML.JS_Node>> = {};
		sprite.frame.forEach((frame, frame_index) => {
			for (let e of frame.remove) {
				model[Number(e.index)].state = false;
			}
			for (let e of frame.append) {
				model[Number(e.index)] = {
					state: null,
					resource: e.resource,
					sprite: e.sprite,
					transform: k_initial_transform,
					color: k_initial_color,
					frame_start: BigInt(frame_index),
					frame_duration: BigInt(frame_index),
				};
				frame_node_list[Number(e.index)] = [];
				if (frame_index > 0) {
					frame_node_list[Number(e.index)].push(
						XML.create_element_node('DOMFrame', {
							index: `0`,
							duration: `${frame_index}`,
						}, [
							XML.create_element_node('elements', {}, []),
						])
					);
				}
			}
			for (let e of frame.change) {
				let layer = model[Number(e.index)];
				layer.state = true;
				layer.transform = compute_standard_transform_from_variant(e.transform);
				if (e.color !== null) {
					layer.color = e.color;
				}
			}
			for (let layer_index in model) {
				let layer = model[layer_index];
				let frame_node = frame_node_list[layer_index];
				if (layer.state !== null) {
					if (frame_node.length > 0) {
						(frame_node[frame_node.length - 1].value as Core.XML.JS_Element).attribute.duration = `${layer.frame_duration}`;
					}
				}
				if (layer.state === true) {
					frame_node.push(
						XML.create_element_node('DOMFrame', {
							index: `${frame_index}`,
							duration: ``,
						}, [
							XML.create_element_node('elements', {}, [
								XML.create_element_node('DOMSymbolInstance', !layer.sprite ? {
									libraryItemName: `image/image_${layer.resource + 1n}`,
									symbolType: 'graphic',
									loop: 'loop',
								} : {
									libraryItemName: `sprite/sprite_${layer.resource + 1n}`,
									symbolType: 'graphic',
									loop: 'loop',
									firstFrame: `${(frame_index - Number(layer.frame_start)) % Number(sub_sprite[Number(layer.resource)].frame.length)}`,
								}, [
									XML.create_element_node('matrix', {}, [
										XML.create_element_node('Matrix', {
											a: `${layer.transform[0].toFixed(6)}`,
											b: `${layer.transform[1].toFixed(6)}`,
											c: `${layer.transform[2].toFixed(6)}`,
											d: `${layer.transform[3].toFixed(6)}`,
											tx: `${layer.transform[4].toFixed(6)}`,
											ty: `${layer.transform[5].toFixed(6)}`,
										}, []),
									]),
									XML.create_element_node('color', {}, [
										XML.create_element_node('Color', {
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
				++layer.frame_duration;
			}
		});
		for (let layer_index in model) {
			let layer = model[layer_index];
			let frame_node = frame_node_list[layer_index];
			(frame_node[frame_node.length - 1].value as Core.XML.JS_Element).attribute.duration = `${layer.frame_duration}`;
			delete model[layer_index];
		}
		return XML.create_element('DOMSymbolItem', {
			...k_xmlns_attribute,
			name: index === 'main' ? `main_sprite` : `sprite/sprite_${index + 1}`,
			symbolType: 'graphic',
		}, [
			XML.create_element_node('timeline', {}, [
				XML.create_element_node('DOMTimeline', {
					name: index === 'main' ? `main_sprite` : `sprite_${index + 1}`,
				}, [
					XML.create_element_node('layers', {}, Object.keys(frame_node_list).reverse().map((layer_index) => (
						XML.create_element_node('DOMLayer', {
							name: `${Number(layer_index) + 1}`,
						}, [
							XML.create_element_node('frames', {}, frame_node_list[Number(layer_index)]),
						])
					))),
				]),
			]),
		]);
	}

	function make_main_document(
		animation: Core.Tool.PopCap.PAM.Manifest.JS_N.Animation,
	): Core.XML.JS_Element {
		if (animation.main_sprite == null) {
			throw new MyError('');
		}
		let prev_end = {
			flow: -1,
			command: -1,
		};
		let flow_node: Array<Core.XML.JS_Node> = [];
		let command_node: Array<Core.XML.JS_Node> = [];
		animation.main_sprite.frame.forEach((frame, frame_index) => {
			if (frame.label !== null || frame.stop) {
				if (prev_end.flow + 1 < frame_index) {
					flow_node.push(XML.create_element_node('DOMFrame', {
						index: `${prev_end.flow + 1}`,
						duration: `${frame_index - (prev_end.flow + 1)}`,
					}, [
						XML.create_element_node('elements', {}, []),
					]));
				}
				let node = XML.create_element_node('DOMFrame', {
					index: `${frame_index}`,
				}, [
					XML.create_element_node('elements', {}, []),
				]);
				let node_element = node.value as Core.XML.JS_Element;
				if (frame.label !== null) {
					node_element.attribute['name'] = `${frame.label}`;
					node_element.attribute['labelType'] = `name`;
				}
				if (frame.stop) {
					node_element.child.unshift(XML.create_element_node('Actionscript', {}, [
						XML.create_element_node('script', {}, [
							XML.create_text_node(`stop();`, true),
						]),
					]));
				}
				flow_node.push(node);
				prev_end.flow = frame_index;
			}
			if (frame.command.length > 0) {
				if (prev_end.command + 1 < frame_index) {
					command_node.push(XML.create_element_node('DOMFrame', {
						index: `${prev_end.command + 1}`,
						duration: `${frame_index - (prev_end.command + 1)}`,
					}, []));
				}
				command_node.push(XML.create_element_node('DOMFrame', {
					index: `${frame_index}`,
				}, [
					XML.create_element_node('Actionscript', {}, [
						XML.create_element_node('script', {}, [
							XML.create_text_node(frame.command.map((e) => (`fscommand("${e.command}", "${e.argument}");`)).join('\n'), true),
						]),
					]),
				]));
				prev_end.command = frame_index;
			}
		});
		if (prev_end.flow + 1 < animation.main_sprite.frame.length) {
			flow_node.push(XML.create_element_node('DOMFrame', {
				index: `${prev_end.flow + 1}`,
				duration: `${animation.main_sprite.frame.length - (prev_end.flow + 1)}`,
			}, []));
		}
		if (prev_end.command + 1 < animation.main_sprite.frame.length) {
			command_node.push(XML.create_element_node('DOMFrame', {
				index: `${prev_end.command + 1}`,
				duration: `${animation.main_sprite.frame.length - (prev_end.command + 1)}`,
			}, []));
		}
		return XML.create_element('DOMDocument', {
			...k_xmlns_attribute,
			frameRate: `${animation.main_sprite.frame_rate}`,
			width: `${animation.size[0]}`,
			height: `${animation.size[1]}`,
			xflVersion: `${k_xfl_version}`,
		}, [
			XML.create_element_node('folders', {},
				['media', 'source', 'image', 'sprite'].map((e) => (XML.create_element_node('DOMFolderItem', {
					name: `${e}`,
					isExpanded: `true`,
				}, [])))
			),
			XML.create_element_node('media', {}, animation.image.map((e) => (
				XML.create_element_node('DOMBitmapItem', {
					name: `media/${e.name.split('|')[0]}`,
					href: `media/${e.name.split('|')[0]}.png`,
				}, [])
			))),
			XML.create_element_node('symbols', {}, [
				...animation.image.map((e, i) => (
					XML.create_element_node('Include', {
						href: `source/source_${i + 1}.xml`,
					}, [])
				)),
				...animation.image.map((e, i) => (
					XML.create_element_node('Include', {
						href: `image/image_${i + 1}.xml`,
					}, [])
				)),
				...animation.sprite.map((e, i) => (
					XML.create_element_node('Include', {
						href: `sprite/sprite_${i + 1}.xml`,
					}, [])
				)),
				XML.create_element_node('Include', {
					href: `main_sprite.xml`,
				}, []),
			]),
			XML.create_element_node('timelines', {}, [
				XML.create_element_node('DOMTimeline', {
					name: 'animation',
				}, [
					XML.create_element_node('layers', {}, [
						XML.create_element_node('DOMLayer', {
							name: 'flow',
						}, [
							XML.create_element_node('frames', {}, flow_node),
						]),
						XML.create_element_node('DOMLayer', {
							name: 'command',
						}, [
							XML.create_element_node('frames', {}, command_node),
						]),
						XML.create_element_node('DOMLayer', {
							name: 'sprite',
						}, [
							XML.create_element_node('frames', {}, [
								XML.create_element_node('DOMFrame', {
									index: `0`,
									duration: `${animation.main_sprite.frame.length}`,
								}, [
									XML.create_element_node('elements', {}, [
										XML.create_element_node('DOMSymbolInstance', {
											libraryItemName: `main_sprite`,
											symbolType: 'graphic',
											loop: 'loop',
										}, []),
									]),
								]),
							]),
						]),
					]),
				]),
			]),
		]);
	}

	export function from(
		animation: Core.Tool.PopCap.PAM.Manifest.JS_N.Animation,
	): FlashPackage {
		if (animation.main_sprite == null) {
			throw new MyError('');
		}
		return {
			extra: {
				position: animation.position,
				image: animation.image.map((e) => ({
					name: e.name,
					size: e.size!,
				})),
				sprite: animation.sprite.map((e) => ({
					name: e.name!,
				})),
				main_sprite: {
					name: animation.main_sprite.name!,
				},
			},
			document: make_main_document(animation),
			library: {
				image: animation.image.map((e, i) => (make_image_document(i, e))),
				sprite: animation.sprite.map((e, i) => (make_sprite_document(i, e, animation.sprite))),
				main_sprite: make_sprite_document('main', animation.main_sprite, animation.sprite),
			},
		};
	}

	// ------------------------------------------------

	export function from_fsh(
		raw: Core.Tool.PopCap.PAM.Manifest.JS_N.Animation,
		ripe_directory: string,
	): void {
		let ripe = from(raw);
		save_flash_package(ripe, ripe_directory);
		return;
	}

	export function from_fs(
		raw_file: string,
		ripe_directory: string,
	): void {
		let raw = CoreX.JSON.read_fs_js<Core.Tool.PopCap.PAM.Manifest.JS_N.Animation>(raw_file);
		let ripe = from(raw);
		save_flash_package(ripe, ripe_directory);
		return;
	}

	// ------------------------------------------------

}