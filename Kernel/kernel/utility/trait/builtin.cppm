module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.builtin;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region fundamental

	template <typename It>
	concept IsBuiltinVoid =
		CategoryConstraint<IsPure<It>>
		&& (IsSame<It, void>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinBoolean =
		CategoryConstraint<IsPure<It>>
		&& (IsSame<It, bool>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinUnsignedInteger =
		CategoryConstraint<IsPure<It>>
		&& (IsSame<It, unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int>)
		;

	template <typename It>
	concept IsBuiltinSignedInteger =
		CategoryConstraint<IsPure<It>>
		&& (IsSame<It, signed char, signed short int, signed int, signed long int, signed long long int>)
		;

	template <typename It>
	concept IsBuiltinInteger =
		CategoryConstraint<IsPure<It>>
		&& (IsBuiltinUnsignedInteger<It> || IsBuiltinSignedInteger<It>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinSignedFloater =
		CategoryConstraint<IsPure<It>>
		&& (IsSame<It, float, double, long double>)
		;

	template <typename It>
	concept IsBuiltinFloater =
		CategoryConstraint<IsPure<It>>
		&& (IsBuiltinSignedFloater<It>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinNumber =
		CategoryConstraint<IsPure<It>>
		&& (IsBuiltinInteger<It> || IsBuiltinFloater<It>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinCharacter =
		CategoryConstraint<IsPure<It>>
		&& (IsSame<It, char, wchar_t, char8_t, char16_t, char32_t>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinArithmetic =
		CategoryConstraint<IsPure<It>>
		&& (IsBuiltinBoolean<It> || IsBuiltinNumber<It> || IsBuiltinCharacter<It>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinFundamental =
		CategoryConstraint<IsPure<It>>
		&& (IsBuiltinVoid<It> || IsBuiltinArithmetic<It>)
		;

	#pragma endregion

	#pragma region compound

	template <typename It>
	concept IsBuiltinEnumeration =
		CategoryConstraint<IsPureInstance<It>>
		&& (std::is_enum_v<It>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinPointer =
		CategoryConstraint<IsPure<It>>
		&& (std::is_pointer_v<It>)
		;

	template <typename It>
	concept IsBuiltinMemberPointer =
		CategoryConstraint<IsPure<It>>
		&& (std::is_member_pointer_v<It>)
		;

	// ----------------

	template <typename It>
	concept IsBuiltinArray =
		CategoryConstraint<IsPure<It>>
		&& (std::is_array_v<It>)
		;

	#pragma endregion

}
