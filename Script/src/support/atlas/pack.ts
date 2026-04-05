namespace Twinning.Script.Support.Atlas.Pack {

	// #region common

	export type SpriteDefinition = {
		name: string;
		position: [bigint, bigint];
		size: [bigint, bigint];
	};

	export type AtlasDefinition = {
		size: [bigint, bigint];
		sprite: Array<SpriteDefinition>;
	};

	// #endregion

	// #region utility for fsh

	export function pack_fsh(
		definition: AtlasDefinition,
		atlas: Kernel.Image.ImageView,
		sprite_directory: StoragePath,
	): void {
		for (let sprite_definition of definition.sprite) {
			KernelX.Tool.Texture.File.Png.read_fs(sprite_directory.join(`${sprite_definition.name}.png`), atlas.sub(Kernel.Image.ImagePosition.value(sprite_definition.position), Kernel.Image.ImageSize.value(sprite_definition.size)));
		}
		return;
	}

	export function unpack_fsh(
		definition: AtlasDefinition,
		atlas: Kernel.Image.ConstantImageView,
		sprite_directory: StoragePath,
	): void {
		for (let sprite_definition of definition.sprite) {
			KernelX.Tool.Texture.File.Png.write_fs(sprite_directory.join(`${sprite_definition.name}.png`), atlas.sub(Kernel.Image.ImagePosition.value(sprite_definition.position), Kernel.Image.ImageSize.value(sprite_definition.size)));
		}
		return;
	}

	// ----------------

	export function pack_automatic_fsh(
		sprite_directory: StoragePath,
		expand_value: number | 'exponent_of_2',
	): [AtlasDefinition, Kernel.Image.Image] {
		let sprite_file_list = StorageHelper.list_directory(sprite_directory, null, true, false, true, false).filter((value) => /.+(\.png)/i.test(value.name()!)).map((value) => value.parent()!.join(value.stem()!));
		let sprite_box = ConvertHelper.record_from_array(sprite_file_list, (index, value) => {
			let size = KernelX.Tool.Texture.File.Png.size_fs(sprite_directory.push(value.parent()!).join(`${value.stem()}.png`));
			return [value.emit(), {width: Number(size[0]), height: Number(size[1])}];
		});
		let [atlas_box, sprite_rect] = PackAutomatic.pack_automatic_best(sprite_box, expand_value === 'exponent_of_2' ? PackAutomatic.expander_exponent_of_2_generator(false) : PackAutomatic.expander_fixed_generator(false, expand_value));
		let definition: AtlasDefinition = {
			size: [BigInt(atlas_box.width), BigInt(atlas_box.height)],
			sprite: [],
		};
		let atlas = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(definition.size));
		let atlas_view = atlas.view();
		for (let sprite_file in sprite_rect) {
			let rect = sprite_rect[sprite_file];
			let sprite_definition: SpriteDefinition = {
				name: sprite_file,
				position: [BigInt(rect.x), BigInt(rect.y)],
				size: [BigInt(rect.width), BigInt(rect.height)],
			};
			definition.sprite.push(sprite_definition);
			let sprite = atlas_view.sub(Kernel.Image.ImagePosition.value(sprite_definition.position), Kernel.Image.ImageSize.value(sprite_definition.size));
			KernelX.Tool.Texture.File.Png.read_fs(sprite_directory.join(`${sprite_file}.png`), sprite);
		}
		return [definition, atlas];
	}

	// #endregion

	// #region utility for fs

	export function pack_fs(
		definition_file: StoragePath,
		atlas_file: StoragePath,
		sprite_directory: StoragePath,
	): void {
		let definition = JsonHelper.decode_file(definition_file) as AtlasDefinition;
		let atlas = Kernel.Image.Image.allocate(Kernel.Image.ImageSize.value(definition.size));
		let atlas_view = atlas.view();
		pack_fsh(definition, atlas_view, sprite_directory);
		KernelX.Tool.Texture.File.Png.write_fs(atlas_file, atlas_view);
		return;
	}

	export function unpack_fs(
		definition_file: StoragePath,
		atlas_file: StoragePath,
		sprite_directory: StoragePath,
	): void {
		let definition = JsonHelper.decode_file(definition_file) as AtlasDefinition;
		let atlas = KernelX.Tool.Texture.File.Png.read_fs_of(atlas_file);
		let atlas_view = atlas.view();
		unpack_fsh(definition, atlas_view, sprite_directory);
		return;
	}

	// ----------------

	export function pack_automatic_fs(
		definition_file: StoragePath,
		atlas_file: StoragePath,
		sprite_directory: StoragePath,
		expand_value: number | 'exponent_of_2',
	): void {
		let [definition, atlas] = pack_automatic_fsh(sprite_directory, expand_value);
		JsonHelper.encode_file(definition_file, definition);
		KernelX.Tool.Texture.File.Png.write_fs(atlas_file, atlas.view());
		return;
	}

	// #endregion

}
