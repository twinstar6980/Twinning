module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.crypt_data.decrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.crypt_data.version;
import twinning.kernel.tool.popcap.crypt_data.common;
import twinning.kernel.tool.data.encryption.exor.common;
import twinning.kernel.tool.data.encryption.exor.encrypt;

export namespace Twinning::Kernel::Tool::Popcap::CryptData {

	template <auto t_version> requires (check_version(t_version))
	struct Decrypt :
		Common<t_version> {

		using Common = Common<t_version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::Header;

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView & raw,
			InputByteStreamView &  ripe,
			Size const &           limit,
			String const &         key
		) -> Void {
			if (ripe.reserve() >= limit + bs_static_size<MagicMarker>() + bs_static_size<Header>()) {
				ripe.read_constant(k_magic_marker);
				auto header = Header{};
				ripe.read(header);
				auto raw_data_size = cast_box<Size>(header.raw_size);
				assert_test(raw_data_size >= limit);
				Data::Encryption::Exor::Encrypt::process(as_left(InputByteStreamView{ripe.forward_view(limit)}), raw, to_byte_view(key.as_view()));
				raw.write(ripe.forward_view(raw_data_size - limit));
			}
			else {
				raw.write(ripe.forward_view(ripe.reserve()));
			}
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size &                       raw_size,
			ConstantByteListView const & ripe,
			Size const &                 limit
		) -> Void {
			raw_size = 0_sz;
			if (ripe.size() >= limit + bs_static_size<MagicMarker>() + bs_static_size<Header>()) {
				auto ripe_stream = InputByteStreamView{ripe};
				ripe_stream.read_constant(k_magic_marker);
				auto header = Header{};
				ripe_stream.read(header);
				auto raw_data_size = cast_box<Size>(header.raw_size);
				assert_test(raw_data_size >= limit);
				raw_size += raw_data_size;
			}
			else {
				raw_size += ripe.size();
			}
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView & raw_,
			InputByteStreamView &  ripe_,
			Size const &           limit,
			String const &         key
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, limit, key);
		}

		inline static auto estimate(
			Size &                       raw_size,
			ConstantByteListView const & ripe,
			Size const &                 limit
		) -> Void {
			restruct(raw_size);
			return estimate_whole(raw_size, ripe, limit);
		}

	};

}
