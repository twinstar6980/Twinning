namespace TwinStar.Script.Support.Atlas.Pack {

	// ------------------------------------------------

	export type SpriteInformation = {
		position: [bigint, bigint];
		size: [bigint, bigint];
	};

	export type Manifest = {
		size: [bigint, bigint];
		sprite: Record<string, SpriteInformation>;
	};

	// ------------------------------------------------

	export function pack_fsh(
		manifest: Manifest,
		atlas: Core.Image.BitmapView,
		sprite_directory: string,
	): void {
		for (let sprite_name in manifest.sprite) {
			let e = manifest.sprite[sprite_name];
			CoreX.Image.File.PNG.read_fs(`${sprite_directory}/${sprite_name}.png`, atlas.sub(Core.Image.ImagePosition.value(e.position), Core.Image.ImageSize.value(e.size)));
		}
		return;
	}

	export function unpack_fsh(
		manifest: Manifest,
		atlas: Core.Image.CBitmapView,
		sprite_directory: string,
	): void {
		for (let sprite_name in manifest.sprite) {
			let e = manifest.sprite[sprite_name];
			CoreX.Image.File.PNG.write_fs(`${sprite_directory}/${sprite_name}.png`, atlas.sub(Core.Image.ImagePosition.value(e.position), Core.Image.ImageSize.value(e.size)));
		}
		return;
	}

	export function pack_auto_fsh(
		sprite_directory: string,
		expand_value: number | 'exponent_of_2' = 'exponent_of_2',
	): [Manifest, Core.Image.Bitmap] {
		let sprite_file_list = CoreX.FileSystem.list_file(sprite_directory).filter((e) => (/.+(\.png)/i.test(e))).map((e) => (e.substring(0, e.length - 4)));
		let sprite_box = record_from_array(sprite_file_list, (i, e) => {
			let size = CoreX.Image.File.PNG.size_fs(`${sprite_directory}/${e}.png`);
			return [e, { w: Number(size[0]), h: Number(size[1]) }];
		});
		let [atlas_box, sprite_rect] = AutoPack.pack_auto_best(sprite_box, expand_value === 'exponent_of_2' ? AutoPack.expander_exponent_of_2_generator(false) : AutoPack.expander_fixed_generator(false, expand_value));
		let manifest: Manifest = {
			size: [BigInt(atlas_box.w), BigInt(atlas_box.h)],
			sprite: {},
		};
		let atlas = Core.Image.Bitmap.allocate(Core.Image.ImageSize.value(manifest.size));
		let atlas_view = atlas.view();
		for (let sprite_file in sprite_rect) {
			let rect = sprite_rect[sprite_file];
			let sprite_information: SpriteInformation = {
				position: [BigInt(rect.x), BigInt(rect.y)],
				size: [BigInt(rect.w), BigInt(rect.h)],
			};
			manifest.sprite[sprite_file] = sprite_information;
			let sprite = atlas_view.sub(Core.Image.ImagePosition.value(sprite_information.position), Core.Image.ImageSize.value(sprite_information.size));
			CoreX.Image.File.PNG.read_fs(`${sprite_directory}/${sprite_file}.png`, sprite);
		}
		return [manifest, atlas];
	}

	// ------------------------------------------------

	export function pack_fs(
		manifest_file: string,
		atlas_file: string,
		sprite_directory: string,
	): void {
		let manifest = CoreX.JSON.read_fs_js<Manifest>(manifest_file);
		let atlas = Core.Image.Bitmap.allocate(Core.Image.ImageSize.value(manifest.size));
		let atlas_view = atlas.view();
		pack_fsh(manifest, atlas_view, sprite_directory);
		CoreX.Image.File.PNG.write_fs(atlas_file, atlas_view);
		return;
	}

	export function unpack_fs(
		manifest_file: string,
		atlas_file: string,
		sprite_directory: string,
	): void {
		let manifest = CoreX.JSON.read_fs_js<Manifest>(manifest_file);
		let atlas = CoreX.Image.File.PNG.read_fs_of(atlas_file);
		let atlas_view = atlas.view();
		unpack_fsh(manifest, atlas_view, sprite_directory);
		return;
	}

	export function pack_auto_fs(
		manifest_file: string,
		atlas_file: string,
		sprite_directory: string,
		expand_value: number | 'exponent_of_2' = 'exponent_of_2',
	): void {
		let [manifest, atlas] = pack_auto_fsh(sprite_directory, expand_value);
		CoreX.JSON.write_fs_js(manifest_file, manifest);
		CoreX.Image.File.PNG.write_fs(atlas_file, atlas.view());
		return;
	}

	// ------------------------------------------------

}