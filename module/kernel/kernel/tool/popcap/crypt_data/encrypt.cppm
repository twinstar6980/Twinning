module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.crypt_data.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.crypt_data.version;
import twinning.kernel.tool.popcap.crypt_data.common;
import twinning.kernel.tool.data.encryption.exor.common;
import twinning.kernel.tool.data.encryption.exor.encrypt;

export namespace Twinning::Kernel::Tool::Popcap::CryptData {

	template <auto t_version> requires (check_version(t_version))
	struct Encrypt :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::Header;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			Size const &             limit,
			String const &           key
		) -> Void {
			if (raw.reserve() >= limit) {
				ripe.write_constant(k_magic_marker);
				auto header = Header{};
				header.raw_size = cast_box<IntegerU64>(raw.reserve());
				ripe.write(header);
				Data::Encryption::Exor::Encrypt::process(as_left(raw.split_forward(limit)), ripe, to_binary_view(key.as_view()));
			}
			ripe.write(raw.split_forward(raw.reserve()).view());
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size,
			Size const & limit
		) -> Void {
			ripe_size = 0_sz;
			if (raw_size >= limit) {
				ripe_size += binary_static_size_of<MagicMarker>();
				ripe_size += binary_static_size_of<Header>();
			}
			ripe_size += raw_size;
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			Size const &             limit,
			String const &           key
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, limit, key);
		}

		inline static auto estimate(
			Size const & raw_size,
			Size &       ripe_size,
			Size const & limit
		) -> Void {
			restruct(ripe_size);
			return estimate_whole(raw_size, ripe_size, limit);
		}

	};

}
