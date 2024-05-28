namespace Twinning.Script.Support.PopCap.Animation.Convert.Flash.SourceManager {

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
		image: Kernel.Tool.PopCap.Animation.Definition.JS_N.Image,
		resolution: bigint,
	): Kernel.XML.JS_Element {
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

	function create(
		animation: Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation,
		resolution: bigint,
	): Array<Kernel.XML.JS_Element> {
		return animation.image.map((value, index) => (create_one(index, value, resolution)));
	}

	// ------------------------------------------------

	function resize_one(
		document: Kernel.XML.JS_Node,
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
												document.value as Kernel.XML.JS_Element, 'timeline'
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

	function resize(
		document: Array<Kernel.XML.JS_Node>,
		resolution: bigint,
	): void {
		document.forEach((value, index) => (resize_one(value, resolution)));
		return;
	}

	// ------------------------------------------------

	export function create_fsh(
		directory: string,
		data: Kernel.Tool.PopCap.Animation.Definition.JS_N.Animation,
		resolution: bigint = k_standard_resolution,
	): void {
		create(data, resolution).forEach((value, index) => {
			KernelX.XML.write_fs_js(`${directory}/LIBRARY/source/source_${index + 1}.xml`, XML.wrap_element(value));
		});
		return;
	}

	export function resize_fs(
		directory: string,
		resolution: bigint,
	): void {
		let extra = KernelX.JSON.read_fs_js<ExtraInformation>(`${directory}/extra.json`);
		let document = extra.image.map((value, index) => (KernelX.XML.read_fs_js(`${directory}/LIBRARY/source/source_${index + 1}.xml`)));
		resize(document, resolution);
		document.forEach((value, index) => {
			KernelX.XML.write_fs_js(`${directory}/LIBRARY/source/source_${index + 1}.xml`, value);
		});
		return;
	}

	// ------------------------------------------------

}