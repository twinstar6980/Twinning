#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/crypt_data/common.hpp"

namespace TwinStar::Kernel::Tool::PopCap::CryptData {

	template <auto version> requires (check_version(version))
	struct Encrypt :
		Common<version> {

		using Common = Common<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::Header;

		// ----------------

		static auto process_whole (
			IByteStreamView & plain,
			OByteStreamView & cipher,
			Size const &      limit,
			String const &    key
		) -> Void {
			if (plain.reserve() >= limit) {
				cipher.write_constant(k_magic_identifier);
				auto header = Header{};
				header.plain_size = cbw<IntegerU64>(plain.reserve());
				cipher.write(header);
				Data::Encryption::XOR::Encrypt::process(as_lvalue(IByteStreamView{plain.forward_view(limit)}), cipher, to_byte_view(key.as_view()));
			}
			cipher.write(plain.forward_view(plain.reserve()));
			return;
		}

		// ----------------

		static auto estimate_whole (
			Size const & plain_size,
			Size &       cipher_size,
			Size const & limit
		) -> Void {
			cipher_size = k_none_size;
			if (plain_size >= limit) {
				cipher_size += bs_static_size<MagicIdentifier>();
				cipher_size += bs_static_size<Header>();
			}
			cipher_size += plain_size;
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView & plain_,
			OByteStreamView & cipher_,
			Size const &      limit,
			String const &    key
		) -> Void {
			M_use_zps_of(plain);
			M_use_zps_of(cipher);
			return process_whole(plain, cipher, limit, key);
		}

		static auto estimate (
			Size const & plain_size,
			Size &       cipher_size,
			Size const & limit
		) -> Void {
			restruct(cipher_size);
			return estimate_whole(plain_size, cipher_size, limit);
		}

	};

}
