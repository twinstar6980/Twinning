module;

#if defined M_system_windows || defined M_system_linux || defined M_system_macintosh

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "third/tinyfiledialogs/tinyfiledialogs.h"

#pragma clang diagnostic pop

#endif

export module twinning.shell.third.tinyfiledialogs;

#if defined M_system_windows || defined M_system_linux || defined M_system_macintosh

export namespace Twinning::Shell::Third::tinyfiledialogs {

	using ::tinyfd_openFileDialog;

	using ::tinyfd_selectFolderDialog;

	using ::tinyfd_saveFileDialog;

	using ::tinyfd_notifyPopup;

}

#endif
