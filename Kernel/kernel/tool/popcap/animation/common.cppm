module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.animation.common;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.animation.version;
import twinning.kernel.tool.popcap.animation.definition;

export namespace Twinning::Kernel::Tool::PopCap::Animation {

	template <auto version> requires (check_version(version, {}))
	struct Common {

		using Definition = Definition<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0xBAF01954_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		struct LayerRemoveFlag {

			inline static constexpr auto k_count = Size{5_sz};

			// ----------------

			inline static constexpr auto unused_1 = Size{1_ix};

			inline static constexpr auto unused_2 = Size{2_ix};

			inline static constexpr auto unused_3 = Size{3_ix};

			inline static constexpr auto unused_4 = Size{4_ix};

			inline static constexpr auto unused_5 = Size{5_ix};

		};

		struct LayerAppendFlag {

			inline static constexpr auto k_count = Size{5_sz};

			// ----------------

			inline static constexpr auto time_scale = Size{1_ix};

			inline static constexpr auto name = Size{2_ix};

			inline static constexpr auto preload_frame = Size{3_ix};

			inline static constexpr auto additive = Size{4_ix};

			inline static constexpr auto sprite = Size{5_ix};

		};

		struct LayerChangeFlag {

			inline static constexpr auto k_count = Size{6_sz};

			// ----------------

			inline static constexpr auto sprite_frame_number = Size{1_ix};

			inline static constexpr auto long_coord = Size{2_ix};

			inline static constexpr auto matrix = Size{3_ix};

			inline static constexpr auto color = Size{4_ix};

			inline static constexpr auto rotate = Size{5_ix};

			inline static constexpr auto source_rectangle = Size{6_ix};

		};

		struct FrameFlag {

			inline static constexpr auto k_count = Size{8_sz};

			// ----------------

			inline static constexpr auto remove = Size{1_ix};

			inline static constexpr auto append = Size{2_ix};

			inline static constexpr auto change = Size{3_ix};

			inline static constexpr auto label = Size{4_ix};

			inline static constexpr auto stop = Size{5_ix};

			inline static constexpr auto command = Size{6_ix};

			inline static constexpr auto unused_7 = Size{7_ix};

			inline static constexpr auto unused_8 = Size{8_ix};

		};

		// ----------------

		struct ValueRate {

			inline static constexpr auto time = Floater{65536.0_f};

			inline static constexpr auto size = Floater{20.0_f};

			inline static constexpr auto angle = Floater{1000.0_f};

			inline static constexpr auto matrix = Floater{65536.0_f};

			inline static constexpr auto matrix_exact = Floater{20.0_f * 65536.0_f};

			inline static constexpr auto color = Floater{255.0_f};

		};

	};

}
