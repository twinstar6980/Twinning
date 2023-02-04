#pragma once

#if defined M_interface_implement
#if defined M_compiler_msvc
#define M_symbol_export __declspec(dllexport)
#endif
#if defined M_compiler_clang
#define M_symbol_export __attribute__((visibility("default")))
#endif
#endif

namespace TwinStar::Core::Interface {

	template <typename It>
	using Type = It;

	// ----------------

	struct Size {
		Type<decltype(sizeof(0))> value;
	};

	struct Character {
		Type<char> value;
	};

	struct String {
		Type<Character *> data;
		Type<Size>        size;
		Type<Size>        capacity;
	};

	struct StringList {
		Type<String *> data;
		Type<Size>     size;
		Type<Size>     capacity;
	};

	struct Callback {
		Type<StringList const & (*) (StringList const &)> value;
	};

	// ----------------

	#if defined M_interface_implement
	M_symbol_export
	#endif
	extern auto version (
	) -> Size const*;

	#if defined M_interface_implement
	M_symbol_export
	#endif
	extern auto execute (
		Callback const *   callback,
		String const *     script,
		StringList const * argument
	) -> String const*;

	#if defined M_interface_implement
	M_symbol_export
	#endif
	extern auto prepare (
	) -> String const*;

}
