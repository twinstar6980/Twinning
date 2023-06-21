namespace TwinStar.Script.Support.Atlas.Pack {

	// ------------------------------------------------

	export type SpriteDefinition = {
		position: [bigint, bigint];
		size: [bigint, bigint];
	};

	export type AtlasDefinition = {
		size: [bigint, bigint];
		sprite: Record<string, SpriteDefinition>;
	};

	// ------------------------------------------------

	export function pack_fsh(
		definition: AtlasDefinition,
		atlas: Core.Image.ImageView,
		sprite_directory: string,
	): void {
		for (let sprite_name in definition.sprite) {
			let e = definition.sprite[sprite_name];
			CoreX.Image.File.PNG.read_fs(`${sprite_directory}/${sprite_name}.png`, atlas.sub(Core.Image.ImagePosition.value(e.position), Core.Image.ImageSize.value(e.size)));
		}
		return;
	}

	export function unpack_fsh(
		definition: AtlasDefinition,
		atlas: Core.Image.CImageView,
		sprite_directory: string,
	): void {
		for (let sprite_name in definition.sprite) {
			let e = definition.sprite[sprite_name];
			CoreX.Image.File.PNG.write_fs(`${sprite_directory}/${sprite_name}.png`, atlas.sub(Core.Image.ImagePosition.value(e.position), Core.Image.ImageSize.value(e.size)));
		}
		return;
	}

	export function pack_automatic_fsh(
		sprite_directory: string,
		expand_value: number | 'exponent_of_2' = 'exponent_of_2',
	): [AtlasDefinition, Core.Image.Image] {
		let sprite_file_list = CoreX.FileSystem.list_file(sprite_directory).filter((e) => (/.+(\.png)/i.test(e))).map((e) => (e.substring(0, e.length - 4)));
		let sprite_box = record_from_array(sprite_file_list, (i, e) => {
			let size = CoreX.Image.File.PNG.size_fs(`${sprite_directory}/${e}.png`);
			return [e, { w: Number(size[0]), h: Number(size[1]) }];
		});
		let [atlas_box, sprite_rect] = PackAutomatic.pack_automatic_best(sprite_box, expand_value === 'exponent_of_2' ? PackAutomatic.expander_exponent_of_2_generator(false) : PackAutomatic.expander_fixed_generator(false, expand_value));
		let definition: AtlasDefinition = {
			size: [BigInt(atlas_box.w), BigInt(atlas_box.h)],
			sprite: {},
		};
		let atlas = Core.Image.Image.allocate(Core.Image.ImageSize.value(definition.size));
		let atlas_view = atlas.view();
		for (let sprite_file in sprite_rect) {
			let rect = sprite_rect[sprite_file];
			let sprite_information: SpriteDefinition = {
				position: [BigInt(rect.x), BigInt(rect.y)],
				size: [BigInt(rect.w), BigInt(rect.h)],
			};
			definition.sprite[sprite_file] = sprite_information;
			let sprite = atlas_view.sub(Core.Image.ImagePosition.value(sprite_information.position), Core.Image.ImageSize.value(sprite_information.size));
			CoreX.Image.File.PNG.read_fs(`${sprite_directory}/${sprite_file}.png`, sprite);
		}
		return [definition, atlas];
	}

	// ------------------------------------------------

	export function pack_fs(
		definition_file: string,
		atlas_file: string,
		sprite_directory: string,
	): void {
		let definition = CoreX.JSON.read_fs_js<AtlasDefinition>(definition_file);
		let atlas = Core.Image.Image.allocate(Core.Image.ImageSize.value(definition.size));
		let atlas_view = atlas.view();
		pack_fsh(definition, atlas_view, sprite_directory);
		CoreX.Image.File.PNG.write_fs(atlas_file, atlas_view);
		return;
	}

	export function unpack_fs(
		definition_file: string,
		atlas_file: string,
		sprite_directory: string,
	): void {
		let definition = CoreX.JSON.read_fs_js<AtlasDefinition>(definition_file);
		let atlas = CoreX.Image.File.PNG.read_fs_of(atlas_file);
		let atlas_view = atlas.view();
		unpack_fsh(definition, atlas_view, sprite_directory);
		return;
	}

	export function pack_automatic_fs(
		definition_file: string,
		atlas_file: string,
		sprite_directory: string,
		expand_value: number | 'exponent_of_2' = 'exponent_of_2',
	): void {
		let [definition, atlas] = pack_automatic_fsh(sprite_directory, expand_value);
		CoreX.JSON.write_fs_js(definition_file, definition);
		CoreX.Image.File.PNG.write_fs(atlas_file, atlas.view());
		return;
	}

	// ------------------------------------------------

}