/** PAM转换Flash */
namespace TwinStar.Support.PopCapAnimation.Convert.Flash {

	// ------------------------------------------------

	export const k_standard_resolution = 1200n;

	// ------------------------------------------------

	export const k_xfl_content = 'PROXY-CS5';

	export const k_xfl_version = '2.971';

	export const k_xmlns_attribute = {
		'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema-instance',
		'xmlns': 'http://ns.adobe.com/xfl/2008/',
	};

	// ------------------------------------------------

	export type ExtraInformation = {
		position: [number, number];
		image: Array<{
			name: string;
			size: [bigint, bigint];
		}>;
		sprite: Array<{
			name: string;
		}>;
		main_sprite: null | {
			name: string;
		};
	};

	export type FlashPackage = {
		extra: ExtraInformation;
		document: Core.XML.JS_Element;
		library: {
			image: Array<Core.XML.JS_Element>;
			sprite: Array<Core.XML.JS_Element>;
			main_sprite: Core.XML.JS_Element;
		};
	};

	export function save_flash_package(
		data: FlashPackage,
		directory: string,
	): void {
		CoreX.JSON.write_fs_js(`${directory}/extra.json`, data.extra);
		CoreX.XML.write_fs_js(`${directory}/DOMDocument.xml`, XML.wrap_element(data.document));
		data.library.image.forEach((e, i) => {
			CoreX.XML.write_fs_js(`${directory}/LIBRARY/image/image_${i + 1}.xml`, XML.wrap_element(e));
		});
		data.library.sprite.forEach((e, i) => {
			CoreX.XML.write_fs_js(`${directory}/LIBRARY/sprite/sprite_${i + 1}.xml`, XML.wrap_element(e));
		});
		CoreX.XML.write_fs_js(`${directory}/LIBRARY/main_sprite.xml`, XML.wrap_element(data.library.main_sprite));
		return;
	}

	export function load_flash_package(
		directory: string,
	): FlashPackage {
		let extra = CoreX.JSON.read_fs_js<ExtraInformation>(`${directory}/extra.json`);
		let document = CoreX.XML.read_fs_js(`${directory}/DOMDocument.xml`).value as Core.XML.JS_Element;
		let library = {
			image: extra.image.map((e, i) => (CoreX.XML.read_fs_js(`${directory}/LIBRARY/image/image_${i + 1}.xml`).value as Core.XML.JS_Element)),
			sprite: extra.sprite.map((e, i) => (CoreX.XML.read_fs_js(`${directory}/LIBRARY/sprite/sprite_${i + 1}.xml`).value as Core.XML.JS_Element)),
			main_sprite: CoreX.XML.read_fs_js(`${directory}/LIBRARY/main_sprite.xml`).value as Core.XML.JS_Element,
		};
		return {
			extra,
			document,
			library,
		};
	}

	// ------------------------------------------------

	export function create_xfl_content_file(
		directory: string,
	): void {
		let data = new ArrayBuffer(k_xfl_content.length);
		let data_view = new DataView(data);
		for (let i = 0; i < k_xfl_content.length; ++i) {
			data_view.setUint8(i, k_xfl_content.charCodeAt(i));
		}
		CoreX.FileSystem.write_file(`${directory}/main.xfl`, data);
		return;
	}

	// ------------------------------------------------

}