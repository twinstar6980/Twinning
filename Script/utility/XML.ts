namespace TwinStar.XML {

	// ------------------------------------------------

	export function wrap_element(
		value: Core.XML.JS_Element,
	): Core.XML.JS_ElementNode {
		return {
			type: 'element',
			value: value,
		};
	}

	export function wrap_text(
		value: Core.XML.JS_Text,
	): Core.XML.JS_TextNode {
		return {
			type: 'text',
			value: value,
		};
	}

	export function wrap_comment(
		value: Core.XML.JS_Comment,
	): Core.XML.JS_CommentNode {
		return {
			type: 'comment',
			value: value,
		};
	}

	// ------------------------------------------------

	export function create_element(
		name: string,
		attribute: Record<string, string>,
		child: Array<Core.XML.JS_Node>,
	): Core.XML.JS_Element {
		return { name, attribute, child };
	}

	export function create_text(
		value: string,
		cdata: boolean,
	): Core.XML.JS_Text {
		return { value, cdata };
	}

	export function create_comment(
		value: string,
	): Core.XML.JS_Comment {
		return { value };
	}

	// ------------------------------------------------

	export function create_element_node(
		name: string,
		attribute: Record<string, string>,
		child: Array<Core.XML.JS_Node>,
	): Core.XML.JS_ElementNode {
		return wrap_element(create_element(name, attribute, child));
	}

	export function create_text_node(
		value: string,
		cdata: boolean,
	): Core.XML.JS_TextNode {
		return wrap_text(create_text(value, cdata));
	}

	export function create_comment_node(
		value: string,
	): Core.XML.JS_CommentNode {
		return wrap_comment(create_comment(value));
	}

	// ------------------------------------------------

	export function find_child_element(
		parent: Core.XML.JS_Element,
		name: string,
	): Array<Core.XML.JS_Element> {
		return parent.child.filter((e) => (e.type === 'element' && e.value.name === name)).map((e) => (e.value as Core.XML.JS_Element));
	}

	// ------------------------------------------------

}