#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/lzma.hpp"

namespace TwinKleS::Core::Tool::Data::Compress::Lzma {

	inline namespace Common {

		namespace Detail {

			#pragma region parameter

			inline constexpr auto k_props_size = Size{mbw<Size>(ThirdParty::lzma::LZMA_PROPS_SIZE_)};

			#pragma endregion

		}

		using Detail::k_props_size;

	}

	namespace Compress {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & raw,
				OByteStreamView & ripe,
				OByteStreamView & props,
				Size const &      level
			) -> Void {
				assert_condition(Math::between(level, 0_sz, 9_sz));
				auto raw_size = raw.reserve().value;
				auto ripe_size = ripe.reserve().value;
				auto props_size = props.reserve().value;
				auto state = ThirdParty::lzma::LzmaCompress(
					cast_pointer<ThirdParty::lzma::Byte>(ripe.current_pointer()).value,
					&ripe_size,
					cast_pointer<ThirdParty::lzma::Byte>(raw.current_pointer()).value,
					raw_size,
					cast_pointer<ThirdParty::lzma::Byte>(props.current_pointer()).value,
					&props_size,
					static_cast<int>(level.value),
					0,
					-1,
					-1,
					-1,
					-1,
					-1
				);
				assert_condition(state == ThirdParty::lzma::SZ_OK_);
				raw.forward(mbw<Size>(raw_size));
				ripe.forward(mbw<Size>(ripe_size));
				props.forward(mbw<Size>(props_size));
				assert_condition(raw.full());
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Uncompress {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & ripe,
				OByteStreamView & raw,
				IByteStreamView & props
			) -> Void {
				auto ripe_size = ripe.reserve().value;
				auto raw_size = raw.reserve().value;
				auto props_size = props.reserve().value;
				auto state = ThirdParty::lzma::LzmaUncompress(
					cast_pointer<ThirdParty::lzma::Byte>(raw.current_pointer()).value,
					&raw_size,
					cast_pointer<ThirdParty::lzma::Byte>(ripe.current_pointer()).value,
					&ripe_size,
					cast_pointer<ThirdParty::lzma::Byte>(props.current_pointer()).value,
					props_size
				);
				assert_condition(state == ThirdParty::lzma::SZ_OK_);
				ripe.forward(mbw<Size>(ripe_size));
				raw.forward(mbw<Size>(raw_size));
				props.forward(mbw<Size>(props_size));
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
