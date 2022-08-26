#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::Data::Encrypt::XOR {

	inline namespace Common {

		namespace Detail {

		}

	}

	namespace Crypt {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			inline auto process (
				IByteStreamView & plain,
				OByteStreamView & cipher,
				Byte const &      key
			) -> Void {
				while (!plain.full()) {
					cipher.write(plain.read_of() ^ key);
				}
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
