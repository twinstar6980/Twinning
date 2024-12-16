module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.character_font_widget_2.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.character_font_widget_2.version;
import twinning.kernel.tool.popcap.character_font_widget_2.definition;

export namespace Twinning::Kernel::Tool::PopCap::CharacterFontWidget2 {

	template <auto version> requires (check_version(version))
	struct Common {

		using Definition = Definition<version>;

	};

}
