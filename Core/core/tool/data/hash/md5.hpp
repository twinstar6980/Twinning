#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/md5.hpp"

namespace TwinKleS::Core::Tool::Data::Hash::MD5 {

	inline namespace Common {

		namespace Detail {

			#pragma region parameter

			inline constexpr auto k_byte_count = Size{16_sz};

			#pragma endregion

		}

		using Detail::k_byte_count;

	}

	namespace Hash {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				CByteListView const & data
			) -> ByteArray {
				auto generator = ThirdParty::md5::MD5{std::span{cast_pointer<ThirdParty::md5::byte>(data.begin()).value, data.size().value}};
				return ByteArray{cast_pointer<Byte>(make_pointer(generator.getDigest())), k_byte_count};
			}

			// ----------------

			inline auto process_to_string (
				CByteListView const & data,
				Boolean const &       use_upper_case_number
			) -> String {
				auto & make_character = use_upper_case_number ? (CharacterType::to_number_hex_upper) : (CharacterType::to_number_hex_lower);
				auto   string = String{k_byte_count * 2_sz};
				for (auto & element : process(data)) {
					string.append(make_character(cbw<IntegerU8>(element / 0x10_b)));
					string.append(make_character(cbw<IntegerU8>(element % 0x10_b)));
				}
				return string;
			}

			#pragma endregion

		}

		using Detail::process;

		using Detail::process_to_string;

	}

}
