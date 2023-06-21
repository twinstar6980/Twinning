#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/trail/common.hpp"
#include "core/tool/common/byte_stream.hpp"

namespace TwinStar::Core::Tool::PopCap::Trail {

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		Common<version>,
		CommonByteStreamExchangeForInput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		// ----------------

		static auto exchange_track_node_list (
			IByteStreamView &                      data,
			List<typename Definition::TrackNode> & value_list
		) -> Void {
			exchange_list_size(data, value_list, &exchange_size_fixed<IntegerU32>);
			for (auto & value : value_list) {
				exchange_floater_fixed<FloaterS32>(data, value.time);
				exchange_floater_fixed<FloaterS32>(data, value.low_value);
				exchange_floater_fixed<FloaterS32>(data, value.high_value);
				exchange_integer_fixed<IntegerU32>(data, value.curve);
				exchange_integer_fixed<IntegerU32>(data, value.distribution);
			}
			return;
		}

		static auto exchange_trail (
			IByteStreamView &            data,
			typename Definition::Trail & value
		) -> Void {
			auto ignored = Integer{0_i};
			exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
			exchange_integer_fixed<IntegerU32>(data, value.maximum_point);
			exchange_floater_fixed<FloaterS32>(data, value.minimum_point_distance);
			exchange_integer_fixed<IntegerU32>(data, value.flag);
			if constexpr (check_version(version, {}, {true})) {
				exchange_integer_fixed<IntegerU32>(data, ignored);
			}
			for (auto & index : SizeRange{5_sz}) {
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
				exchange_integer_fixed<IntegerOfPlatform>(data, ignored);
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::television()}, {})) {
				exchange_string_block<IntegerU32>(data, value.image);
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {})) {
				exchange_integer_fixed<IntegerU32>(data, value.image);
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
				exchange_string_block<IntegerU32>(data, value.image_resource);
			}
			exchange_track_node_list(data, value.width_over_length);
			exchange_track_node_list(data, value.width_over_time);
			exchange_track_node_list(data, value.alpha_over_length);
			exchange_track_node_list(data, value.alpha_over_time);
			exchange_track_node_list(data, value.duration);
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView &            data,
			typename Definition::Trail & value
		) -> Void {
			data.read_constant(k_magic_identifier);
			exchange_trail(data, value);
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView &            data_,
			typename Definition::Trail & value
		) -> Void {
			M_use_zps_of(data);
			restruct(value);
			return process_whole(data, value);
		}

	};

}
