#pragma once

#if defined M_system_windows || defined M_system_linux || defined M_system_macintosh

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/tinyfiledialogs/tinyfiledialogs.h"

#pragma clang diagnostic pop

namespace Twinning::Shell::Third::tinyfiledialogs {

	inline constexpr auto tinyfd_openFileDialog = ::tinyfd_openFileDialog;

	inline constexpr auto tinyfd_selectFolderDialog = ::tinyfd_selectFolderDialog;

	inline constexpr auto tinyfd_saveFileDialog = ::tinyfd_saveFileDialog;

	inline constexpr auto tinyfd_notifyPopup = ::tinyfd_notifyPopup;

}

#endif
