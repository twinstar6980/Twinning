module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.character_font_widget_2.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.character_font_widget_2.version;
import twinning.kernel.tool.popcap.character_font_widget_2.definition;

export namespace Twinning::Kernel::Tool::Popcap::CharacterFontWidget2 {

	template <auto t_version> requires (check_version(t_version))
	struct Common {

		using Definition = Definition<t_version>;

	};

}
