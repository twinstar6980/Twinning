module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.std;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.storage.path;

export {

	#pragma region hash

	template <>
	struct std::hash<Twinning::Kernel::ConstantStringView> {
		auto operator () (
			Twinning::Kernel::ConstantStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
		}
	};

	template <>
	struct std::hash<Twinning::Kernel::VariableStringView> {
		auto operator () (
			Twinning::Kernel::VariableStringView const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
		}
	};

	template <>
	struct std::hash<Twinning::Kernel::String> {
		auto operator () (
			Twinning::Kernel::String const & it
		) const noexcept -> std::size_t {
			return std::hash<std::string_view>{}(Twinning::Kernel::make_std_string_view(it));
		}
	};

	#pragma endregion

	#pragma region formatter

	template <>
	struct std::formatter<Twinning::Kernel::ConstantStringView> :
		std::formatter<std::string_view> {
		template <typename Context> requires
			Twinning::Kernel::NoneConstraint
		auto format (
			Twinning::Kernel::ConstantStringView const & value,
			Context &                                    context
		) const -> typename Context::iterator {
			return std::formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
		}
	};

	template <>
	struct std::formatter<Twinning::Kernel::VariableStringView> :
		std::formatter<std::string_view> {
		template <typename Context> requires
			Twinning::Kernel::NoneConstraint
		auto format (
			Twinning::Kernel::VariableStringView const & value,
			Context &                                    context
		) const -> typename Context::iterator {
			return std::formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
		}
	};

	template <>
	struct std::formatter<Twinning::Kernel::String> :
		std::formatter<std::string_view> {
		template <typename Context> requires
			Twinning::Kernel::NoneConstraint
		auto format (
			Twinning::Kernel::String const & value,
			Context &                        context
		) const -> typename Context::iterator {
			return std::formatter<std::string_view>::format(Twinning::Kernel::make_std_string_view(value), context);
		}
	};

	// ----------------

	template <>
	struct std::formatter<Twinning::Kernel::Storage::Path> :
		std::formatter<Twinning::Kernel::String> {
		template <typename Context> requires
			Twinning::Kernel::NoneConstraint
		auto format (
			Twinning::Kernel::Storage::Path const & value,
			Context &                               context
		) const -> typename Context::iterator {
			return std::formatter<Twinning::Kernel::String>::format(value.to_string(), context);
		}
	};

	// ----------------

	// TODO: specialization format for floater box?
	template <typename TType> requires
		Twinning::Kernel::AutoConstraint
		&& (Twinning::Kernel::IsBaseBox<TType>)
	struct std::formatter<TType> :
		std::formatter<typename TType::Value> {
		template <typename Context> requires
			Twinning::Kernel::NoneConstraint
		auto format (
			TType const & value,
			Context &     context
		) const -> typename Context::iterator {
			return std::formatter<typename TType::Value>::format(value.value, context);
		}
	};

	#pragma endregion

}
