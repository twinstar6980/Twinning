module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.differentiation.vcdiff.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.third.open_vcdiff;

export namespace Twinning::Kernel::Tool::Data::Differentiation::Vcdiff {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputByteStreamView &  before,
			OutputByteStreamView & after,
			InputByteStreamView &  patch,
			Integer const &        maximum_window_size
		) -> Void {
			auto v_state = bool{};
			auto after_container = std::string{};
			auto decoder = Third::open_vcdiff::VCDiffStreamingDecoder{};
			decoder.SetAllowVcdTarget(true);
			decoder.SetMaximumTargetFileSize(unmake_box<std::size_t>(k_number_maximum<IntegerS32>));
			decoder.SetMaximumTargetWindowSize(unmake_box<std::size_t>(maximum_window_size));
			decoder.StartDecoding(unmake_pointer_unsafe<char>(before.current_pointer()), unmake_box<std::size_t>(before.reserve()));
			v_state = decoder.DecodeChunk(unmake_pointer_unsafe<char>(patch.current_pointer()), unmake_box<std::size_t>(patch.reserve()), &after_container);
			assert_test(v_state);
			v_state = decoder.FinishDecoding();
			assert_test(v_state);
			before.forward(before.reserve());
			patch.forward(patch.reserve());
			for (auto & element : after_container) {
				after.write(unsafe_cast<Byte>(element));
			}
			return;
		}

		// ----------------

		inline static auto process(
			InputByteStreamView &  before_,
			OutputByteStreamView & after_,
			InputByteStreamView &  patch_,
			Integer const &        maximum_window_size
		) -> Void {
			M_use_zps_of(before);
			M_use_zps_of(after);
			M_use_zps_of(patch);
			return process_whole(before, after, patch, maximum_window_size);
		}

	};

}
