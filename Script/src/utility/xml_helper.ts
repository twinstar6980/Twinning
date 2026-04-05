namespace Twinning.Script.XmlHelper {

	// #region wrap

	export function wrap_element(
		value: Kernel.Xml.JS_Element,
	): Kernel.Xml.JS_ElementNode {
		return {
			type: 'element',
			value: value,
		};
	}

	export function wrap_text(
		value: Kernel.Xml.JS_Text,
	): Kernel.Xml.JS_TextNode {
		return {
			type: 'text',
			value: value,
		};
	}

	export function wrap_comment(
		value: Kernel.Xml.JS_Comment,
	): Kernel.Xml.JS_CommentNode {
		return {
			type: 'comment',
			value: value,
		};
	}

	// #endregion

	// #region create

	export function create_element(
		name: string,
		attribute: Record<string, string>,
		child: Array<Kernel.Xml.JS_Node>,
	): Kernel.Xml.JS_Element {
		return {name, attribute, child};
	}

	export function create_text(
		value: string,
		cdata: boolean,
	): Kernel.Xml.JS_Text {
		return {value, cdata};
	}

	export function create_comment(
		value: string,
	): Kernel.Xml.JS_Comment {
		return {value};
	}

	// ----------------

	export function create_element_node(
		name: string,
		attribute: Record<string, string>,
		child: Array<Kernel.Xml.JS_Node>,
	): Kernel.Xml.JS_ElementNode {
		return wrap_element(create_element(name, attribute, child));
	}

	export function create_text_node(
		value: string,
		cdata: boolean,
	): Kernel.Xml.JS_TextNode {
		return wrap_text(create_text(value, cdata));
	}

	export function create_comment_node(
		value: string,
	): Kernel.Xml.JS_CommentNode {
		return wrap_comment(create_comment(value));
	}

	// #endregion

	// #region find

	export function find_child_element(
		parent: Kernel.Xml.JS_Element,
		name: string,
	): Array<Kernel.Xml.JS_Element> {
		return parent.child.filter((child) => (child.type === 'element' && child.value.name === name)).map((child) => (child.value as Kernel.Xml.JS_Element));
	}

	export function find_child_element_unique(
		parent: Kernel.Xml.JS_Element,
		name: string,
	): Kernel.Xml.JS_Element {
		let list = find_child_element(parent, name);
		if (list.length !== 1) {
			throw new Error(`xml element is not unique`);
		}
		return list[0];
	}

	export function find_child_element_unique_or_none(
		parent: Kernel.Xml.JS_Element,
		name: string,
	): null | Kernel.Xml.JS_Element {
		let list = find_child_element(parent, name);
		if (list.length > 1) {
			throw new Error(`xml element is not unique or none`);
		}
		return list.length === 0 ? null : list[0];
	}

	// #endregion

	// #region encoding

	export function encode_text<TValue extends Kernel.Xml.JS_Node>(
		value: TValue,
	): string {
		let data = Kernel.String.default();
		Kernel.Tool.Data.Serialization.Xml.Write.process(data, Kernel.Xml.Node.value(value));
		return data.value;
	}

	export function decode_text<TValue extends Kernel.Xml.JS_Node>(
		text: string,
	): TValue {
		let value = Kernel.Xml.Node.default<TValue>();
		Kernel.Tool.Data.Serialization.Xml.Read.process(Kernel.String.value(text), value);
		return value.value;
	}

	// ----------------

	export function encode_file<TValue extends Kernel.Xml.JS_Node>(
		path: StoragePath,
		value: TValue,
	): void {
		let data = Kernel.String.default();
		Kernel.Tool.Data.Serialization.Xml.Write.process(data, Kernel.Xml.Node.value(value));
		let data_byte = Kernel.Miscellaneous.cast_moveable_String_to_ByteArray(data);
		StorageHelper.write_file(path, data_byte);
		return;
	}

	export function decode_file<TValue extends Kernel.Xml.JS_Node>(
		path: StoragePath,
	): TValue {
		let data_byte = StorageHelper.read_file(path);
		let data = Kernel.Miscellaneous.cast_moveable_ByteArray_to_String(data_byte);
		let value = Kernel.Xml.Node.default<TValue>();
		Kernel.Tool.Data.Serialization.Xml.Read.process(data, value);
		return value.value;
	}

	// #endregion

}
