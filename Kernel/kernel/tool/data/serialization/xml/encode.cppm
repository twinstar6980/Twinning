module;

#include "kernel/common.hpp"

// TODO: refactor, use stream style api?

export module twinning.kernel.tool.data.serialization.xml.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.serialization.xml.common;
import twinning.kernel.third.tinyxml2;

export namespace Twinning::Kernel::Tool::Data::Serialization::Xml {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto convert_node(
			Third::tinyxml2::XMLDocument & raw_document,
			Notation::Xml::Node const &    node
		) -> ZPointer<Third::tinyxml2::XMLNode> {
			auto raw_node = ZPointer<Third::tinyxml2::XMLNode>{};
			switch (node.type().value) {
				case Notation::Xml::NodeType::Constant::element().value: {
					auto & node_value = node.get_element();
					raw_node = raw_document.NewElement(rubox<char *>(make_null_terminated_string(node_value.name).begin()));
					auto raw_element = raw_node->ToElement();
					for (auto & attribute : node_value.attribute) {
						raw_element->SetAttribute(rubox<char *>(make_null_terminated_string(attribute.key).begin()), rubox<char *>(make_null_terminated_string(attribute.value).begin()));
					}
					for (auto & child : node_value.child) {
						raw_element->InsertEndChild(convert_node(raw_document, child));
					}
					break;
				}
				case Notation::Xml::NodeType::Constant::text().value: {
					auto & node_value = node.get_text();
					raw_node = raw_document.NewText(rubox<char *>(make_null_terminated_string(node_value.value).begin()));
					auto raw_text = raw_node->ToText();
					raw_text->SetCData(node_value.cdata.value);
					break;
				}
				case Notation::Xml::NodeType::Constant::comment().value: {
					auto & node_value = node.get_comment();
					raw_node = raw_document.NewComment(rubox<char *>(make_null_terminated_string(node_value.value).begin()));
					break;
				}
				default: throw UnreachableException{};
			}
			return raw_node;
		}

		// ----------------

		inline static auto process_whole(
			String &                    data,
			Notation::Xml::Node const & value
		) -> Void {
			auto raw_document = Third::tinyxml2::XMLDocument{};
			raw_document.InsertEndChild(convert_node(raw_document, value));
			auto printer = TinyXml2Printer{};
			raw_document.Print(&printer);
			data = move_printer_buffer_if_can(printer);
			return;
		}

		// ----------------

		inline static auto process(
			String &                    data,
			Notation::Xml::Node const & value
		) -> Void {
			restruct(data);
			return process_whole(data, value);
		}

	};

}
