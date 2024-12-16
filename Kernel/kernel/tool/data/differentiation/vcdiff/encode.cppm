module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.differentiation.vcdiff.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.third.open_vcdiff;

export namespace Twinning::Kernel::Tool::Data::Differentiation::VCDiff {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView & before,
			IByteStreamView & after,
			OByteStreamView & patch,
			Boolean const &   interleaved
		) -> Void {
			auto state = bool{};
			auto patch_container = std::string{};
			auto hashed_dictionary = Third::open_vcdiff::HashedDictionary{cast_pointer<char>(as_variable_pointer(before.current_pointer())).value, before.reserve().value};
			auto encoder = Third::open_vcdiff::VCDiffStreamingEncoder{&hashed_dictionary, !interleaved ? (Third::open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_STANDARD_FORMAT) : (Third::open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_FORMAT_INTERLEAVED), true};
			state = hashed_dictionary.Init();
			assert_test(state);
			state = encoder.StartEncoding(&patch_container);
			assert_test(state);
			state = encoder.EncodeChunk(cast_pointer<char>(as_variable_pointer(after.current_pointer())).value, after.reserve().value, &patch_container);
			assert_test(state);
			state = encoder.FinishEncoding(&patch_container);
			assert_test(state);
			before.forward(before.reserve());
			after.forward(after.reserve());
			for (auto & element : patch_container) {
				patch.write(self_cast<Byte>(element));
			}
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & before_,
			IByteStreamView & after_,
			OByteStreamView & patch_,
			Boolean const &   interleaved
		) -> Void {
			M_use_zps_of(before);
			M_use_zps_of(after);
			M_use_zps_of(patch);
			return process_whole(before, after, patch, interleaved);
		}

	};

}
