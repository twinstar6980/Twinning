#pragma once

#include "core/utility/utility.hpp"
#include "core/third/lzma.hpp"

namespace TwinStar::Core::Tool::Data::Compression::Lzma {

	inline constexpr auto k_property_size = Size{mbw<Size>(Third::lzma::LZMA_PROPS_SIZE_)};

	// ----------------

	struct CompressCommon {

	protected:

	};

	struct Compress :
		CompressCommon {

	protected:

		static auto process_whole (
			IByteStreamView & raw,
			OByteStreamView & ripe,
			OByteStreamView & property,
			Size const &      level
		) -> Void {
			assert_condition(Math::between(level, 0_sz, 9_sz));
			auto raw_size = raw.reserve().value;
			auto ripe_size = ripe.reserve().value;
			auto property_size = property.reserve().value;
			auto state = Third::lzma::LzmaCompress(
				cast_pointer<Third::lzma::Byte>(ripe.current_pointer()).value,
				&ripe_size,
				cast_pointer<Third::lzma::Byte>(raw.current_pointer()).value,
				raw_size,
				cast_pointer<Third::lzma::Byte>(property.current_pointer()).value,
				&property_size,
				static_cast<int>(level.value),
				0,
				-1,
				-1,
				-1,
				-1,
				-1
			);
			assert_condition(state == Third::lzma::SZ_OK_);
			raw.forward(mbw<Size>(raw_size));
			ripe.forward(mbw<Size>(ripe_size));
			property.forward(mbw<Size>(property_size));
			assert_condition(raw.full());
			return;
		}

	public:

		static auto do_process_whole (
			IByteStreamView & raw_,
			OByteStreamView & ripe_,
			OByteStreamView & property_,
			Size const &      level
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			M_use_zps_of(property);
			return process_whole(raw, ripe, property, level);
		}

	};

	struct Uncompress :
		CompressCommon {

	protected:

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw,
			IByteStreamView & property
		) -> Void {
			auto ripe_size = ripe.reserve().value;
			auto raw_size = raw.reserve().value;
			auto property_size = property.reserve().value;
			auto state = Third::lzma::LzmaUncompress(
				cast_pointer<Third::lzma::Byte>(raw.current_pointer()).value,
				&raw_size,
				cast_pointer<Third::lzma::Byte>(ripe.current_pointer()).value,
				&ripe_size,
				cast_pointer<Third::lzma::Byte>(property.current_pointer()).value,
				property_size
			);
			assert_condition(state == Third::lzma::SZ_OK_);
			ripe.forward(mbw<Size>(ripe_size));
			raw.forward(mbw<Size>(raw_size));
			property.forward(mbw<Size>(property_size));
			return;
		}

	public:

		static auto do_process_whole (
			IByteStreamView & ripe_,
			OByteStreamView & raw_,
			IByteStreamView & property_
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			M_use_zps_of(property);
			return process_whole(ripe, raw, property);
		}

	};

}
