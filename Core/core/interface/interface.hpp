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

	struct Size {
		decltype(sizeof(0)) value;
	};

	struct Boolean {
		bool value;
	};

	struct Character {
		char value;
	};

	struct String {
		Character * data;
		Size        size;
		Size        capacity;
	};

	struct StringList {
		String * data;
		Size     size;
		Size     capacity;
	};

	using Callback = StringList const & (*) (StringList const &);

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
		Callback const &   callback,
		String const &     script,
		StringList const & argument
	) -> String const*;

}
