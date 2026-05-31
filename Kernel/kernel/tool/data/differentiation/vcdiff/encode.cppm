module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.differentiation.vcdiff.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.third.open_vcdiff;

export namespace Twinning::Kernel::Tool::Data::Differentiation::Vcdiff {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  before,
			InputByteStreamView &  after,
			OutputByteStreamView & patch,
			Boolean const &        interleaved
		) -> Void {
			auto v_state = bool{};
			auto patch_container = std::string{};
			auto hashed_dictionary = Third::open_vcdiff::HashedDictionary{unmake_pointer_unsafe<char>(before.current_pointer()), unmake_box<std::size_t>(before.reserve())};
			auto encoder = Third::open_vcdiff::VCDiffStreamingEncoder{&hashed_dictionary, !interleaved ? (Third::open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_STANDARD_FORMAT) : (Third::open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_FORMAT_INTERLEAVED), true};
			v_state = hashed_dictionary.Init();
			assert_test(v_state);
			v_state = encoder.StartEncoding(&patch_container);
			assert_test(v_state);
			v_state = encoder.EncodeChunk(unmake_pointer_unsafe<char>(after.current_pointer()), unmake_box<std::size_t>(after.reserve()), &patch_container);
			assert_test(v_state);
			v_state = encoder.FinishEncoding(&patch_container);
			assert_test(v_state);
			before.forward(before.reserve());
			after.forward(after.reserve());
			for (auto & element : patch_container) {
				patch.write(unsafe_cast<Byte>(element));
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  before_,
			InputByteStreamView &  after_,
			OutputByteStreamView & patch_,
			Boolean const &        interleaved
		) -> Void {
			M_use_zps_of(before);
			M_use_zps_of(after);
			M_use_zps_of(patch);
			return process_whole(before, after, patch, interleaved);
		}

	};

}
