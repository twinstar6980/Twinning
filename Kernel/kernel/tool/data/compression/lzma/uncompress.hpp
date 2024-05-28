#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/compression/lzma/common.hpp"
#include "kernel/third/lzma.hpp"

namespace Twinning::Kernel::Tool::Data::Compression::Lzma {

	struct Uncompress :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw
		) -> Void {
			auto property = ripe.forward_view(mbw<Size>(Third::lzma::LZMA_PROPS_SIZE_));
			auto raw_size_recorded = ripe.read_of<IntegerU64>();
			assert_test(raw_size_recorded != ~0_iu64);
			assert_test(raw.reserve() >= cbw<Size>(raw_size_recorded));
			auto ripe_size = ripe.reserve().value;
			auto raw_size = cbw<Size>(raw_size_recorded).value;
			auto property_size = property.size().value;
			auto state = Third::lzma::LzmaUncompress(
				cast_pointer<Third::lzma::Byte>(raw.current_pointer()).value,
				&raw_size,
				cast_pointer<Third::lzma::Byte>(ripe.current_pointer()).value,
				&ripe_size,
				cast_pointer<Third::lzma::Byte>(property.begin()).value,
				property_size
			);
			assert_test(state == Third::lzma::SZ_OK_);
			assert_test(raw_size == cbw<Size>(raw_size_recorded).value);
			ripe.forward(mbw<Size>(ripe_size));
			raw.forward(mbw<Size>(raw_size));
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView & ripe_,
			OByteStreamView & raw_
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw);
		}

	};

}
