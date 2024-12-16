module;

#include "kernel/common.hpp"

// TODO : refactor, use stream style api ?

export module twinning.kernel.tool.data.serialization.xml.read;
import twinning.kernel.utility;
import twinning.kernel.tool.data.serialization.xml.common;
import twinning.kernel.third.tinyxml2;

export namespace Twinning::Kernel::Tool::Data::Serialization::XML {

	struct Read :
		Common {

		using Common = Common;

		// ----------------

		inline static auto convert_node (
			ZPointer<Third::tinyxml2::XMLNode const> const & raw_node,
			Node &                                           node
		) -> Void {
			if (auto raw_element = raw_node->ToElement()) {
				node.set_element();
				auto & node_value = node.get_element();
				node_value.name = make_string_view(raw_element->Name());
				{
					auto attribute_count = k_none_size;
					for (auto raw_attribute = raw_element->FirstAttribute(); raw_attribute; raw_attribute = raw_attribute->Next()) {
						++attribute_count;
					}
					node_value.attribute.allocate(attribute_count);
					for (auto raw_attribute = raw_element->FirstAttribute(); raw_attribute; raw_attribute = raw_attribute->Next()) {
						node_value.attribute.append(make_string(raw_attribute->Name()), make_string(raw_attribute->Value()));
					}
				}
				{
					auto child_count = k_none_size;
					for (auto raw_child = raw_element->FirstChild(); raw_child; raw_child = raw_child->NextSibling()) {
						++child_count;
					}
					node_value.child.allocate(child_count);
					for (auto raw_child = raw_element->FirstChild(); raw_child; raw_child = raw_child->NextSibling()) {
						node_value.child.append();
						convert_node(raw_child, node_value.child.last());
					}
				}
			}
			else if (auto raw_text = raw_node->ToText()) {
				node.set_text();
				auto & node_value = node.get_text();
				node_value.value = make_string_view(raw_text->Value());
				node_value.cdata = mbox<Boolean>(raw_text->CData());
			}
			else if (auto raw_comment = raw_node->ToComment()) {
				node.set_comment();
				auto & node_value = node.get_comment();
				node_value.value = make_string_view(raw_comment->Value());
			}
			else {
				assert_fail(R"(/* node is valid */)");
			}
			return;
		}

		// ----------------

		inline static auto process_whole (
			String const & data,
			Node &         value
		) -> Void {
			auto raw_document = Third::tinyxml2::XMLDocument{};
			raw_document.Parse(cast_pointer<ZCharacter>(data.begin()).value, data.size().value);
			auto current_child = raw_document.FirstChild();
			assert_test(current_child != nullptr);
			auto declaration = current_child->ToDeclaration();
			if (declaration) {
				current_child = current_child->NextSibling();
			}
			assert_test(current_child != nullptr);
			auto root_element = current_child->ToElement();
			assert_test(root_element);
			current_child = current_child->NextSibling();
			assert_test(current_child == nullptr);
			convert_node(root_element, value);
			return;
		}

		// ----------------

		inline static auto process (
			String const & data,
			Node &         value
		) -> Void {
			restruct(value);
			return process_whole(data, value);
		}

	};

}
