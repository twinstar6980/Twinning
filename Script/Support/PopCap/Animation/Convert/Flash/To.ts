namespace TwinStar.Script.Support.PopCap.Animation.Convert.Flash.To {

	// ------------------------------------------------

	function parse_transform_origin(
		x_Matrix: Kernel.XML.JS_Element,
	): [number, number] {
		return [
			Number(defined_or(x_Matrix.attribute.x, '0')),
			Number(defined_or(x_Matrix.attribute.y, '0')),
		];
	}

	function parse_transform(
		x_Matrix: Kernel.XML.JS_Element,
	): Transform {
		return [
			Number(defined_or(x_Matrix.attribute.a, '1')),
			Number(defined_or(x_Matrix.attribute.b, '0')),
			Number(defined_or(x_Matrix.attribute.c, '0')),
			Number(defined_or(x_Matrix.attribute.d, '1')),
			Number(defined_or(x_Matrix.attribute.tx, '0')),
			Number(defined_or(x_Matrix.attribute.ty, '0')),
		];
	}

	function parse_color(
		x_Matrix: Kernel.XML.JS_Element,
	): Color {
		let compute = (multiplier_s: undefined | string, offset_s: undefined | string) => (Math.max(0, Math.min(255, Number(defined_or(multiplier_s, '1')) * 255 + Number(defined_or(offset_s, '0')))) / 255);
		return [
			compute(x_Matrix.attribute.redMultiplier, x_Matrix.attribute.redOffset),
			compute(x_Matrix.attribute.greenMultiplier, x_Matrix.attribute.greenOffset),
			compute(x_Matrix.attribute.blueMultiplier, x_Matrix.attribute.blueOffset),
			compute(x_Matrix.attribute.alphaMultiplier, x_Matrix.attribute.alphaOffset),
		];
	}

	function parse_image_document(
		x_DOMSymbolItem: Kernel.XML.JS_Element,
		index: number,
	): Transform {
		assert_test(x_DOMSymbolItem.name === 'DOMSymbolItem');
		assert_test(x_DOMSymbolItem.attribute.name === `image/image_${index + 1}`);
		let x_timeline_list = XML.find_child_element(x_DOMSymbolItem, 'timeline');
		assert_test(x_timeline_list.length === 1);
		let x_timeline = x_timeline_list[0];
		let x_DOMTimeline_list = XML.find_child_element(x_timeline, 'DOMTimeline');
		assert_test(x_DOMTimeline_list.length === 1);
		let x_DOMTimeline = x_DOMTimeline_list[0];
		assert_test(x_DOMTimeline.attribute.name === `image_${index + 1}`);
		let x_layers_list = XML.find_child_element(x_DOMTimeline, 'layers');
		assert_test(x_layers_list.length === 1);
		let x_layers = x_layers_list[0];
		let x_DOMLayer_list = XML.find_child_element(x_layers, 'DOMLayer');
		assert_test(x_DOMLayer_list.length === 1);
		let x_DOMLayer = x_DOMLayer_list[0];
		let x_frames_list = XML.find_child_element(x_DOMLayer, 'frames');
		assert_test(x_frames_list.length === 1);
		let x_frames = x_frames_list[0];
		let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
		assert_test(x_DOMFrame_list.length === 1);
		let x_DOMFrame = x_DOMFrame_list[0];
		let x_elements_list = XML.find_child_element(x_DOMFrame, 'elements');
		assert_test(x_elements_list.length === 1);
		let x_elements = x_elements_list[0];
		let x_DOMSymbolInstance_list = XML.find_child_element(x_elements, 'DOMSymbolInstance');
		assert_test(x_DOMSymbolInstance_list.length === 1);
		let x_DOMSymbolInstance = x_DOMSymbolInstance_list[0];
		assert_test(x_DOMSymbolInstance.attribute.libraryItemName === `source/source_${index + 1}`);
		let transform: Transform;
		let x_matrix_list = XML.find_child_element(x_DOMSymbolInstance, 'matrix');
		if (x_matrix_list.length === 0) {
			transform = [1.0, 0.0, 0.0, 1.0, 0.0, 0.0];
		} else {
			assert_test(x_matrix_list.length === 1);
			let x_matrix = x_matrix_list[0];
			let x_Matrix_list = XML.find_child_element(x_matrix, 'Matrix');
			assert_test(x_Matrix_list.length === 1);
			let x_Matrix = x_Matrix_list[0];
			transform = parse_transform(x_Matrix);
		}
		return transform;
	}

	function parse_sprite_document(
		x_DOMSymbolItem: Kernel.XML.JS_Element,
		index: number | 'main',
	): Array<Kernel.Tool.PopCap.Animation.Definition.JS_N.Frame> {
		let model: null | {
			index: bigint;
			resource: bigint;
			sprite: boolean;
			frame_start: bigint;
			frame_duration: bigint;
			color: Color;
		} = null;
		let result: Array<Kernel.Tool.PopCap.Animation.Definition.JS_N.Frame>;
		assert_test(x_DOMSymbolItem.name === 'DOMSymbolItem');
		assert_test(x_DOMSymbolItem.attribute.name === (index === 'main' ? `main_sprite` : `sprite/sprite_${index + 1}`));
		let x_timeline_list = XML.find_child_element(x_DOMSymbolItem, 'timeline');
		assert_test(x_timeline_list.length === 1);
		let x_timeline = x_timeline_list[0];
		let x_DOMTimeline_list = XML.find_child_element(x_timeline, 'DOMTimeline');
		assert_test(x_DOMTimeline_list.length === 1);
		let x_DOMTimeline = x_DOMTimeline_list[0];
		assert_test(x_DOMTimeline.attribute.name === (index === 'main' ? `main_sprite` : `sprite_${index + 1}`));
		let x_layers_list = XML.find_child_element(x_DOMTimeline, 'layers');
		assert_test(x_layers_list.length === 1);
		let x_layers = x_layers_list[0];
		let x_DOMLayer_list = XML.find_child_element(x_layers, 'DOMLayer');
		assert_test(x_DOMLayer_list.length >= 1);
		{
			let x_DOMLayer = x_DOMLayer_list[x_DOMLayer_list.length - 1];
			let x_frames_list = XML.find_child_element(x_DOMLayer, 'frames');
			assert_test(x_frames_list.length === 1);
			let x_frames = x_frames_list[0];
			let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
			assert_test(x_DOMFrame_list.length === 1);
			let x_DOMFrame = x_DOMFrame_list[0];
			let frame_index = BigInt(x_DOMFrame.attribute.index);
			let frame_duration = BigInt(defined_or(x_DOMFrame.attribute.duration, '1'));
			assert_test(frame_index === 0n && frame_duration > 0n);
			let x_elements_list = XML.find_child_element(x_DOMFrame, 'elements');
			assert_test(x_elements_list.length === 1);
			let x_elements = x_elements_list[0];
			assert_test(x_elements.child.length === 0);
			result = new Array(Number(frame_duration) + 1);
			for (let index = 0; index < result.length; index++) {
				result[index] = {
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
		x_DOMLayer_list.forEach((x_DOMLayer) => {
			let x_frames_list = XML.find_child_element(x_DOMLayer, 'frames');
			assert_test(x_frames_list.length === 1);
			let x_frames = x_frames_list[0];
			let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
			let colse_current_model_if_need = () => {
				if (model !== null) {
					let target_frame = result[Number(model.frame_start + model.frame_duration)];
					target_frame.remove.push({
						index: model.index,
					});
					model = null;
				}
			};
			x_DOMFrame_list.forEach((x_DOMFrame) => {
				let frame_index = BigInt(x_DOMFrame.attribute.index);
				let frame_duration = BigInt(defined_or(x_DOMFrame.attribute.duration, '1'));
				let transform: Kernel.Tool.PopCap.Animation.Definition.JS_N.VariantTransform;
				let color: Color;
				let x_elements_list = XML.find_child_element(x_DOMFrame, 'elements');
				if (x_elements_list.length === 0) {
					colse_current_model_if_need();
					return;
				}
				assert_test(x_elements_list.length === 1);
				let x_elements = x_elements_list[0];
				let x_DOMSymbolInstance_list = XML.find_child_element(x_elements, 'DOMSymbolInstance');
				if (x_DOMSymbolInstance_list.length === 0) {
					return;
				}
				assert_test(x_DOMSymbolInstance_list.length === 1);
				let x_DOMSymbolInstance = x_DOMSymbolInstance_list[0];
				let name_match = /(image|sprite)\/(image|sprite)_([0-9]+)/.exec(x_DOMSymbolInstance.attribute.libraryItemName);
				assert_test(name_match !== null && name_match[1] === name_match[2]);
				let current_instance = {
					resource: BigInt(name_match[3]) - 1n,
					sprite: name_match[1] === 'sprite',
				};
				let x_matrix_list = XML.find_child_element(x_DOMSymbolInstance, 'matrix');
				if (x_matrix_list.length === 0) {
					transform = [0.0, 0.0];
				} else {
					assert_test(x_matrix_list.length === 1);
					let x_matrix = x_matrix_list[0];
					let x_Matrix_list = XML.find_child_element(x_matrix, 'Matrix');
					assert_test(x_Matrix_list.length === 1);
					let x_Matrix = x_Matrix_list[0];
					transform = compute_variant_transform_from_standard(parse_transform(x_Matrix));
				}
				let x_color_list = XML.find_child_element(x_DOMSymbolInstance, 'color');
				if (x_color_list.length === 0) {
					color = [...k_initial_color];
				} else {
					assert_test(x_color_list.length === 1);
					let x_color = x_color_list[0];
					let x_Color_list = XML.find_child_element(x_color, 'Color');
					assert_test(x_Color_list.length === 1);
					let x_Color = x_Color_list[0];
					color = parse_color(x_Color);
				}
				let target_frame = result[Number(frame_index)];
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
				} else {
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
			});
			colse_current_model_if_need();
		});
		return result.slice(0, -1);
	}

	export function to(
		flash: FlashPackage,
	): Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation {
		let x_DOMDocument = flash.document;
		assert_test(x_DOMDocument.name === 'DOMDocument');
		{
			let x_media_list = XML.find_child_element(x_DOMDocument, 'media');
			assert_test(x_media_list.length === 1);
			let x_media = x_media_list[0];
			let x_DOMBitmapItem_list = XML.find_child_element(x_media, 'DOMBitmapItem');
		}
		{
			let x_symbols_list = XML.find_child_element(x_DOMDocument, 'symbols');
			assert_test(x_symbols_list.length === 1);
			let x_symbols = x_symbols_list[0];
			let x_Include_list = XML.find_child_element(x_symbols, 'Include');
		}
		let main_sprite_frame = parse_sprite_document(flash.library.main_sprite, 'main');
		{
			let x_timelines_list = XML.find_child_element(x_DOMDocument, 'timelines');
			assert_test(x_timelines_list.length === 1);
			let x_timelines = x_timelines_list[0];
			let x_DOMTimeline_list = XML.find_child_element(x_timelines, 'DOMTimeline');
			assert_test(x_DOMTimeline_list.length === 1);
			let x_DOMTimeline = x_DOMTimeline_list[0];
			assert_test(x_DOMTimeline.attribute.name === 'animation');
			let x_layers_list = XML.find_child_element(x_DOMTimeline, 'layers');
			assert_test(x_layers_list.length === 1);
			let x_layers = x_layers_list[0];
			let x_DOMLayer_list = XML.find_child_element(x_layers, 'DOMLayer');
			assert_test(x_DOMLayer_list.length === 3);
			{
				let x_DOMLayer_flow = x_DOMLayer_list[0];
				let x_frames_list = XML.find_child_element(x_DOMLayer_flow, 'frames');
				assert_test(x_frames_list.length === 1);
				let x_frames = x_frames_list[0];
				let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
				x_DOMFrame_list.forEach((x_DOMFrame) => {
					let frame_index = Number(x_DOMFrame.attribute.index);
					if (x_DOMFrame.attribute.name !== undefined) {
						assert_test(x_DOMFrame.attribute.labelType === 'name');
						main_sprite_frame[frame_index].label = x_DOMFrame.attribute.name;
					}
					let x_Actionscript_list = XML.find_child_element(x_DOMFrame, 'Actionscript');
					if (x_Actionscript_list.length === 0) {
						return;
					}
					assert_test(x_Actionscript_list.length === 1);
					let x_Actionscript = x_Actionscript_list[0];
					assert_test(x_Actionscript.child.length === 1);
					let x_script_list = XML.find_child_element(x_Actionscript, 'script');
					assert_test(x_script_list.length === 1);
					let x_script = x_script_list[0];
					assert_test(x_script.child.length === 1);
					let x_script_text = x_script.child[0];
					assert_test(x_script_text.type === 'text');
					assert_test(x_script_text.value.value.trim() === 'stop();');
					main_sprite_frame[frame_index].stop = true;
				});
			}
			{
				let x_DOMLayer_command = x_DOMLayer_list[1];
				let x_frames_list = XML.find_child_element(x_DOMLayer_command, 'frames');
				assert_test(x_frames_list.length === 1);
				let x_frames = x_frames_list[0];
				let x_DOMFrame_list = XML.find_child_element(x_frames, 'DOMFrame');
				x_DOMFrame_list.forEach((x_DOMFrame) => {
					let frame_index = Number(x_DOMFrame.attribute.index);
					let x_Actionscript_list = XML.find_child_element(x_DOMFrame, 'Actionscript');
					if (x_Actionscript_list.length === 0) {
						return;
					}
					assert_test(x_Actionscript_list.length === 1);
					let x_Actionscript = x_Actionscript_list[0];
					assert_test(x_Actionscript.child.length === 1);
					let x_script_list = XML.find_child_element(x_Actionscript, 'script');
					assert_test(x_script_list.length === 1);
					let x_script = x_script_list[0];
					assert_test(x_script.child.length === 1);
					let x_script_text = x_script.child[0];
					assert_test(x_script_text.type === 'text');
					let command_string = x_script_text.value.value.trim().split('\n');
					for (let e of command_string) {
						let regex_result = /fscommand\("(.*)", "(.*)"\);/.exec(e.trim());
						assert_test(regex_result !== null);
						main_sprite_frame[frame_index].command.push([
							regex_result[1],
							regex_result[2],
						]);
					}
				});
			}
			{
				let x_DOMLayer_sprite = x_DOMLayer_list[2];
				// TODO : check
			}
		}
		let frame_rate = BigInt(defined_or(x_DOMDocument.attribute.frameRate, '24'));
		let width = Number(x_DOMDocument.attribute.width);
		let height = Number(x_DOMDocument.attribute.height);
		return {
			frame_rate: frame_rate,
			position: flash.extra.position,
			size: [width, height],
			image: flash.extra.image.map((e, i) => ({ name: e.name, size: e.size, transform: parse_image_document(flash.library.image[i], i) })),
			sprite: flash.extra.sprite.map((e, i) => {
				let frame = parse_sprite_document(flash.library.sprite[i], i);
				return { name: e.name, frame_rate: Number(frame_rate), work_area: [0n, BigInt(frame.length - 1)], frame: frame };
			}),
			main_sprite: { name: flash.extra.main_sprite!.name, frame_rate: Number(frame_rate), work_area: [0n, BigInt(main_sprite_frame.length - 1)], frame: main_sprite_frame },
		};
	}

	// ------------------------------------------------

	export function to_fsh(
		raw_file: string,
		ripe: FlashPackage,
	): void {
		let raw = to(ripe);
		KernelX.JSON.write_fs_js(raw_file, raw);
		return;
	}

	export function to_fs(
		raw_file: string,
		ripe_directory: string,
	): void {
		let ripe = load_flash_package(ripe_directory);
		to_fsh(raw_file, ripe);
		return;
	}

	// ------------------------------------------------

}