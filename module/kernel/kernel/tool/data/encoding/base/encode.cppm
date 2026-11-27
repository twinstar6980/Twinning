module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encoding.base.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encoding.base.common;
import twinning.kernel.dependency.cryptopp;

export namespace Twinning::Kernel::Tool::Data::Encoding::Base {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Mode const &             mode
		) -> Void {
			auto ripe_size = Size{};
			estimate_whole(raw.reserve(), ripe_size, mode);
			assert_test(ripe.reserve() >= ripe_size);
			auto parameter = Dependency::cryptopp::AlgorithmParameters{};
			parameter(Dependency::cryptopp::Name::Pad(), true);
			parameter(Dependency::cryptopp::Name::PaddingByte(), Dependency::cryptopp::byte{'='});
			do {
				if (mode == Mode::Constant::v16_std()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{4});
					parameter(Dependency::cryptopp::Name::EncodingLookupArray(), unmake_pointer_unsafe<Dependency::cryptopp::byte>(k_encode_table_v16_std.data()));
					break;
				}
				if (mode == Mode::Constant::v32_std()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{5});
					parameter(Dependency::cryptopp::Name::EncodingLookupArray(), unmake_pointer_unsafe<Dependency::cryptopp::byte>(k_encode_table_v32_std.data()));
					break;
				}
				if (mode == Mode::Constant::v32_hex()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{5});
					parameter(Dependency::cryptopp::Name::EncodingLookupArray(), unmake_pointer_unsafe<Dependency::cryptopp::byte>(k_encode_table_v32_hex.data()));
					break;
				}
				if (mode == Mode::Constant::v64_std()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{6});
					parameter(Dependency::cryptopp::Name::EncodingLookupArray(), unmake_pointer_unsafe<Dependency::cryptopp::byte>(k_encode_table_v64_std.data()));
					break;
				}
				if (mode == Mode::Constant::v64_url()) {
					parameter(Dependency::cryptopp::Name::Log2Base(), int{6});
					parameter(Dependency::cryptopp::Name::EncodingLookupArray(), unmake_pointer_unsafe<Dependency::cryptopp::byte>(k_encode_table_v64_url.data()));
					break;
				}
				throw UnsupportedException{};
			}
			while (false);
			auto source = Dependency::cryptopp::ArraySource{unmake_pointer_unsafe<Dependency::cryptopp::byte>(raw.current()), unmake_box<std::size_t>(raw.reserve()), false, nullptr};
			auto sink = Dependency::cryptopp::ArraySink{unmake_pointer_unsafe<Dependency::cryptopp::byte>(ripe.current()), unmake_box<std::size_t>(ripe.reserve())};
			auto transformer = Dependency::cryptopp::BaseN_Encoder{};
			transformer.IsolatedInitialize(parameter);
			transformer.Attach(new Dependency::cryptopp::Redirector{sink});
			source.Attach(new Dependency::cryptopp::Redirector{transformer});
			source.PumpAll();
			assert_test(make_box<Size>(source.TotalBytesRetrievable()) == 0_sz);
			assert_test(make_box<Size>(sink.TotalPutLength()) == ripe_size);
			raw.forward(raw.reserve());
			ripe.forward(make_box<Size>(sink.TotalPutLength()));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size,
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
			ripe_size = Math::compute_padded_unit_count(raw_size, raw_block_size) * ripe_block_size;
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Mode const &             mode
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, mode);
		}

		inline static auto estimate(
			Size const & raw_size,
			Size &       ripe_size,
			Mode const & mode
		) -> Void {
			restruct(ripe_size);
			return estimate_whole(raw_size, ripe_size, mode);
		}

	};

}
