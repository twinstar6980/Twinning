namespace Twinning.Script.XML {

	// ------------------------------------------------

	export function wrap_element(
		value: Kernel.XML.JS_Element,
	): Kernel.XML.JS_ElementNode {
		return {
			type: 'element',
			value: value,
		};
	}

	export function wrap_text(
		value: Kernel.XML.JS_Text,
	): Kernel.XML.JS_TextNode {
		return {
			type: 'text',
			value: value,
		};
	}

	export function wrap_comment(
		value: Kernel.XML.JS_Comment,
	): Kernel.XML.JS_CommentNode {
		return {
			type: 'comment',
			value: value,
		};
	}

	// ------------------------------------------------

	export function create_element(
		name: string,
		attribute: Record<string, string>,
		child: Array<Kernel.XML.JS_Node>,
	): Kernel.XML.JS_Element {
		return { name, attribute, child };
	}

	export function create_text(
		value: string,
		cdata: boolean,
	): Kernel.XML.JS_Text {
		return { value, cdata };
	}

	export function create_comment(
		value: string,
	): Kernel.XML.JS_Comment {
		return { value };
	}

	// ------------------------------------------------

	export function create_element_node(
		name: string,
		attribute: Record<string, string>,
		child: Array<Kernel.XML.JS_Node>,
	): Kernel.XML.JS_ElementNode {
		return wrap_element(create_element(name, attribute, child));
	}

	export function create_text_node(
		value: string,
		cdata: boolean,
	): Kernel.XML.JS_TextNode {
		return wrap_text(create_text(value, cdata));
	}

	export function create_comment_node(
		value: string,
	): Kernel.XML.JS_CommentNode {
		return wrap_comment(create_comment(value));
	}

	// ------------------------------------------------

	export function find_child_element(
		parent: Kernel.XML.JS_Element,
		name: string,
	): Array<Kernel.XML.JS_Element> {
		return parent.child.filter((child) => (child.type === 'element' && child.value.name === name)).map((child) => (child.value as Kernel.XML.JS_Element));
	}

	export function find_child_element_unique(
		parent: Kernel.XML.JS_Element,
		name: string,
	): Kernel.XML.JS_Element {
		let list = find_child_element(parent, name);
		assert_test(list.length === 1, 'xml element is not unique');
		return list[0];
	}

	export function find_child_element_unique_or_none(
		parent: Kernel.XML.JS_Element,
		name: string,
	): null | Kernel.XML.JS_Element {
		let list = find_child_element(parent, name);
		assert_test(list.length <= 1, 'xml element is not unique or none');
		return list.length === 0 ? null : list[0];
	}

	// ------------------------------------------------

}