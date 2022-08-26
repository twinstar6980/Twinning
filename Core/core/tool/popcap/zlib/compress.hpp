#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/zlib/structure.hpp"

namespace TwinKleS::Core::Tool::PopCap::ZLib {

	inline namespace CommonOfCompress {

		namespace Detail {

		}

	}

	namespace Compress {

		namespace Detail {

			#pragma region using

			using namespace CommonOfCompress::Detail;

			#pragma endregion

			#pragma region compute size

			inline auto compute_size_bound (
				Size const &    raw_size,
				Size const &    window_bits,
				Size const &    memory_level,
				Boolean const & variant_64
			) -> Size {
				auto size = k_none_size;
				size += bs_static_size<Structure::MagicIdentifier>();
				if (variant_64) {
					size += bs_static_size<IntegerU32>();
				}
				if (!variant_64) {
					size += bs_static_size<IntegerU32>();
				} else {
					size += bs_static_size<IntegerU64>();
				}
				size += Data::Compress::Deflate::Compress::compute_size_bound(raw_size, window_bits, memory_level, Data::Compress::Deflate::Wrapper::zlib);
				return size;
			}

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView &                         raw,
				OByteStreamView &                         ripe,
				Size const &                              level,
				Size const &                              window_bits,
				Size const &                              memory_level,
				Data::Compress::Deflate::Strategy const & strategy,
				Boolean const &                           variant_64
			) -> Void {
				ripe.write(Structure::k_magic_identifier);
				if (variant_64) {
					ripe.forward(bs_static_size<IntegerU32>());
				}
				if (!variant_64) {
					ripe.write(cbw<IntegerU32>(raw.reserve()));
				} else {
					ripe.write(cbw<IntegerU64>(raw.reserve()));
				}
				Data::Compress::Deflate::Compress::process(raw, ripe, level, window_bits, memory_level, strategy, Data::Compress::Deflate::Wrapper::zlib);
				return;
			}

			#pragma endregion

		}

		using Detail::compute_size_bound;

		using Detail::process;

	}

	namespace Uncompress {

		namespace Detail {

			#pragma region using

			using namespace CommonOfCompress::Detail;

			#pragma endregion

			#pragma region compute size

			inline auto compute_size (
				CByteListView const & ripe,
				Boolean const &       variant_64
			) -> Size {
				auto ripe_stream = IByteStreamView{ripe};
				assert_condition(ripe_stream.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				if (variant_64) {
					ripe_stream.forward(bs_static_size<IntegerU32>());
				}
				auto size = Size{};
				if (!variant_64) {
					size = cbw<Size>(ripe_stream.read_of<IntegerU32>());
				} else {
					size = cbw<Size>(ripe_stream.read_of<IntegerU64>());
				}
				return size;
			}

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & ripe,
				OByteStreamView & raw,
				Size const &      window_bits,
				Boolean const &   variant_64
			) -> Void {
				assert_condition(ripe.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				if (variant_64) {
					ripe.forward(bs_static_size<IntegerU32>());
				}
				auto size = Size{};
				if (!variant_64) {
					size = cbw<Size>(ripe.read_of<IntegerU32>());
				} else {
					size = cbw<Size>(ripe.read_of<IntegerU64>());
				}
				auto raw_begin = raw.position();
				Data::Compress::Deflate::Uncompress::process(ripe, raw, window_bits, Data::Compress::Deflate::Wrapper::zlib);
				assert_condition(raw.position() - raw_begin == size);
				return;
			}

			#pragma endregion

		}

		using Detail::compute_size;

		using Detail::process;

	}

}
