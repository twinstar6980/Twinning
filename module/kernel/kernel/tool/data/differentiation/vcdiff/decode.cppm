module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.differentiation.vcdiff.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.dependency.open_vcdiff;

export namespace Twinning::Kernel::Tool::Data::Differentiation::Vcdiff {

	struct Decode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  before,
			OutputBinaryStreamView & after,
			InputBinaryStreamView &  patch,
			Integer const &          maximum_window_size
		) -> Void {
			auto v_state = bool{};
			auto after_container = std::string{};
			auto decoder = Dependency::open_vcdiff::VCDiffStreamingDecoder{};
			decoder.SetAllowVcdTarget(true);
			decoder.SetMaximumTargetFileSize(unmake_box<std::size_t>(maximum_number_of<IntegerS32>()));
			decoder.SetMaximumTargetWindowSize(unmake_box<std::size_t>(maximum_window_size));
			decoder.StartDecoding(unmake_pointer_unsafe<char>(before.current()), unmake_box<std::size_t>(before.reserve()));
			v_state = decoder.DecodeChunk(unmake_pointer_unsafe<char>(patch.current()), unmake_box<std::size_t>(patch.reserve()), &after_container);
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
			InputBinaryStreamView &  before_,
			OutputBinaryStreamView & after_,
			InputBinaryStreamView &  patch_,
			Integer const &          maximum_window_size
		) -> Void {
			M_use_split_stream_of(before);
			M_use_split_stream_of(after);
			M_use_split_stream_of(patch);
			return process_whole(before, after, patch, maximum_window_size);
		}

	};

}
