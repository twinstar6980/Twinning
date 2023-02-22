namespace TwinStar.Script.Support.PopCapAnimation.Convert.Flash.SourceManager {

	// ------------------------------------------------

	function make_scale_matrix(
		resolution: bigint,
	): {
		a: string,
		d: string,
	} {
		let scale = Number(k_standard_resolution) / Number(resolution);
		return {
			a: `${scale.toFixed(6)}`,
			d: `${scale.toFixed(6)}`,
		};
	}

	// ------------------------------------------------

	function create_one(
		index: number,
		image: Core.Tool.PopCap.PAM.Manifest.JS_N.Image,
		resolution: bigint,
	): Core.XML.JS_Element {
		return XML.create_element('DOMSymbolItem', {
			...k_xmlns_attribute,
			name: `source/source_${index + 1}`,
			symbolType: 'graphic',
		}, [
			XML.create_element_node('timeline', {}, [
				XML.create_element_node('DOMTimeline', {
					name: `source_${index + 1}`,
				}, [
					XML.create_element_node('layers', {}, [
						XML.create_element_node('DOMLayer', {}, [
							XML.create_element_node('frames', {}, [
								XML.create_element_node('DOMFrame', {
									index: `0`,
								}, [
									XML.create_element_node('elements', {}, [
										XML.create_element_node('DOMBitmapInstance', {
											libraryItemName: `media/${image.name.split('|')[0]}`,
										}, [
											XML.create_element_node('matrix', {}, [
												XML.create_element_node('Matrix', make_scale_matrix(resolution), []),
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

	export function create(
		animation: Core.Tool.PopCap.PAM.Manifest.JS_N.Animation,
		resolution: bigint,
	): Array<Core.XML.JS_Element> {
		return animation.image.map((e, i) => (create_one(i, e, resolution)));
	}

	// ------------------------------------------------

	function resize_one(
		document: Core.XML.JS_Node,
		resolution: bigint,
	): void {
		let scale_matrix = XML.find_child_element(
			XML.find_child_element(
				XML.find_child_element(
					XML.find_child_element(
						XML.find_child_element(
							XML.find_child_element(
								XML.find_child_element(
									XML.find_child_element(
										XML.find_child_element(
											XML.find_child_element(
												document.value as Core.XML.JS_Element, 'timeline'
											)[0], 'DOMTimeline'
										)[0], 'layers'
									)[0], 'DOMLayer'
								)[0], 'frames'
							)[0], 'DOMFrame'
						)[0], 'elements'
					)[0], 'DOMBitmapInstance'
				)[0], 'matrix'
			)[0], 'Matrix'
		)[0];
		scale_matrix.attribute = make_scale_matrix(resolution);
		return;
	}

	export function resize(
		document: Array<Core.XML.JS_Node>,
		resolution: bigint,
	): void {
		document.forEach((e, i) => (resize_one(e, resolution)));
		return;
	}

	// ------------------------------------------------

	export function create_fsh(
		directory: string,
		data: Core.Tool.PopCap.PAM.Manifest.JS_N.Animation,
		resolution: bigint = k_standard_resolution,
	): void {
		create(data, resolution).forEach((e, i) => {
			CoreX.XML.write_fs_js(`${directory}/LIBRARY/source/source_${i + 1}.xml`, XML.wrap_element(e));
		});
		return;
	}

	export function resize_fs(
		directory: string,
		resolution: bigint,
	): void {
		let extra = CoreX.JSON.read_fs_js<ExtraInformation>(`${directory}/extra.json`);
		let document = extra.image.map((e, i) => (CoreX.XML.read_fs_js(`${directory}/LIBRARY/source/source_${i + 1}.xml`)));
		resize(document, resolution);
		document.forEach((e, i) => {
			CoreX.XML.write_fs_js(`${directory}/LIBRARY/source/source_${i + 1}.xml`, e);
		});
		return;
	}

	// ------------------------------------------------

}