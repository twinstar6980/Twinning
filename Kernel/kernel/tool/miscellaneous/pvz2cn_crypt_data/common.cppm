module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.common;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hashing.md5.hash;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnCryptData {

	struct Common {

		using MagicMarker = StaticByteArray<2_sz>;

		inline static constexpr auto k_magic_marker = MagicMarker{{0x10_b, 0x00_b}};

		// ----------------

		inline static constexpr auto k_rijndael_block_size = Integer{24_i};

		inline static constexpr auto k_rijndael_key_size = Integer{32_i};

		// ----------------

		inline static auto compute_rijndael_key(
			String const & original_key
		) -> ByteArray {
			auto result = ByteArray{cast_box<Size>(k_rijndael_key_size)};
			auto key_hash = ByteArray{};
			Data::Hashing::Md5::Hash::process(to_byte_view(original_key.view()), key_hash);
			for (auto & key_index : SizeRange{key_hash.size()}) {
				result[key_index * 2_sz + 0_sz] = cast_box<Byte>(CharacterType::to_number_hexadecimal_lower(cast_box<IntegerU8>(key_hash[key_index] / 0x10_b)));
				result[key_index * 2_sz + 1_sz] = cast_box<Byte>(CharacterType::to_number_hexadecimal_lower(cast_box<IntegerU8>(key_hash[key_index] % 0x10_b)));
			}
			return result;
		}

		inline static auto compute_rijndael_initialization_vector(
			ByteArray const & rijndael_key
		) -> ByteArray {
			return ByteArray{rijndael_key.sub(5_ix, cast_box<Size>(k_rijndael_block_size))};
		}

	};

}
