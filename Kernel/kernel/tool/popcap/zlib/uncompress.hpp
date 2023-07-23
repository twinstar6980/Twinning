#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/zlib/common.hpp"
#include "kernel/tool/data/compression/deflate/uncompress.hpp"

namespace TwinStar::Kernel::Tool::PopCap::ZLib {

	template <auto version> requires (check_version(version, {}))
	struct Uncompress :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		using typename Common::Header;

		// ----------------

		static auto process_whole (
			IByteStreamView & ripe,
			OByteStreamView & raw,
			Size const &      window_bits
		) -> Void {
			ripe.read_constant(k_magic_identifier);
			if constexpr (check_version(version, {true})) {
				ripe.read_constant(0x00000000_iu32);
			}
			auto header = Header{};
			ripe.read(header);
			Data::Compression::Deflate::Uncompress::process(ripe, raw, window_bits, Data::Compression::Deflate::Wrapper::Constant::zlib());
			assert_test(raw.position() == cbw<Size>(header.raw_size));
			return;
		}

		// ----------------

		static auto estimate_whole (
			CByteListView const & ripe,
			Size &                raw_size
		) -> Void {
			raw_size = k_none_size;
			auto ripe_stream = IByteStreamView{ripe};
			ripe_stream.read_constant(k_magic_identifier);
			if constexpr (check_version(version, {true})) {
				ripe_stream.read_constant(0x00000000_iu32);
			}
			auto header = Header{};
			ripe_stream.read(header);
			raw_size += cbw<Size>(header.raw_size);
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView & ripe_,
			OByteStreamView & raw_,
			Size const &      window_bits
		) -> Void {
			M_use_zps_of(ripe);
			M_use_zps_of(raw);
			return process_whole(ripe, raw, window_bits);
		}

		static auto estimate (
			CByteListView const & ripe,
			Size &                raw_size
		) -> Void {
			restruct(raw_size);
			return estimate_whole(ripe, raw_size);
		}

	};

}
