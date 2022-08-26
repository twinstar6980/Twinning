#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/data/hash/md5.hpp"
#include "core/tool/data/encrypt/rijndael.hpp"

namespace TwinKleS::Core::Tool::PopCap::RTON {

	inline namespace CommonOfEncrypt {

		namespace Detail {

			#pragma region magic identifier

			// TODO : maybe version

			using MagicIdentifier = StaticByteArray<2_szz>;

			inline constexpr auto k_magic_identifier = MagicIdentifier{{0x10_b, 0x00_b}};

			#pragma endregion

			#pragma region parameter

			inline constexpr auto k_crypt_block_size = Size{6_sz};

			inline constexpr auto k_crypt_key_size = Size{8_sz};

			#pragma endregion

		}

	}

	namespace Encrypt {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncrypt::Detail;

			#pragma endregion

			#pragma region compute size

			inline auto compute_size (
				Size const & plain_size
			) -> Size {
				return bs_static_size<MagicIdentifier>() + compute_padded_size(plain_size, k_crypt_block_size * bs_static_size<IntegerU32>());
			}

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & plain,
				OByteStreamView & cipher,
				String const &    key
			) -> Void {
				auto rijndael_plain_container = Array<IntegerU32>{};
				auto rijndael_plain = IStreamView<IntegerU32>{};
				auto plain_data = plain.forward_view(plain.reserve());
				auto rijndael_data_size = compute_padded_size(plain_data.size(), k_crypt_block_size * bs_static_size<IntegerU32>());
				if (plain_data.size() == rijndael_data_size) {
					// NOTE : avoid clang bug : explicitly provide template argument <ListView>
					rijndael_plain.set(from_byte_view<IntegerU32, ListView>(plain_data));
				} else {
					rijndael_plain_container.allocate(rijndael_data_size / bs_static_size<IntegerU32>());
					Range::assign_from(to_byte_view(rijndael_plain_container.as_view()).head(plain_data.size()), plain_data);
					rijndael_plain.set(rijndael_plain_container);
				}
				cipher.write(k_magic_identifier);
				// NOTE : avoid clang bug : explicitly provide template argument <ListView>
				auto rijndael_cipher = OStreamView<IntegerU32>{from_byte_view<IntegerU32, ListView>(cipher.forward_view(rijndael_data_size))};
				auto rijndael_key = Data::Hash::MD5::Hash::process_to_string(to_byte_view(key.view()), k_false);
				auto rijndael_iv = String{rijndael_key.sub(5_ix, 24_sz)};
				Data::Encrypt::Rijndael::Encrypt::process(rijndael_plain, rijndael_cipher, k_crypt_block_size, k_crypt_key_size, rijndael_key, rijndael_iv);
				return;
			}

			#pragma endregion

		}

		using Detail::compute_size;

		using Detail::process;

	}

	namespace Decrypt {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncrypt::Detail;

			#pragma endregion

			#pragma region compute size

			inline auto compute_size (
				Size const & cipher_size
			) -> Size {
				assert_condition(cipher_size >= bs_static_size<MagicIdentifier>());
				auto rijndael_data_size = cipher_size - bs_static_size<MagicIdentifier>();
				assert_condition(is_padded_size(rijndael_data_size, k_crypt_block_size * bs_static_size<IntegerU32>()));
				return rijndael_data_size;
			}

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & cipher,
				OByteStreamView & plain,
				String const &    key
			) -> Void {
				auto rijndael_data_size = compute_size(cipher.reserve());
				assert_condition(cipher.read_of<MagicIdentifier>() == k_magic_identifier);
				auto rijndael_cipher = IStreamView<IntegerU32>{from_byte_view<IntegerU32>(cipher.forward_view(rijndael_data_size))};
				auto rijndael_plain = OStreamView<IntegerU32>{from_byte_view<IntegerU32>(plain.forward_view(rijndael_data_size))};
				auto rijndael_key = Data::Hash::MD5::Hash::process_to_string(to_byte_view(key.view()), k_false);
				auto rijndael_iv = String{rijndael_key.sub(5_ix, 24_sz)};
				Data::Encrypt::Rijndael::Decrypt::process(rijndael_cipher, rijndael_plain, k_crypt_block_size, k_crypt_key_size, rijndael_key, rijndael_iv);
				return;
			}

			#pragma endregion

		}

		using Detail::compute_size;

		using Detail::process;

	}

}
