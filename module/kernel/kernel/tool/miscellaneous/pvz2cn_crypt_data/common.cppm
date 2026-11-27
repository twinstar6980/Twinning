module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.common;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hashing.md.common;
import twinning.kernel.tool.data.hashing.md.hash;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnCryptData {

	struct Common {

		using MagicMarker = StaticArray<Byte, 2_sz>;

		inline static constexpr auto k_magic_marker = MagicMarker{{0x10_b, 0x00_b}};

		// ----------------

		inline static constexpr auto k_rijndael_block_size = Integer{24_i};

		inline static constexpr auto k_rijndael_key_size = Integer{32_i};

		// ----------------

		inline static auto compute_rijndael_key(
			String const & original_key
		) -> Binary {
			auto result = Binary{cast_box<Size>(k_rijndael_key_size)};
			auto key_hash = Binary{};
			Data::Hashing::Md::Hash::process(to_binary_view(original_key.as_view()), key_hash, Data::Hashing::Md::Mode::Constant::v5(), 128_sz);
			for (auto & key_index : SizeRange{key_hash.size()}) {
				result[key_index * 2_sz + 0_sz] = cast_box<Byte>(StringUtility::to_number_radix_character<16_sz>(cast_box<IntegerU8>(key_hash[key_index] / 0x10_b), k_false));
				result[key_index * 2_sz + 1_sz] = cast_box<Byte>(StringUtility::to_number_radix_character<16_sz>(cast_box<IntegerU8>(key_hash[key_index] % 0x10_b), k_false));
			}
			return result;
		}

		inline static auto compute_rijndael_initialization_vector(
			Binary const & rijndael_key
		) -> Binary {
			return Binary{rijndael_key.sub(5_ix, cast_box<Size>(k_rijndael_block_size))};
		}

	};

}
