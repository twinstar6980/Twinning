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
			decoder.SetMaximumTargetFileSize(static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()));
			decoder.SetMaximumTargetWindowSize(static_cast<std::size_t>(maximum_window_size.value));
			decoder.StartDecoding(cast_pointer<char>(as_variable_pointer(before.current_pointer())).value, before.reserve().value);
			v_state = decoder.DecodeChunk(cast_pointer<char>(as_variable_pointer(patch.current_pointer())).value, patch.reserve().value, &after_container);
			assert_test(v_state);
			v_state = decoder.FinishDecoding();
			assert_test(v_state);
			before.forward(before.reserve());
			patch.forward(patch.reserve());
			for (auto & element : after_container) {
				after.write(self_cast<Byte>(element));
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
