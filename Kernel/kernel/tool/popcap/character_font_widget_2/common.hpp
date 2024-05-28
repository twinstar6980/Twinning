#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/tool/popcap/character_font_widget_2/version.hpp"
#include "kernel/tool/popcap/character_font_widget_2/definition.hpp"

namespace Twinning::Kernel::Tool::PopCap::CharacterFontWidget2 {

	template <auto version> requires (check_version(version))
	struct Common {

		using Definition = Definition<version>;

	};

}
