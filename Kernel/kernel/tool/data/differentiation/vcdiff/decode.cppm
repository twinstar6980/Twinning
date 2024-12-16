module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.differentiation.vcdiff.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.third.open_vcdiff;

export namespace Twinning::Kernel::Tool::Data::Differentiation::VCDiff {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			IByteStreamView & before,
			OByteStreamView & after,
			IByteStreamView & patch,
			Size const &      maximum_window_size
		) -> Void {
			auto state = bool{};
			auto after_container = std::string{};
			auto decoder = Third::open_vcdiff::VCDiffStreamingDecoder{};
			decoder.SetAllowVcdTarget(true);
			decoder.SetMaximumTargetFileSize(std::numeric_limits<std::int32_t>::max());
			decoder.SetMaximumTargetWindowSize(maximum_window_size.value);
			decoder.StartDecoding(cast_pointer<char>(as_variable_pointer(before.current_pointer())).value, before.reserve().value);
			state = decoder.DecodeChunk(cast_pointer<char>(as_variable_pointer(patch.current_pointer())).value, patch.reserve().value, &after_container);
			assert_test(state);
			state = decoder.FinishDecoding();
			assert_test(state);
			before.forward(before.reserve());
			patch.forward(patch.reserve());
			for (auto & element : after_container) {
				after.write(self_cast<Byte>(element));
			}
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView & before_,
			OByteStreamView & after_,
			IByteStreamView & patch_,
			Size const &      maximum_window_size
		) -> Void {
			M_use_zps_of(before);
			M_use_zps_of(after);
			M_use_zps_of(patch);
			return process_whole(before, after, patch, maximum_window_size);
		}

	};

}
