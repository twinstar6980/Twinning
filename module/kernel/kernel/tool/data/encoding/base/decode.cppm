module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encoding.base.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encoding.base.common;
import twinning.kernel.dependency.cryptopp;

export namespace Twinning::Kernel::Tool::Data::Encoding::Base {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static constexpr auto k_decode_table_v16_std = StaticArray<IntegerS32, 256_sz>{generate_reverse_table(k_encode_table_v16_std, k_true)};

		inline static constexpr auto k_decode_table_v32_std = StaticArray<IntegerS32, 256_sz>{generate_reverse_table(k_encode_table_v32_std, k_true)};

		inline static constexpr auto k_decode_table_v32_hex = StaticArray<IntegerS32, 256_sz>{generate_reverse_table(k_encode_table_v32_hex, k_true)};

		inline static constexpr auto k_decode_table_v64_std = StaticArray<IntegerS32, 256_sz>{generate_reverse_table(k_encode_table_v64_std, k_false)};

		inline static constexpr auto k_decode_table_v64_url = StaticArray<IntegerS32, 256_sz>{generate_reverse_table(k_encode_table_v64_url, k_false)};

		// ----------------

		inline static auto process_whole(
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Mode const &             mode
		) -> Void {
			auto raw_size = Size{};
			estimate_whole(raw_size, ripe.reserve(), mode);
			assert_test(raw.reserve() >= raw_size);
			auto parameter = Dependency::cryptopp::AlgorithmParameters{};
			do {
				if (mode == Mode::Constant::v16_std()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{4});
					parameter(Dependency::cryptopp::Name::DecodingLookupArray(), unmake_pointer_unsafe<int>(k_decode_table_v16_std.data()));
					break;
				}
				if (mode == Mode::Constant::v32_std()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{5});
					parameter(Dependency::cryptopp::Name::DecodingLookupArray(), unmake_pointer_unsafe<int>(k_decode_table_v32_std.data()));
					break;
				}
				if (mode == Mode::Constant::v32_hex()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{5});
					parameter(Dependency::cryptopp::Name::DecodingLookupArray(), unmake_pointer_unsafe<int>(k_decode_table_v32_hex.data()));
					break;
				}
				if (mode == Mode::Constant::v64_std()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{6});
					parameter(Dependency::cryptopp::Name::DecodingLookupArray(), unmake_pointer_unsafe<int>(k_decode_table_v64_std.data()));
					break;
				}
				if (mode == Mode::Constant::v64_url()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{6});
					parameter(Dependency::cryptopp::Name::DecodingLookupArray(), unmake_pointer_unsafe<int>(k_decode_table_v64_url.data()));
					break;
				}
				throw UnsupportedException{};
			}
			while (false);
			auto source = Dependency::cryptopp::ArraySource{unmake_pointer_unsafe<Dependency::cryptopp::byte>(ripe.current()), unmake_box<std::size_t>(ripe.reserve()), false, nullptr};
			auto sink = Dependency::cryptopp::ArraySink{unmake_pointer_unsafe<Dependency::cryptopp::byte>(raw.current()), unmake_box<std::size_t>(raw.reserve())};
			auto transformer = Dependency::cryptopp::BaseN_Decoder{};
			transformer.IsolatedInitialize(parameter);
			transformer.Attach(new Dependency::cryptopp::Redirector{sink});
			source.Attach(new Dependency::cryptopp::Redirector{transformer});
			source.PumpAll();
			assert_test(make_box<Size>(source.TotalBytesRetrievable()) == 0_sz);
			assert_test(make_box<Size>(sink.TotalPutLength()) <= raw_size);
			ripe.forward(ripe.reserve());
			raw.forward(make_box<Size>(sink.TotalPutLength()));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size &       raw_size,
			Size const & ripe_size,
			Mode const & mode
		) -> Void {
			auto raw_block_size = Size{};
			auto ripe_block_size = Size{};
			do {
				if (mode == Mode::Constant::v16_std()) {
					raw_block_size = 1_sz;
					ripe_block_size = 2_sz;
					break;
				}
				if (mode == Mode::Constant::v32_std()) {
					raw_block_size = 5_sz;
					ripe_block_size = 8_sz;
					break;
				}
				if (mode == Mode::Constant::v32_hex()) {
					raw_block_size = 5_sz;
					ripe_block_size = 8_sz;
					break;
				}
				if (mode == Mode::Constant::v64_std()) {
					raw_block_size = 3_sz;
					ripe_block_size = 4_sz;
					break;
				}
				if (mode == Mode::Constant::v64_url()) {
					raw_block_size = 3_sz;
					ripe_block_size = 4_sz;
					break;
				}
				throw UnsupportedException{};
			}
			while (false);
			assert_test(Math::is_padded_size(ripe_size, ripe_block_size));
			raw_size = Math::compute_padded_unit_count(ripe_size, ripe_block_size) * raw_block_size;
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView & raw_,
			InputBinaryStreamView &  ripe_,
			Mode const &             mode
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, mode);
		}

		inline static auto estimate(
			Size &       raw_size,
			Size const & ripe_size,
			Mode const & mode
		) -> Void {
			restruct(raw_size);
			return estimate_whole(raw_size, ripe_size, mode);
		}

	};

}
