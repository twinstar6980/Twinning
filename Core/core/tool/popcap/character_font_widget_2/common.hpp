#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/character_font_widget_2/version.hpp"
#include "core/tool/popcap/character_font_widget_2/definition.hpp"

namespace TwinStar::Core::Tool::PopCap::CharacterFontWidget2 {

	template <auto version> requires (check_version(version))
	struct Common {

		using Definition = Definition<version>;

	};

}
