#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/compression/lzma/common.hpp"
#include "kernel/third/lzma.hpp"

namespace Twinning::Kernel::Tool::Data::Compression::Lzma {

	struct Compress :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
			IByteStreamView & raw,
			OByteStreamView & ripe,
			Size const &      level
		) -> Void {
			assert_test(Math::between(level, 0_sz, 9_sz));
			auto property = ripe.forward_view(mbw<Size>(Third::lzma::LZMA_PROPS_SIZE_));
			ripe.write(cbw<IntegerU64>(raw.reserve()));
			auto raw_size = raw.reserve().value;
			auto ripe_size = ripe.reserve().value;
			auto property_size = property.size().value;
			auto state = Third::lzma::LzmaCompress(
				cast_pointer<Third::lzma::Byte>(ripe.current_pointer()).value,
				&ripe_size,
				cast_pointer<Third::lzma::Byte>(raw.current_pointer()).value,
				raw_size,
				cast_pointer<Third::lzma::Byte>(property.begin()).value,
				&property_size,
				static_cast<int>(level.value),
				0,
				-1,
				-1,
				-1,
				-1,
				-1
			);
			assert_test(state == Third::lzma::SZ_OK_);
			assert_test(property_size == property.size().value);
			raw.forward(mbw<Size>(raw_size));
			ripe.forward(mbw<Size>(ripe_size));
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView & raw_,
			OByteStreamView & ripe_,
			Size const &      level
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, level);
		}

	};

}
