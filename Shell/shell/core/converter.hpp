#pragma once

#include "shell/common.hpp"
#include "shell/core/interface.hpp"

namespace TwinStar::Shell::Core::Converter {

	#pragma region function

	inline auto from_size (
		Interface::Size const & structure
	) -> std::size_t {
		return structure.value;
	}

	inline auto to_size (
		std::size_t const & value
	) -> Interface::Size {
		return Interface::Size{
			.value = value,
		};
	}

	// ----------------

	inline auto from_string (
		Interface::String const & structure
	) -> std::string {
		return std::string{reinterpret_cast<char const *>(structure.data), from_size(structure.size)};
	}

	inline auto to_string (
		std::string const & value
	) -> Interface::String {
		return Interface::String{
			.data = const_cast<Interface::Character *>(reinterpret_cast<Interface::Character const *>(value.data())),
			.size = to_size(value.size()),
			.capacity = to_size(value.size()),
		};
	}

	// ----------------

	inline auto from_string_list (
		Interface::StringList const & structure
	) -> std::vector<std::string> {
		auto value = std::vector<std::string>{};
		value.reserve(from_size(structure.size));
		for (auto & structure_element : std::span{structure.data, from_size(structure.size)}) {
			value.emplace_back(reinterpret_cast<char const *>(structure_element.data), from_size(structure_element.size));
		}
		return value;
	}

	inline auto allocate_string_list (
		std::vector<std::string> const & value
	) -> Interface::StringList {
		auto structure = Interface::StringList{
			.data = new Interface::String[value.size()]{},
			.size = to_size(value.size()),
			.capacity = to_size(value.size()),
		};
		for (auto index = std::size_t{0}; index < value.size(); ++index) {
			auto & value_element = value[index];
			structure.data[index] = Interface::String{
				.data = new Interface::Character[value_element.size()]{},
				.size = to_size(value_element.size()),
				.capacity = to_size(value_element.size()),
			};
			std::memcpy(structure.data[index].data, value_element.data(), value_element.size());
		}
		return structure;
	}

	inline auto free_string_list (
		Interface::StringList & structure
	) -> void {
		if (structure.data != nullptr) {
			for (auto & structure_element : std::span{structure.data, from_size(structure.capacity)}) {
				delete[] structure_element.data;
			}
			delete[] structure.data;
			structure.data = nullptr;
		}
		structure.size = to_size(0);
		structure.capacity = to_size(0);
		return;
	}

	// ----------------

	inline auto from_callback (
		Interface::Callback const & structure
	) -> std::function<Interface::StringList const & (Interface::StringList const &)> {
		return std::function<Interface::StringList const & (Interface::StringList const &)>{structure.value};
	}

	inline auto to_callback (
		std::function<Interface::StringList const & (Interface::StringList const &)> const & value
	) -> Interface::Callback {
		return Interface::Callback{
			.value = *value.target<Interface::StringList const & (*) (Interface::StringList const &)>(),
		};
	}

	#pragma endregion

}
