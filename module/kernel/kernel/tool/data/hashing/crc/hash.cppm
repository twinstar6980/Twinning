module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hashing.crc.hash;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hashing.crc.common;
import twinning.kernel.dependency.cryptopp;

export namespace Twinning::Kernel::Tool::Data::Hashing::Crc {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			ConstantBinaryView const & data,
			Binary &                   value,
			Mode const &               mode,
			Size const &               length
		) -> Void {
			auto transformer = Pointer<Dependency::cryptopp::HashTransformation>{};
			auto _ = make_finalizer(
				[&] {
					if (transformer != k_null_pointer) {
						Memory::free(transformer);
					}
				}
			);
			do {
				if (mode == Mode::Constant::standard() && length == 32_sz) {
					transformer = cast_pointer<Dependency::cryptopp::HashTransformation>(Memory::allocate<Dependency::cryptopp::CRC32>());
					break;
				}
				if (mode == Mode::Constant::castagnoli() && length == 32_sz) {
					transformer = cast_pointer<Dependency::cryptopp::HashTransformation>(Memory::allocate<Dependency::cryptopp::CRC32C>());
					break;
				}
				throw UnsupportedException{};
			}
			while (false);
			auto value_data = Binary{Math::compute_padded_unit_count(length, type_bit_size_of<Byte>())};
			auto source = Dependency::cryptopp::ArraySource{unmake_pointer_unsafe<Dependency::cryptopp::byte>(data.data()), unmake_box<std::size_t>(data.size()), false, nullptr};
			auto sink = Dependency::cryptopp::ArraySink{unmake_pointer_unsafe<Dependency::cryptopp::byte>(value_data.data()), unmake_box<std::size_t>(value_data.size())};
			auto filter = Dependency::cryptopp::HashFilter{transformer.dereference()};
			filter.Attach(new Dependency::cryptopp::Redirector{sink});
			source.Attach(new Dependency::cryptopp::Redirector{filter});
			source.PumpAll();
			assert_test(make_box<Size>(source.TotalBytesRetrievable()) == 0_sz);
			assert_test(make_box<Size>(sink.TotalPutLength()) == value_data.size());
			value = as_moveable(value_data);
			return;
		}

		// ----------------

		inline static auto process(
			ConstantBinaryView const & data,
			Binary &                   value,
			Mode const &               mode,
			Size const &               length
		) -> Void {
			restruct(value);
			return process_whole(data, value, mode, length);
		}

	};

}
