namespace Twinning.Script.Support.Popcap.Animation.Convert.Flash.SourceManager {

	// #region utility

	function make_scale_matrix(
		resolution: bigint,
	): {
		a: string;
		d: string;
	} {
		let scale = Number(k_standard_resolution) / Number(resolution);
		return {
			a: `${scale.toFixed(6)}`,
			d: `${scale.toFixed(6)}`,
		};
	}

	// ----------------

	function create_one(
		index: number,
		image: Kernel.Tool.Popcap.Animation.Definition.JS_N.Image,
		resolution: bigint,
	): Kernel.Xml.JS_Element {
		return Xml.create_element('DOMSymbolItem', {
			...k_xmlns_attribute,
			name: `source/source_${index + 1}`,
			symbolType: 'graphic',
		}, [
			Xml.create_element_node('timeline', {}, [
				Xml.create_element_node('DOMTimeline', {
					name: `source_${index + 1}`,
				}, [
					Xml.create_element_node('layers', {}, [
						Xml.create_element_node('DOMLayer', {}, [
							Xml.create_element_node('frames', {}, [
								Xml.create_element_node('DOMFrame', {
									index: `0`,
								}, [
									Xml.create_element_node('elements', {}, [
										Xml.create_element_node('DOMBitmapInstance', {
											libraryItemName: `media/${image.name.split('|')[0]}`,
										}, [
											Xml.create_element_node('matrix', {}, [
												Xml.create_element_node('Matrix', make_scale_matrix(resolution), []),
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
		animation: Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation,
		resolution: bigint,
	): Array<Kernel.Xml.JS_Element> {
		return animation.image.map((value, index) => (create_one(index, value, resolution)));
	}

	// ----------------

	function resize_one(
		document: Kernel.Xml.JS_Node,
		resolution: bigint,
	): void {
		let scale_matrix = Xml.find_child_element(
			Xml.find_child_element(
				Xml.find_child_element(
					Xml.find_child_element(
						Xml.find_child_element(
							Xml.find_child_element(
								Xml.find_child_element(
									Xml.find_child_element(
										Xml.find_child_element(
											Xml.find_child_element(
												document.value as Kernel.Xml.JS_Element, 'timeline'
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
		document: Array<Kernel.Xml.JS_Node>,
		resolution: bigint,
	): void {
		document.forEach((value, index) => (resize_one(value, resolution)));
		return;
	}

	// ----------------

	export function create_fsh(
		directory: string,
		data: Kernel.Tool.Popcap.Animation.Definition.JS_N.Animation,
		resolution: null | bigint,
	): void {
		resolution = CheckHelper.not_null_or(resolution, k_standard_resolution);
		create(data, resolution).forEach((value, index) => {
			KernelX.Xml.write_fs_js(`${directory}/LIBRARY/source/source_${index + 1}.xml`, Xml.wrap_element(value));
		});
		return;
	}

	export function resize_fs(
		directory: string,
		resolution: bigint,
	): void {
		let extra = KernelX.Json.read_fs_js(`${directory}/extra.json`) as ExtraInformation;
		let document = extra.image.map((value, index) => (KernelX.Xml.read_fs_js(`${directory}/LIBRARY/source/source_${index + 1}.xml`)));
		resize(document, resolution);
		document.forEach((value, index) => {
			KernelX.Xml.write_fs_js(`${directory}/LIBRARY/source/source_${index + 1}.xml`, value);
		});
		return;
	}

	// #endregion

}
