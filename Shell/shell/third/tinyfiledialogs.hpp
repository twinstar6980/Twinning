#pragma once

#if defined M_system_windows || defined M_system_linux || defined M_system_macintosh

#if defined M_compiler_msvc
#pragma warning(push, 0)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif

#include "third/tinyfiledialogs/tinyfiledialogs.h"

#if defined M_compiler_msvc
#pragma warning(pop)
#endif
#if defined M_compiler_clang
#pragma clang diagnostic pop
#endif

namespace Twinning::Shell::Third::tinyfiledialogs {

	inline constexpr auto tinyfd_openFileDialog = ::tinyfd_openFileDialog;

	inline constexpr auto tinyfd_selectFolderDialog = ::tinyfd_selectFolderDialog;

	inline constexpr auto tinyfd_saveFileDialog = ::tinyfd_saveFileDialog;

	inline constexpr auto tinyfd_notifyPopup = ::tinyfd_notifyPopup;

}

#endif
