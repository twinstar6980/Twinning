module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.crypt_data.decrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.crypt_data.version;
import twinning.kernel.tool.popcap.crypt_data.common;
import twinning.kernel.tool.data.encryption.exor.common;
import twinning.kernel.tool.data.encryption.exor.encrypt;

export namespace Twinning::Kernel::Tool::Popcap::CryptData {

	template <auto version> requires (check_version(version))
	struct Decrypt :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicMarker;

		using Common::k_magic_marker;

		using typename Common::Header;

		// ----------------

		inline static auto process_whole (
			InputByteStreamView &  cipher,
			OutputByteStreamView & plain,
			Size const &           limit,
			String const &         key
		) -> Void {
			if (cipher.reserve() >= limit + bs_static_size<MagicMarker>() + bs_static_size<Header>()) {
				cipher.read_constant(k_magic_marker);
				auto header = Header{};
				cipher.read(header);
				auto plain_data_size = cbox<Size>(header.plain_size);
				assert_test(plain_data_size >= limit);
				Data::Encryption::Exor::Encrypt::process(as_left(InputByteStreamView{cipher.forward_view(limit)}), plain, to_byte_view(key.as_view()));
				plain.write(cipher.forward_view(plain_data_size - limit));
			}
			else {
				plain.write(cipher.forward_view(cipher.reserve()));
			}
			return;
		}

		// ----------------

		inline static auto estimate_whole (
			ConstantByteListView const & cipher,
			Size &                       plain_size,
			Size const &                 limit
		) -> Void {
			plain_size = k_none_size;
			if (cipher.size() >= limit + bs_static_size<MagicMarker>() + bs_static_size<Header>()) {
				auto cipher_stream = InputByteStreamView{cipher};
				cipher_stream.read_constant(k_magic_marker);
				auto header = Header{};
				cipher_stream.read(header);
				auto plain_data_size = cbox<Size>(header.plain_size);
				assert_test(plain_data_size >= limit);
				plain_size += plain_data_size;
			}
			else {
				plain_size += cipher.size();
			}
			return;
		}

		// ----------------

		inline static auto process (
			InputByteStreamView &  cipher_,
			OutputByteStreamView & plain_,
			Size const &           limit,
			String const &         key
		) -> Void {
			M_use_zps_of(cipher);
			M_use_zps_of(plain);
			return process_whole(cipher, plain, limit, key);
		}

		inline static auto estimate (
			ConstantByteListView const & cipher,
			Size &                       plain_size,
			Size const &                 limit
		) -> Void {
			restruct(plain_size);
			return estimate_whole(cipher, plain_size, limit);
		}

	};

}
