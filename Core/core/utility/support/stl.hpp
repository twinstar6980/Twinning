#pragma once

#include "core/utility/string/string.hpp"

namespace std {

	#pragma region iostream

	inline auto operator << (
		std::ostream &                      thix,
		TwinStar::Core::CStringView const & that
	) -> std::ostream& {
		for (auto & element : that) {
			thix << element.value;
		}
		return thix;
	}

	#pragma endregion

	#pragma region hash

	template <>
	struct hash<TwinStar::Core::IntegerU32> {
		auto operator () (
			TwinStar::Core::IntegerU32 const & it
		) const noexcept -> std::size_t {
			return it.value;
		}
	};

	template <>
	struct hash<TwinStar::Core::CStringView> {
		auto operator () (
			TwinStar::Core::CStringView const & it
		) const noexcept -> std::size_t {
			return it.hash().value;
		}
	};

	template <>
	struct hash<TwinStar::Core::VStringView> {
		auto operator () (
			TwinStar::Core::VStringView const & it
		) const noexcept -> std::size_t {
			return it.hash().value;
		}
	};

	template <>
	struct hash<TwinStar::Core::String> {
		auto operator () (
			TwinStar::Core::String const & it
		) const noexcept -> std::size_t {
			return it.hash().value;
		}
	};

	#pragma endregion

}
