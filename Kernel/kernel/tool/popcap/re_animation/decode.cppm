module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.re_animation.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.re_animation.version;
import twinning.kernel.tool.popcap.re_animation.definition;
import twinning.kernel.tool.popcap.re_animation.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::ReAnimation {

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		Common<version>,
		CommonByteStreamExchangeForInput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		using Common::k_transform_data_size;

		using Common::k_track_data_size;

		// ----------------

		inline static auto exchange_transform_list (
			IByteStreamView &                      data,
			List<typename Definition::Transform> & value_list
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_raw_constant(data, cbox<IntegerU32>(k_transform_data_size));
			for (auto & value : value_list) {
				exchange_floater_fixed<FloaterS32>(data, value.x);
				exchange_floater_fixed<FloaterS32>(data, value.y);
				exchange_floater_fixed<FloaterS32>(data, value.kx);
				exchange_floater_fixed<FloaterS32>(data, value.ky);
				exchange_floater_fixed<FloaterS32>(data, value.sx);
				exchange_floater_fixed<FloaterS32>(data, value.sy);
				exchange_floater_fixed<FloaterS32>(data, value.f);
				exchange_floater_fixed<FloaterS32>(data, value.a);
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				}
			}
			for (auto & value : value_list) {
				if constexpr (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::television()}, {})) {
					exchange_string_block<IntegerU32>(data, value.image);
				}
				if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {})) {
					exchange_integer_fixed<IntegerU32>(data, value.image);
				}
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_string_block<IntegerU32>(data, value.image_path);
					exchange_string_block<IntegerU32>(data, value.image_another);
					exchange_string_block<IntegerU32>(data, value.image_path_another);
				}
				exchange_string_block<IntegerU32>(data, value.font);
				exchange_string_block<IntegerU32>(data, value.text);
			}
			return;
		}

		inline static auto exchange_track_list (
			IByteStreamView &                  data,
			List<typename Definition::Track> & value_list
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_raw_constant(data, cbox<IntegerU32>(k_track_data_size));
			for (auto & value : value_list) {
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				if constexpr (check_version(version, {VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {})) {
					exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				}
				exchange_list_size(data, value.transform, &exchange_size_fixed<IntegerOfPlatform>);
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				}
			}
			for (auto & value : value_list) {
				exchange_string_block<IntegerU32>(data, value.name);
				exchange_transform_list(data, value.transform);
			}
			return;
		}

		inline static auto exchange_animation (
			IByteStreamView &                data,
			typename Definition::Animation & value
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
			exchange_list_size(data, value.track, &exchange_size_fixed<IntegerU32>);
			exchange_floater_fixed<FloaterS32>(data, value.frame_rate);
			exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
			exchange_track_list(data, value.track);
			return;
		}

		// ----------------

		inline static auto process_whole (
			IByteStreamView &                data,
			typename Definition::Animation & definition
		) -> Void {
			data.read_constant(k_magic_identifier);
			exchange_animation(data, definition);
			return;
		}

		// ----------------

		inline static auto process (
			IByteStreamView &                data_,
			typename Definition::Animation & definition
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_whole(data, definition);
		}

	};

}
