namespace Twinning.Script.Support.Popcap.Animation.Convert.Flash {

	// #region common

	export const k_standard_resolution = 1200n;

	// #endregion

	// #region xfl

	export const k_xfl_content = 'PROXY-CS5';

	export const k_xfl_version = '2.971';

	export const k_xmlns_attribute = {
		'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema-instance',
		'xmlns': 'http://ns.adobe.com/xfl/2008/',
	};

	// ----------------

	export function create_xfl_content_file(
		directory: StoragePath,
	): void {
		StorageHelper.write_file_text(directory.join('main.xfl'), k_xfl_content);
		return;
	}

	// #endregion

	// #region flash package

	export type ExtraInformation = {
		position: [number, number];
		image: Array<{
			name: string;
			size: [bigint, bigint];
		}>;
		sprite: Array<{
			name: string;
			frame_rate: number;
			work_area: null | [bigint, bigint];
		}>;
		main_sprite: null | {
			name: string;
			frame_rate: number;
			work_area: null | [bigint, bigint];
		};
	};

	export type FlashPackage = {
		extra: ExtraInformation;
		document: Kernel.Xml.JS_Element;
		library: {
			image: Array<Kernel.Xml.JS_Element>;
			sprite: Array<Kernel.Xml.JS_Element>;
			main_sprite: null | Kernel.Xml.JS_Element;
		};
	};

	// ----------------

	export function save_flash_package(
		directory: StoragePath,
		data: FlashPackage,
	): void {
		JsonHelper.encode_file(directory.join('extra.json'), data.extra);
		XmlHelper.encode_file(directory.join('DOMDocument.xml'), XmlHelper.wrap_element(data.document));
		data.library.image.forEach((value, index) => {
			XmlHelper.encode_file(directory.join('LIBRARY').join('image').join(`image_${index + 1}.xml`), XmlHelper.wrap_element(value));
		});
		data.library.sprite.forEach((value, index) => {
			XmlHelper.encode_file(directory.join('LIBRARY').join('sprite').join(`sprite_${index + 1}.xml`), XmlHelper.wrap_element(value));
		});
		if (data.library.main_sprite !== null) {
			XmlHelper.encode_file(directory.join('LIBRARY').join('main_sprite.xml'), XmlHelper.wrap_element(data.library.main_sprite));
		}
		return;
	}

	export function load_flash_package(
		directory: StoragePath,
	): FlashPackage {
		let extra = JsonHelper.decode_file(directory.join('extra.json')) as ExtraInformation;
		let document = XmlHelper.decode_file(directory.join('DOMDocument.xml'),).value as Kernel.Xml.JS_Element;
		let library = {
			image: extra.image.map((value, index) => (XmlHelper.decode_file(directory.join('LIBRARY').join('image').join(`image_${index + 1}.xml`)).value as Kernel.Xml.JS_Element)),
			sprite: extra.sprite.map((value, index) => (XmlHelper.decode_file(directory.join('LIBRARY').join('sprite').join(`sprite_${index + 1}.xml`)).value as Kernel.Xml.JS_Element)),
			main_sprite: extra.main_sprite === null ? null : XmlHelper.decode_file(directory.join('LIBRARY').join('main_sprite.xml')).value as Kernel.Xml.JS_Element,
		};
		return {
			extra,
			document,
			library,
		};
	}

	// #endregion

}
