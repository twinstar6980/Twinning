#pragma once

// TODO : refactor, use stream style api ?

#include "core/utility/utility.hpp"
#include "core/third/tinyxml2.hpp"

namespace TwinStar::Core::Tool::Data::Serialization::XML {

	using Core::XML::NodeType;

	using Core::XML::Node;

	// ----------------

	struct WriteCommon {

	protected:

		#pragma region tinyxml2 utility

		#if defined M_compiler_msvc
		#pragma warning(push)
		#pragma warning(disable:4625)
		#pragma warning(disable:4626)
		#pragma warning(disable:5026)
		#pragma warning(disable:5027)
		#endif

		class TinyXML2Printer :
			public Third::tinyxml2::XMLPrinter {

		public:

			virtual auto PrintSpace (
				int depth
			) -> void override {
				for (auto index = 0; index < depth; ++index) {
					thiz.Putc('\t');
				}
			}

		};

		#if defined M_compiler_msvc
		#pragma warning(pop)
		#endif

		template <class T, int INITIAL_SIZE>
		struct TinyXML2DynArray {

			T * _mem;
			T   _pool[INITIAL_SIZE];
			int _allocated;
			int _size;

		};

		struct TinyXML2PrinterImitator {

			enum {
				ENTITY_RANGE = 64,
				BUF_SIZE     = 200
			};

			// ----------------

			virtual ~TinyXML2PrinterImitator () = 0;

			// ----------------

			bool                               _elementJustOpened;
			TinyXML2DynArray<char const *, 10> _stack;
			bool                               _firstElement;
			FILE *                             _fp;
			int                                _depth;
			int                                _textDepth;
			bool                               _processEntities;
			bool                               _compactMode;
			bool                               _entityFlag[ENTITY_RANGE];
			bool                               _restrictedEntityFlag[ENTITY_RANGE];
			TinyXML2DynArray<char, 20>         _buffer;

		};

		// NOTE : maybe failed if api changed
		static_assert(sizeof(TinyXML2PrinterImitator) == sizeof(Third::tinyxml2::XMLPrinter));

		static auto move_printer_buffer_if_can (
			Third::tinyxml2::XMLPrinter & printer
		) -> String {
			auto & printer_imitator = self_cast<TinyXML2PrinterImitator>(printer);
			auto   buffer_data = cast_pointer<Character>(make_pointer(printer_imitator._buffer._mem));
			auto   buffer_size = mbw<Size>(printer_imitator._buffer._size);
			auto   buffer_capacity = mbw<Size>(printer_imitator._buffer._allocated);
			auto   result = String{};
			assert_condition(buffer_size > 0_sz);
			if (printer_imitator._buffer._mem == printer_imitator._buffer._pool) {
				result.assign(CStringView{buffer_data, buffer_size - 1_sz});
			} else {
				printer_imitator._buffer._mem = printer_imitator._buffer._pool;
				printer_imitator._buffer._size = 0;
				printer_imitator._buffer._allocated = 20;
				result.bind(buffer_data, buffer_size - 1_sz, buffer_capacity);
			}
			return result;
		}

		#pragma endregion

	};

	struct Write :
		WriteCommon {

	protected:

		static auto process_node (
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
						raw_element->InsertEndChild(process_node(raw_document, child));
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

		static auto process_whole (
			String &     data,
			Node const & value
		) -> Void {
			auto raw_document = Third::tinyxml2::XMLDocument{};
			raw_document.InsertEndChild(process_node(raw_document, value));
			auto printer = TinyXML2Printer{};
			raw_document.Print(&printer);
			data = move_printer_buffer_if_can(printer);
			return;
		}

	public:

		static auto do_process_whole (
			String &     data,
			Node const & value
		) -> Void {
			restruct(data);
			return process_whole(data, value);
		}

	};

	struct Read :
		WriteCommon {

	protected:

		static auto process_node (
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
						process_node(raw_child, node_value.child.last());
					}
				}
			} else if (auto raw_text = raw_node->ToText()) {
				node.set_text();
				auto & node_value = node.get_text();
				node_value.value = make_string_view(raw_text->Value());
				node_value.cdata = mbw<Boolean>(raw_text->CData());
			} else if (auto raw_comment = raw_node->ToComment()) {
				node.set_comment();
				auto & node_value = node.get_comment();
				node_value.value = make_string_view(raw_comment->Value());
			} else {
				assert_failed(R"(/* node is valid */)");
			}
		}

		static auto process_document (
			Third::tinyxml2::XMLDocument const & raw_document,
			Node &                               node
		) -> Void {
			auto current_child = raw_document.FirstChild();
			assert_condition(current_child);
			auto declaration = current_child->ToDeclaration();
			if (declaration) {
				current_child = current_child->NextSibling();
			}
			assert_condition(current_child);
			auto root_element = current_child->ToElement();
			assert_condition(root_element);
			current_child = current_child->NextSibling();
			assert_condition(!current_child);
			process_node(root_element, node);
			return;
		}

		// ----------------

		static auto process_whole (
			String const & data,
			Node &         value
		) -> Void {
			auto raw_document = Third::tinyxml2::XMLDocument{};
			raw_document.Parse(cast_pointer<ZCharacter>(data.begin()).value, data.size().value);
			process_document(raw_document, value);
			return;
		}

	public:

		static auto do_process_whole (
			String const & data,
			Node &         value
		) -> Void {
			restruct(value);
			return process_whole(data, value);
		}

	};

}
