namespace TwinStar.Script.XML {

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
		return parent.child.filter((e) => (e.type === 'element' && e.value.name === name)).map((e) => (e.value as Kernel.XML.JS_Element));
	}

	// ------------------------------------------------

}