#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/Rijndael.hpp"

namespace TwinKleS::Core::Tool::Data::Encrypt::Rijndael {

	inline namespace Common {

		namespace Detail {

			#pragma region parameter

			enum class Mode : ZIntegerU8 {
				ecb = ThirdParty::Rijndael::CRijndael::ECB,
				cbc = ThirdParty::Rijndael::CRijndael::CBC,
				cfb = ThirdParty::Rijndael::CRijndael::CFB,
			};

			#pragma endregion

			#pragma region parameter

			inline constexpr auto k_valid_block_size = StaticArray<Size, 3_szz>{{16_sz, 24_sz, 32_sz}};

			inline constexpr auto k_empty_iv = StaticArray<Character, 32_szz>{
				{
					0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
					0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
					0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
					0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c, 0x00_c,
				}
			};

			#pragma endregion

		}

		using Detail::Mode;

	}

	namespace Encrypt {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & plain,
				OByteStreamView & cipher,
				Mode const &      mode,
				Size const &      block_size,
				Size const &      key_size,
				String const &    key,
				String const &    iv
			) -> Void {
				assert_condition(is_padded_size(plain.reserve(), block_size));
				assert_condition(Range::has(k_valid_block_size, block_size));
				assert_condition(Range::has(k_valid_block_size, key_size));
				assert_condition(is_padded_size(key.size() * k_type_bit_count<Character>, key_size));
				assert_condition(is_padded_size(iv.size() * k_type_bit_count<Character>, block_size));
				auto iv_view = mode == Mode::ecb ? k_empty_iv.view().head(block_size) : iv.view();
				auto plain_size = plain.reserve();
				if (plain_size != k_none_size) {
					auto rijndael = ThirdParty::Rijndael::CRijndael{};
					rijndael.MakeKey(cast_pointer<char>(key.begin()).value, cast_pointer<char>(iv_view.begin()).value, static_cast<int>(key_size.value), static_cast<int>(block_size.value));
					rijndael.Encrypt(cast_pointer<char>(plain.current_pointer()).value, cast_pointer<char>(cipher.current_pointer()).value, plain.reserve().value, static_cast<int>(mode));
				}
				plain.forward(plain_size);
				cipher.forward(plain_size);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Decrypt {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & cipher,
				OByteStreamView & plain,
				Mode const &      mode,
				Size const &      block_size,
				Size const &      key_size,
				String const &    key,
				String const &    iv
			) -> Void {
				assert_condition(is_padded_size(cipher.reserve(), block_size));
				assert_condition(Range::has(k_valid_block_size, block_size));
				assert_condition(Range::has(k_valid_block_size, key_size));
				assert_condition(is_padded_size(key.size() * k_type_bit_count<Character>, key_size));
				assert_condition(is_padded_size(iv.size() * k_type_bit_count<Character>, block_size));
				auto iv_view = mode == Mode::ecb ? k_empty_iv.view().head(block_size) : iv.view();
				auto cipher_size = cipher.reserve();
				if (cipher_size != k_none_size) {
					auto rijndael = ThirdParty::Rijndael::CRijndael{};
					rijndael.MakeKey(cast_pointer<char>(key.begin()).value, cast_pointer<char>(iv_view.begin()).value, static_cast<int>(key_size.value), static_cast<int>(block_size.value));
					rijndael.Decrypt(cast_pointer<char>(cipher.current_pointer()).value, cast_pointer<char>(plain.current_pointer()).value, cipher.reserve().value, static_cast<int>(mode));
				}
				cipher.forward(cipher_size);
				plain.forward(cipher_size);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}

namespace TwinKleS::Core::Trait::Reflection {

	M_reflection_enum(
		M_wrap(Tool::Data::Encrypt::Rijndael::Mode),
		M_wrap(Mode),
		M_wrap(ecb, cbc, cfb),
	);

}
