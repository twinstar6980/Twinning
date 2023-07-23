#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/third/tinyxml2.hpp"

namespace TwinStar::Kernel::Tool::Data::Serialization::XML {

	using Kernel::XML::NodeType;

	using Kernel::XML::Node;

	// ----------------

	struct Common {

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
		struct TinyXML2DynArrayImitator {

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

			bool                                       _elementJustOpened;
			TinyXML2DynArrayImitator<char const *, 10> _stack;
			bool                                       _firstElement;
			FILE *                                     _fp;
			int                                        _depth;
			int                                        _textDepth;
			bool                                       _processEntities;
			bool                                       _compactMode;
			bool                                       _entityFlag[ENTITY_RANGE];
			bool                                       _restrictedEntityFlag[ENTITY_RANGE];
			TinyXML2DynArrayImitator<char, 20>         _buffer;

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
			assert_test(buffer_size > 0_sz);
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

	};

}
