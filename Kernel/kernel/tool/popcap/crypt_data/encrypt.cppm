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
			InputByteStreamView &  raw,
			OutputByteStreamView & ripe,
			Size const &           limit,
			String const &         key
		) -> Void {
			if (raw.reserve() >= limit) {
				ripe.write_constant(k_magic_marker);
				auto header = Header{};
				header.raw_size = cast_box<IntegerU64>(raw.reserve());
				ripe.write(header);
				Data::Encryption::Exor::Encrypt::process(as_left(InputByteStreamView{raw.forward_view(limit)}), ripe, to_byte_view(key.as_view()));
			}
			ripe.write(raw.forward_view(raw.reserve()));
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
				ripe_size += bs_static_size<MagicMarker>();
				ripe_size += bs_static_size<Header>();
			}
			ripe_size += raw_size;
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  raw_,
			OutputByteStreamView & ripe_,
			Size const &           limit,
			String const &         key
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
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
