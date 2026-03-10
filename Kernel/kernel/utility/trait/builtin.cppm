module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region fundamental

	template <typename TIt>
	concept IsBuiltinVoid =
		CategoryConstraint<IsPure<TIt>>
		&& (IsSame<TIt, void>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinBoolean =
		CategoryConstraint<IsPure<TIt>>
		&& (IsSame<TIt, bool>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinUnsignedInteger =
		CategoryConstraint<IsPure<TIt>>
		&& (IsSame<TIt, unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int>)
		;

	template <typename TIt>
	concept IsBuiltinSignedInteger =
		CategoryConstraint<IsPure<TIt>>
		&& (IsSame<TIt, signed char, signed short int, signed int, signed long int, signed long long int>)
		;

	template <typename TIt>
	concept IsBuiltinInteger =
		CategoryConstraint<IsPure<TIt>>
		&& (IsBuiltinUnsignedInteger<TIt> || IsBuiltinSignedInteger<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinSignedFloater =
		CategoryConstraint<IsPure<TIt>>
		&& (IsSame<TIt, float, double, long double>)
		;

	template <typename TIt>
	concept IsBuiltinFloater =
		CategoryConstraint<IsPure<TIt>>
		&& (IsBuiltinSignedFloater<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinNumber =
		CategoryConstraint<IsPure<TIt>>
		&& (IsBuiltinInteger<TIt> || IsBuiltinFloater<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinCharacter =
		CategoryConstraint<IsPure<TIt>>
		&& (IsSame<TIt, char, wchar_t, char8_t, char16_t, char32_t>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinArithmetic =
		CategoryConstraint<IsPure<TIt>>
		&& (IsBuiltinBoolean<TIt> || IsBuiltinNumber<TIt> || IsBuiltinCharacter<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinFundamental =
		CategoryConstraint<IsPure<TIt>>
		&& (IsBuiltinVoid<TIt> || IsBuiltinArithmetic<TIt>)
		;

	#pragma endregion

	#pragma region compound

	template <typename TIt>
	concept IsBuiltinEnumeration =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (std::is_enum_v<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinPointer =
		CategoryConstraint<IsPure<TIt>>
		&& (std::is_pointer_v<TIt>)
		;

	template <typename TIt>
	concept IsBuiltinMemberPointer =
		CategoryConstraint<IsPure<TIt>>
		&& (std::is_member_pointer_v<TIt>)
		;

	// ----------------

	template <typename TIt>
	concept IsBuiltinArray =
		CategoryConstraint<IsPure<TIt>>
		&& (std::is_array_v<TIt>)
		;

	#pragma endregion

}
