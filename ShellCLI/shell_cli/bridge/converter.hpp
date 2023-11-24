#pragma once

#include "shell_cli/common.hpp"
#include "shell_cli/bridge/interface.hpp"

namespace TwinStar::ShellCLI::Bridge {

	#pragma region type

	struct Converter {

		#pragma region function

		static auto parse_size (
			Interface::Size const & structure
		) -> std::size_t {
			return structure.value;
		}

		static auto construct_size (
			Interface::Size &   structure,
			std::size_t const & value
		) -> void {
			structure.value = value;
			return;
		}

		static auto destruct_size (
			Interface::Size & structure
		) -> void {
			structure.value = 0;
			return;
		}

		// ----------------

		static auto parse_string (
			Interface::String const & structure
		) -> std::string {
			return std::string{reinterpret_cast<char const *>(structure.data), parse_size(structure.size)};
		}

		static auto construct_string (
			Interface::String & structure,
			std::string const & value
		) -> void {
			structure.data = new Interface::Character[value.size()]{};
			std::memcpy(structure.data, value.data(), value.size());
			construct_size(structure.size, value.size());
			construct_size(structure.capacity, value.size());
			return;
		}

		static auto destruct_string (
			Interface::String & structure
		) -> void {
			if (structure.data != nullptr) {
				delete[] structure.data;
				structure.data = nullptr;
			}
			destruct_size(structure.size);
			destruct_size(structure.capacity);
			return;
		}

		// ----------------

		static auto parse_string_list (
			Interface::StringList const & structure
		) -> std::vector<std::string> {
			auto value = std::vector<std::string>{};
			value.reserve(parse_size(structure.size));
			for (auto & structure_element : std::span{structure.data, parse_size(structure.size)}) {
				value.emplace_back(parse_string(structure_element));
			}
			return value;
		}

		static auto construct_string_list (
			Interface::StringList &          structure,
			std::vector<std::string> const & value
		) -> void {
			structure.data = new Interface::String[value.size()]{};
			for (auto index = std::size_t{0}; index < value.size(); ++index) {
				construct_string(structure.data[index], value[index]);
			}
			construct_size(structure.size, value.size());
			construct_size(structure.capacity, value.size());
			return;
		}

		static auto destruct_string_list (
			Interface::StringList & structure
		) -> void {
			if (structure.data != nullptr) {
				for (auto & structure_element : std::span{structure.data, parse_size(structure.capacity)}) {
					destruct_string(structure_element);
				}
				delete[] structure.data;
				structure.data = nullptr;
			}
			destruct_size(structure.size);
			destruct_size(structure.capacity);
			return;
		}

		// ----------------

		static auto parse_callback (
			Interface::Callback const & structure
		) -> std::function<Interface::String * (Interface::StringList * *, Interface::StringList * *)> {
			return std::function<Interface::String * (Interface::StringList * *, Interface::StringList * *)>{structure.value};
		}

		static auto construct_callback (
			Interface::Callback &                                                                             structure,
			std::function<Interface::String * (Interface::StringList * *, Interface::StringList * *)> const & value
		) -> void {
			structure.value = *value.target<Interface::String * (*) (Interface::StringList * *, Interface::StringList * *)>();
			return;
		}

		static auto destruct_callback (
			Interface::Callback & structure
		) -> void {
			structure.value = nullptr;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
