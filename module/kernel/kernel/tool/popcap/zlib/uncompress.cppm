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
			OutputBinaryStreamView & raw,
			InputBinaryStreamView &  ripe,
			Integer const &          window_exponent
		) -> Void {
			ripe.read_constant(k_magic_marker);
			if constexpr (check_version(t_version, {true})) {
				ripe.read_constant(0x00000000_iu32);
			}
			auto header = Header{};
			ripe.read(header);
			Data::Compression::Deflate::Uncompress::process(raw, ripe, Data::Compression::Deflate::Container::Constant::zlib(), window_exponent);
			assert_test(raw.position() == cast_box<Size>(header.raw_size));
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size &                  raw_size,
			InputBinaryStreamView & ripe
		) -> Void {
			raw_size = 0_sz;
			ripe.read_constant(k_magic_marker);
			if constexpr (check_version(t_version, {true})) {
				ripe.read_constant(0x00000000_iu32);
			}
			auto header = Header{};
			ripe.read(header);
			raw_size += cast_box<Size>(header.raw_size);
			return;
		}

		// ----------------

		inline static auto process(
			OutputBinaryStreamView & raw_,
			InputBinaryStreamView &  ripe_,
			Integer const &          window_exponent
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, window_exponent);
		}

		inline static auto estimate(
			Size &                  raw_size,
			InputBinaryStreamView & ripe_
		) -> Void {
			restruct(raw_size);
			M_use_split_stream_of(ripe);
			return estimate_whole(raw_size, ripe);
		}

	};

}
