namespace Twinning.Script.Support.PopCap.Animation.Convert.Flash.To {

	// ------------------------------------------------

	function parse_transform(
		x_Matrix: Kernel.XML.JS_Element,
	): Transform {
		return [
			Number(not_undefined_or(x_Matrix.attribute.a, '1')),
			Number(not_undefined_or(x_Matrix.attribute.b, '0')),
			Number(not_undefined_or(x_Matrix.attribute.c, '0')),
			Number(not_undefined_or(x_Matrix.attribute.d, '1')),
			Number(not_undefined_or(x_Matrix.attribute.tx, '0')),
			Number(not_undefined_or(x_Matrix.attribute.ty, '0')),
		];
	}

	function parse_color(
		x_Matrix: Kernel.XML.JS_Element,
	): Color {
		let compute = (multiplier_s: undefined | string, offset_s: undefined | string) => (Math.max(0, Math.min(255, Number(not_undefined_or(multiplier_s, '1')) * 255 + Number(not_undefined_or(offset_s, '0')))) / 255);
		return [
			compute(x_Matrix.attribute.redMultiplier, x_Matrix.attribute.redOffset),
			compute(x_Matrix.attribute.greenMultiplier, x_Matrix.attribute.greenOffset),
			compute(x_Matrix.attribute.blueMultiplier, x_Matrix.attribute.blueOffset),
			compute(x_Matrix.attribute.alphaMultiplier, x_Matrix.attribute.alphaOffset),
		];
	}

	function parse_image_document(
		document: Kernel.XML.JS_Element,
		extra: ExtraInformation['image'][number],
		index: number,
		version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
	): Kernel.Tool.PopCap.Animation.Definition.JS_N.Image {
		let x_DOMSymbolItem = document;
		assert_test(x_DOMSymbolItem.name === 'DOMSymbolItem');
		assert_test(x_DOMSymbolItem.attribute.name === `image/image_${index + 1}`);
		let x_timeline = XML.find_child_element_unique(x_DOMSymbolItem, 'timeline');
		let x_DOMTimeline = XML.find_child_element_unique(x_timeline, 'DOMTimeline');
		assert_test(x_DOMTimeline.attribute.name === `image_${index + 1}`);
		let x_layers = XML.find_child_element_unique(x_DOMTimeline, 'layers');
		let x_DOMLayer = XML.find_child_element_unique(x_layers, 'DOMLayer');
		let x_frames = XML.find_child_element_unique(x_DOMLayer, 'frames');
		let x_DOMFrame = XML.find_child_element_unique(x_frames, 'DOMFrame');
		let x_elements = XML.find_child_element_unique(x_DOMFrame, 'elements');
		let x_DOMSymbolInstance = XML.find_child_element_unique(x_elements, 'DOMSymbolInstance');
		assert_test(x_DOMSymbolInstance.attribute.libraryItemName === `source/source_${index + 1}`);
		let transform: Transform;
		let x_matrix = XML.find_child_element_unique_or_none(x_DOMSymbolInstance, 'matrix');
		if (x_matrix === null) {
			transform = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0];
		}
		else {
			let x_Matrix = XML.find_child_element_unique(x_matrix, 'Matrix');
			transform = parse_transform(x_Matrix);
		}
		return {
			name: extra.name,
			...(version.number < 4n ? {} : { size: extra.size }),
			...(version.number < 2n ? { transform: convert_transform_from_standard_to_rotate(transform) } : { transform: transform }),
		};
	}

	function parse_sprite_document(
		document: Kernel.XML.JS_Element,
		extra: ExtraInformation['sprite'][number],
		index: number | null,
		version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
	): Kernel.Tool.PopCap.Animation.Definition.JS_N.Sprite {
		let model: null | {
			index: bigint;
			resource: bigint;
			sprite: boolean;
			frame_start: bigint;
			frame_duration: bigint;
			color: Color;
		} = null;
		let x_DOMSymbolItem = document;
		assert_test(x_DOMSymbolItem.name === 'DOMSymbolItem');
		assert_test(x_DOMSymbolItem.attribute.name === (index === null ? `main_sprite` : `sprite/sprite_${index + 1}`));
		let x_timeline = XML.find_child_element_unique(x_DOMSymbolItem, 'timeline');
		let x_DOMTimeline = XML.find_child_element_unique(x_timeline, 'DOMTimeline');
		assert_test(x_DOMTimeline.attribute.name === (index === null ? `main_sprite` : `sprite_${index + 1}`));
		let x_layers = XML.find_child_element_unique(x_DOMTimeline, 'layers');
		let x_DOMLayer_list = XML.find_child_element(x_layers, 'DOMLayer');
		assert_test(x_DOMLayer_list.length >= 1);
		let frame_list: Array<Kernel.Tool.PopCap.Animation.Definition.JS_N.Frame>;
		{
			let x_DOMLayer = x_DOMLayer_list[x_DOMLayer_list.length - 1];
			let x_frames = XML.find_child_element_unique(x_DOMLayer, 'frames');
			let x_DOMFrame = XML.find_child_element_unique(x_frames, 'DOMFrame');
			let frame_index = BigInt(x_DOMFrame.attribute.index);
			let frame_duration = BigInt(not_undefined_or(x_DOMFrame.attribute.duration, '1'));
			assert_test(frame_index === 0n && frame_duration > 0n);
			let x_elements = XML.find_child_element_unique(x_DOMFrame, 'elements');
			assert_test(x_elements.child.length === 0);
			frame_list = new Array(Number(frame_duration) + 1);
			for (let index = 0; index < frame_list.length; index++) {
				frame_list[index] = {
					label: null,
					stop: false,
					command: [],
					remove: [],
					append: [],
					change: [],
				};
			}
		}
		x_DOMLayer_list.pop();
		x_DOMLayer_list.reverse();
		let layer_count = 0;
		for (let x_DOMLayer of x_DOMLayer_list) {
			let x_frames = XML.find_child_element_unique(x_DOMLayer, 'frames');
			let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
			let colse_current_model_if_need = () => {
				if (model !== null) {
					let target_frame = frame_list[Number(model.frame_start + model.frame_duration)];
					target_frame.remove.push({
						index: model.index,
					});
					model = null;
				}
			};
			for (let x_DOMFrame of x_DOMFrame_list) {
				let frame_index = BigInt(x_DOMFrame.attribute.index);
				let frame_duration = BigInt(not_undefined_or(x_DOMFrame.attribute.duration, '1'));
				let transform: Kernel.Tool.PopCap.Animation.Definition.JS_N.VariantTransform;
				let color: Color;
				let x_elements = XML.find_child_element_unique_or_none(x_DOMFrame, 'elements');
				if (x_elements === null) {
					colse_current_model_if_need();
					continue;
				}
				let x_DOMSymbolInstance = XML.find_child_element_unique_or_none(x_elements, 'DOMSymbolInstance');
				if (x_DOMSymbolInstance === null) {
					continue;
				}
				let name_match = /(image|sprite)\/(image|sprite)_([0-9]+)/.exec(x_DOMSymbolInstance.attribute.libraryItemName);
				assert_test(name_match !== null && name_match[1] === name_match[2]);
				let current_instance = {
					resource: BigInt(name_match[3]) - 1n,
					sprite: name_match[1] === 'sprite',
				};
				let x_matrix = XML.find_child_element_unique_or_none(x_DOMSymbolInstance, 'matrix');
				if (x_matrix === null) {
					transform = [0.0, 0.0];
				}
				else {
					let x_Matrix = XML.find_child_element_unique(x_matrix, 'Matrix');
					transform = convert_transform_from_standard_to_variant(parse_transform(x_Matrix));
				}
				let x_color = XML.find_child_element_unique_or_none(x_DOMSymbolInstance, 'color');
				if (x_color === null) {
					color = [...k_initial_color];
				}
				else {
					let x_Color = XML.find_child_element_unique(x_color, 'Color');
					color = parse_color(x_Color);
				}
				let target_frame = frame_list[Number(frame_index)];
				if (model === null) {
					model = {
						index: BigInt(layer_count),
						...current_instance,
						frame_start: frame_index,
						frame_duration: frame_duration,
						color: [...k_initial_color],
					};
					target_frame.append.push({
						index: model.index,
						name: null,
						...current_instance,
						additive: false,
						preload_frame: 0n,
						time_scale: 1.0,
					});
					layer_count++;
				}
				else {
					assert_test(current_instance.resource === model.resource && current_instance.sprite === model.sprite);
				}
				model.frame_start = frame_index;
				model.frame_duration = frame_duration;
				let color_is_changed = !(model.color[0] === color[0] && model.color[1] === color[1] && model.color[2] === color[2] && model.color[3] === color[3]);
				if (color_is_changed) {
					model.color = color;
				}
				target_frame.change.push({
					index: model.index,
					transform,
					color: color_is_changed ? color : null,
					sprite_frame_number: null,
					source_rectangle: null,
				});
			}
			colse_current_model_if_need();
		}
		frame_list.pop();
		return {
			...(version.number < 4n ? {} : { name: extra.name }),
			...(version.number < 4n ? {} : { frame_rate: extra.frame_rate }),
			...(version.number < 5n ? {} : { work_area: extra.work_area === null ? [0n, BigInt(frame_list.length - 1)] : extra.work_area }),
			frame: frame_list,
		};
	}

	function parse_main_document(
		document: Kernel.XML.JS_Element,
		animation: Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation,
		version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
	): void {
		let x_DOMDocument = document;
		assert_test(x_DOMDocument.name === 'DOMDocument');
		let frame_rate = BigInt(not_undefined_or(x_DOMDocument.attribute.frameRate, '24'));
		animation.frame_rate = frame_rate;
		animation.size = [
			Number(x_DOMDocument.attribute.width),
			Number(x_DOMDocument.attribute.height),
		];
		if (animation.main_sprite !== null) {
			let x_timelines = XML.find_child_element_unique(x_DOMDocument, 'timelines');
			let x_DOMTimeline = XML.find_child_element_unique(x_timelines, 'DOMTimeline');
			assert_test(x_DOMTimeline.attribute.name === 'animation');
			let x_layers = XML.find_child_element_unique(x_DOMTimeline, 'layers');
			let x_DOMLayer_list = XML.find_child_element(x_layers, 'DOMLayer');
			assert_test(x_DOMLayer_list.length === 3);
			{
				let x_DOMLayer = x_DOMLayer_list[0];
				assert_test(x_DOMLayer.attribute.name === 'flow');
				let x_frames = XML.find_child_element_unique(x_DOMLayer, 'frames');
				let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
				for (let x_DOMFrame of x_DOMFrame_list) {
					let frame_index = Number(x_DOMFrame.attribute.index);
					if (x_DOMFrame.attribute.name !== undefined) {
						assert_test(x_DOMFrame.attribute.labelType === 'name');
						animation.main_sprite.frame[frame_index].label = x_DOMFrame.attribute.name;
					}
					let x_Actionscript = XML.find_child_element_unique_or_none(x_DOMFrame, 'Actionscript');
					if (x_Actionscript === null) {
						continue;
					}
					let x_script = XML.find_child_element_unique(x_Actionscript, 'script');
					assert_test(x_script.child.length === 1);
					let x_script_text = x_script.child[0];
					assert_test(x_script_text.type === 'text');
					assert_test(x_script_text.value.value.trim() === 'stop();');
					animation.main_sprite.frame[frame_index].stop = true;
				}
			}
			{
				let x_DOMLayer = x_DOMLayer_list[1];
				assert_test(x_DOMLayer.attribute.name === 'command');
				let x_frames = XML.find_child_element_unique(x_DOMLayer, 'frames');
				let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
				for (let x_DOMFrame of x_DOMFrame_list) {
					let frame_index = Number(x_DOMFrame.attribute.index);
					let x_Actionscript = XML.find_child_element_unique_or_none(x_DOMFrame, 'Actionscript');
					if (x_Actionscript === null) {
						continue;
					}
					let x_script = XML.find_child_element_unique(x_Actionscript, 'script');
					assert_test(x_script.child.length === 1);
					let x_script_text = x_script.child[0];
					assert_test(x_script_text.type === 'text');
					let command_list = x_script_text.value.value.trim().split('\n');
					for (let command of command_list) {
						let regex_result = /fscommand\("(.*)", "(.*)"\);/.exec(command.trim());
						assert_test(regex_result !== null);
						animation.main_sprite.frame[frame_index].command.push([
							regex_result[1],
							regex_result[2],
						]);
					}
				}
			}
			{
				let x_DOMLayer = x_DOMLayer_list[2];
				assert_test(x_DOMLayer.attribute.name === 'instance');
				let x_frames = XML.find_child_element_unique(x_DOMLayer, 'frames');
				let x_DOMFrame = XML.find_child_element_unique(x_frames, 'DOMFrame');
				let x_elements = XML.find_child_element_unique(x_DOMFrame, 'elements');
				let x_DOMSymbolInstance = XML.find_child_element_unique(x_elements, 'DOMSymbolInstance');
				assert_test(x_DOMSymbolInstance.attribute.libraryItemName === 'main_sprite');
			}
		}
		return undefined!;
	}

	export function to(
		flash: FlashPackage,
		version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
	): Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation {
		let animation: Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation = {
			frame_rate: undefined!,
			position: undefined!,
			size: undefined!,
			image: [],
			sprite: [],
			main_sprite: undefined!,
		};
		animation.position = flash.extra.position;
		for (let index = 0; index < flash.library.image.length; ++index) {
			let value_document = flash.library.image[index];
			let value_extra = flash.extra.image[index];
			let value = parse_image_document(value_document, value_extra, index, version);
			animation.image.push(value);
		}
		for (let index = 0; index < flash.library.sprite.length; ++index) {
			let value_document = flash.library.sprite[index];
			let value_extra = flash.extra.sprite[index];
			let value = parse_sprite_document(value_document, value_extra, index, version);
			animation.sprite.push(value);
		}
		if (flash.library.main_sprite === null) {
			assert_test(version.number >= 4n);
			animation.main_sprite = null;
		}
		else {
			assert_test(flash.extra.main_sprite !== null);
			let value_document = flash.library.main_sprite;
			let value_extra = flash.extra.main_sprite;
			let value = parse_sprite_document(value_document, value_extra, null, version);
			animation.main_sprite = value;
		}
		parse_main_document(flash.document, animation, version);
		return animation;
	}

	// ------------------------------------------------

	export function to_fsh(
		raw_file: string,
		ripe: FlashPackage,
		version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
	): void {
		let raw = to(ripe, version);
		KernelX.JSON.write_fs_js(raw_file, raw);
		return;
	}

	export function to_fs(
		raw_file: string,
		ripe_directory: string,
		version: typeof Kernel.Tool.PopCap.Animation.Version.Value,
	): void {
		let ripe = load_flash_package(ripe_directory);
		to_fsh(raw_file, ripe, version);
		return;
	}

	// ------------------------------------------------

}