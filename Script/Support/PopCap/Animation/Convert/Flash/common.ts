namespace Twinning.Script.Support.PopCap.Animation.Convert.Flash {

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
		document: Kernel.XML.JS_Element;
		library: {
			image: Array<Kernel.XML.JS_Element>;
			sprite: Array<Kernel.XML.JS_Element>;
			main_sprite: null | Kernel.XML.JS_Element;
		};
	};

	export function save_flash_package(
		directory: string,
		data: FlashPackage,
	): void {
		KernelX.JSON.write_fs_js(`${directory}/extra.json`, data.extra);
		KernelX.XML.write_fs_js(`${directory}/DOMDocument.xml`, XML.wrap_element(data.document));
		data.library.image.forEach((value, index) => {
			KernelX.XML.write_fs_js(`${directory}/LIBRARY/image/image_${index + 1}.xml`, XML.wrap_element(value));
		});
		data.library.sprite.forEach((value, index) => {
			KernelX.XML.write_fs_js(`${directory}/LIBRARY/sprite/sprite_${index + 1}.xml`, XML.wrap_element(value));
		});
		if (data.library.main_sprite !== null) {
			KernelX.XML.write_fs_js(`${directory}/LIBRARY/main_sprite.xml`, XML.wrap_element(data.library.main_sprite));
		}
		return;
	}

	export function load_flash_package(
		directory: string,
	): FlashPackage {
		let extra = KernelX.JSON.read_fs_js<ExtraInformation>(`${directory}/extra.json`);
		let document = KernelX.XML.read_fs_js(`${directory}/DOMDocument.xml`).value as Kernel.XML.JS_Element;
		let library = {
			image: extra.image.map((value, index) => (KernelX.XML.read_fs_js(`${directory}/LIBRARY/image/image_${index + 1}.xml`).value as Kernel.XML.JS_Element)),
			sprite: extra.sprite.map((value, index) => (KernelX.XML.read_fs_js(`${directory}/LIBRARY/sprite/sprite_${index + 1}.xml`).value as Kernel.XML.JS_Element)),
			main_sprite: extra.main_sprite === null ? null : KernelX.XML.read_fs_js(`${directory}/LIBRARY/main_sprite.xml`).value as Kernel.XML.JS_Element,
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
		KernelX.Storage.write_file_s(`${directory}/main.xfl`, k_xfl_content);
		return;
	}

	// ------------------------------------------------

}