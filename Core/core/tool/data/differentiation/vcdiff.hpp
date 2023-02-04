#pragma once

#include "core/utility/utility.hpp"
#include "core/third/open_vcdiff.hpp"

namespace TwinStar::Core::Tool::Data::Differentiation::VCDiff {

	struct EncodeCommon {

	protected:

	};

	struct Encode :
		EncodeCommon {

	protected:

		static auto process_whole (
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
			assert_condition(state);
			state = encoder.StartEncoding(&patch_container);
			assert_condition(state);
			state = encoder.EncodeChunk(cast_pointer<char>(as_variable_pointer(after.current_pointer())).value, after.reserve().value, &patch_container);
			assert_condition(state);
			state = encoder.FinishEncoding(&patch_container);
			assert_condition(state);
			before.forward(before.reserve());
			after.forward(after.reserve());
			for (auto & element : patch_container) {
				patch.write(self_cast<Byte>(element));
			}
			return;
		}

	public:

		static auto do_process_whole (
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

	struct Decode :
		EncodeCommon {

	protected:

		static auto process_whole (
			IByteStreamView & before,
			OByteStreamView & after,
			IByteStreamView & patch,
			Size const &      maximum_window_size
		) -> Void {
			auto state = bool{};
			auto after_container = std::string{};
			auto decoder = Third::open_vcdiff::VCDiffStreamingDecoder{};
			decoder.SetAllowVcdTarget(true);
			decoder.SetMaximumTargetFileSize(INT32_MAX);
			decoder.SetMaximumTargetWindowSize(maximum_window_size.value);
			decoder.StartDecoding(cast_pointer<char>(as_variable_pointer(before.current_pointer())).value, before.reserve().value);
			state = decoder.DecodeChunk(cast_pointer<char>(as_variable_pointer(patch.current_pointer())).value, patch.reserve().value, &after_container);
			assert_condition(state);
			state = decoder.FinishDecoding();
			assert_condition(state);
			before.forward(before.reserve());
			patch.forward(patch.reserve());
			for (auto & element : after_container) {
				after.write(self_cast<Byte>(element));
			}
			return;
		}

	public:

		static auto do_process_whole (
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
