/** XML */
namespace TwinStar.XML {

	// ------------------------------------------------

	/**
	 * 包装元素为结点
	 * @param value 元素
	 * @returns 结点
	 */
	export function wrap_element(
		value: Core.XML.JS_Element,
	): Core.XML.JS_ElementNode {
		return {
			type: 'element',
			value: value,
		};
	}

	/**
	 * 包装文本为结点
	 * @param value 文本
	 * @returns 结点
	 */
	export function wrap_text(
		value: Core.XML.JS_Text,
	): Core.XML.JS_TextNode {
		return {
			type: 'text',
			value: value,
		};
	}

	/**
	 * 包装注释为结点
	 * @param value 注释
	 * @returns 结点
	 */
	export function wrap_comment(
		value: Core.XML.JS_Comment,
	): Core.XML.JS_CommentNode {
		return {
			type: 'comment',
			value: value,
		};
	}

	// ------------------------------------------------

	/**
	 * 创建元素
	 * @param name 名称
	 * @param attribute 属性
	 * @param child 子结点
	 * @returns 元素
	 */
	export function create_element(
		name: string,
		attribute: Record<string, string>,
		child: Array<Core.XML.JS_Node>,
	): Core.XML.JS_Element {
		return { name, attribute, child };
	}

	/**
	 * 创建文本
	 * @param value 文本内容
	 * @param cdata 是否为CDATA
	 * @returns 文本
	 */
	export function create_text(
		value: string,
		cdata: boolean,
	): Core.XML.JS_Text {
		return { value, cdata };
	}

	/**
	 * 创建注释
	 * @param value 注释内容
	 * @returns 注释
	 */
	export function create_comment(
		value: string,
	): Core.XML.JS_Comment {
		return { value };
	}

	// ------------------------------------------------

	/**
	 * 创建元素结点
	 * @param name 名称
	 * @param attribute 属性
	 * @param child 子结点
	 * @returns 元素结点
	 */
	export function create_element_node(
		name: string,
		attribute: Record<string, string>,
		child: Array<Core.XML.JS_Node>,
	): Core.XML.JS_ElementNode {
		return wrap_element(create_element(name, attribute, child));
	}

	/**
	 * 创建文本结点
	 * @param value 文本内容
	 * @param cdata 是否为CDATA
	 * @returns 文本结点
	 */
	export function create_text_node(
		value: string,
		cdata: boolean,
	): Core.XML.JS_TextNode {
		return wrap_text(create_text(value, cdata));
	}

	/**
	 * 创建注释结点
	 * @param value 注释内容
	 * @returns 注释结点
	 */
	export function create_comment_node(
		value: string,
	): Core.XML.JS_CommentNode {
		return wrap_comment(create_comment(value));
	}

	// ------------------------------------------------

	/**
	 * 根据元素名查找子元素
	 * @param parent 父元素
	 * @param name 子元素名
	 * @returns 查找到的子元素
	 */
	export function find_child_element(
		parent: Core.XML.JS_Element,
		name: string,
	): Array<Core.XML.JS_Element> {
		return parent.child.filter((e) => (e.type === 'element' && e.value.name === name)).map((e) => (e.value as Core.XML.JS_Element));
	}

	// ------------------------------------------------

}