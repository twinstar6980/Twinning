module;

#include "kernel/common.hpp"

// TODO : refactor, use stream style api ?

export module twinning.kernel.tool.data.serialization.xml.write;
import twinning.kernel.utility;
import twinning.kernel.tool.data.serialization.xml.common;
import twinning.kernel.third.tinyxml2;

export namespace Twinning::Kernel::Tool::Data::Serialization::XML {

	struct Write :
		Common {

		using Common = Common;

		// ----------------

		inline static auto convert_node (
			Third::tinyxml2::XMLDocument & raw_document,
			Node const &                   node
		) -> ZPointer<Third::tinyxml2::XMLNode> {
			auto raw_node = ZPointer<Third::tinyxml2::XMLNode>{};
			switch (node.type().value) {
				case NodeType::Constant::element().value : {
					auto & node_value = node.get_element();
					raw_node = raw_document.NewElement(cast_pointer<char>(make_null_terminated_string(node_value.name).begin()).value);
					auto raw_element = raw_node->ToElement();
					for (auto & attribute : node_value.attribute) {
						raw_element->SetAttribute(cast_pointer<char>(make_null_terminated_string(attribute.key).begin()).value, cast_pointer<char>(make_null_terminated_string(attribute.value).begin()).value);
					}
					for (auto & child : node_value.child) {
						raw_element->InsertEndChild(convert_node(raw_document, child));
					}
					break;
				}
				case NodeType::Constant::text().value : {
					auto & node_value = node.get_text();
					raw_node = raw_document.NewText(cast_pointer<char>(make_null_terminated_string(node_value.value).begin()).value);
					auto raw_text = raw_node->ToText();
					raw_text->SetCData(node_value.cdata.value);
					break;
				}
				case NodeType::Constant::comment().value : {
					auto & node_value = node.get_comment();
					raw_node = raw_document.NewComment(cast_pointer<char>(make_null_terminated_string(node_value.value).begin()).value);
					break;
				}
			}
			return raw_node;
		}

		// ----------------

		inline static auto process_whole (
			String &     data,
			Node const & value
		) -> Void {
			auto raw_document = Third::tinyxml2::XMLDocument{};
			raw_document.InsertEndChild(convert_node(raw_document, value));
			auto printer = TinyXML2Printer{};
			raw_document.Print(&printer);
			data = move_printer_buffer_if_can(printer);
			return;
		}

		// ----------------

		inline static auto process (
			String &     data,
			Node const & value
		) -> Void {
			restruct(data);
			return process_whole(data, value);
		}

	};

}
