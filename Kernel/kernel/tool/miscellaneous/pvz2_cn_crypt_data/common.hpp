#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/data/hash/md5/hash.hpp"

namespace Twinning::Kernel::Tool::Miscellaneous::PvZ2CNCryptData {

	struct Common {

		using MagicIdentifier = StaticByteArray<2_sz>;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{{0x10_b, 0x00_b}};

		// ----------------

		inline static constexpr auto k_crypt_block_size = Size{24_sz};

		inline static constexpr auto k_crypt_key_size = Size{32_sz};

		// ----------------

		static auto compute_rijndael_key (
			String const & key
		) -> String {
			auto string = String{16_sz * 2_sz};
			auto value_byte = ByteArray{};
			Data::Hash::MD5::Hash::process(to_byte_view(key.view()), value_byte);
			for (auto & element : value_byte) {
				string.append(CharacterType::to_number_hex_lower(cbw<IntegerU8>(element / 0x10_b)));
				string.append(CharacterType::to_number_hex_lower(cbw<IntegerU8>(element % 0x10_b)));
			}
			return string;
		}

		static auto compute_rijndael_iv (
			String const & rijndael_key
		) -> String {
			return String{rijndael_key.sub(5_ix, 24_sz)};
		}

	};

}
