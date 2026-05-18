module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.zlib.uncompress;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.zlib.version;
import twinning.kernel.tool.popcap.zlib.common;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.uncompress;

export namespace Twinning::Kernel::Tool::Popcap::Zlib {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Uncompress :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::IntegerOfPlatform;

		using typename Common::Header;

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView & raw,
			InputByteStreamView &  ripe,
			Integer const &        window_exponent
		) -> Void {
			ripe.read_constant(k_magic_marker);
			if constexpr (check_version(t_version, {true})) {
				ripe.read_constant(0x00000000_iu32);
			}
			auto header = Header{};
			ripe.read(header);
			Data::Compression::Deflate::Uncompress::process(raw, ripe, window_exponent, Data::Compression::Deflate::Wrapper::Constant::zlib());
			assert_test(raw.position() == cbox<Size>(header.raw_size));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size &                       raw_size,
			ConstantByteListView const & ripe
		) -> Void {
			raw_size = 0_sz;
			auto ripe_stream = InputByteStreamView{ripe};
			ripe_stream.read_constant(k_magic_marker);
			if constexpr (check_version(t_version, {true})) {
				ripe_stream.read_constant(0x00000000_iu32);
			}
			auto header = Header{};
			ripe_stream.read(header);
			raw_size += cbox<Size>(header.raw_size);
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView & raw_,
			InputByteStreamView &  ripe_,
			Integer const &        window_exponent
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, window_exponent);
		}

		inline static auto estimate(
			Size &                       raw_size,
			ConstantByteListView const & ripe
		) -> Void {
			restruct(raw_size);
			return estimate_whole(raw_size, ripe);
		}

	};

}
