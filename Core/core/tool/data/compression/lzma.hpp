#pragma once

#include "core/utility/utility.hpp"
#include "core/third/lzma.hpp"

namespace TwinStar::Core::Tool::Data::Compression::Lzma {

	struct CompressCommon {

	protected:

	};

	struct Compress :
		CompressCommon {

	protected:

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

	public:

		static auto do_process_whole (
			IByteStreamView & raw_,
			OByteStreamView & ripe_,
			Size const &      level
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, level);
		}

	};

	struct Uncompress :
		CompressCommon {

	protected:

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

	public:

		static auto do_process_whole (
			IByteStreamView & ripe_,
			OByteStreamView & raw_
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw);
		}

	};

}
