#pragma once

#include "core/utility/string/string.hpp"

namespace std {

	#pragma region iostream

	inline auto operator << (
		std::ostream &                      thix,
		TwinKleS::Core::CStringView const & that
	) -> std::ostream& {
		for (auto & element : that) {
			thix << element.value;
		}
		return thix;
	}

	#pragma endregion

	#pragma region hash

	template <>
	struct hash<TwinKleS::Core::CStringView> {
		auto operator () (
			TwinKleS::Core::CStringView const & it
		) const noexcept -> std::size_t {
			return it.hash().value;
		}
	};

	template <>
	struct hash<TwinKleS::Core::VStringView> {
		auto operator () (
			TwinKleS::Core::VStringView const & it
		) const noexcept -> std::size_t {
			return it.hash().value;
		}
	};

	template <>
	struct hash<TwinKleS::Core::String> {
		auto operator () (
			TwinKleS::Core::String const & it
		) const noexcept -> std::size_t {
			return it.hash().value;
		}
	};

	#pragma endregion

}
