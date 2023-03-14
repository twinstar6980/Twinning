#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/trail/version.hpp"
#include "core/tool/popcap/trail/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::Trail {

	template <auto version> requires (check_version(version, {}, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = [] {
			if constexpr (version.platform == VersionPlatform::Constant::desktop() && version.variant_64 == k_false) {
				return MagicIdentifier{0xAB8B62B3_iu32};
			}
			if constexpr (version.platform == VersionPlatform::Constant::mobile() && version.variant_64 == k_false) {
				return MagicIdentifier{0xAB8B62B3_iu32};
			}
			if constexpr (version.platform == VersionPlatform::Constant::mobile() && version.variant_64 == k_true) {
				return MagicIdentifier{0x8488BC08_iu32};
			}
			if constexpr (version.platform == VersionPlatform::Constant::television() && version.variant_64 == k_false) {
				return MagicIdentifier{0xAB8B62B3_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;

	};

	template <auto version> requires (check_version(version, {}, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		// ----------------

		static auto exchange_integer_platform (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerOfPlatform>(value));
			return;
		}

		static auto exchange_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU32>(value));
			return;
		}

		static auto exchange_floating (
			OByteStreamView & data,
			Floating const &  value
		) -> Void {
			data.write(cbw<Floating32>(value));
			return;
		}

		static auto exchange_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			data.write(self_cast<StringBlock32>(value));
			return;
		}

		static auto exchange_integer_constant (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write_constant(cbw<IntegerU32>(value));
			return;
		}

		// ----------------

		static auto process_track_node_list (
			OByteStreamView &                          track_node_list_data,
			List<typename Manifest::TrackNode> const & track_node_list_manifest
		) -> Void {
			exchange_integer(track_node_list_data, cbw<Integer>(track_node_list_manifest.size()));
			for (auto & track_node_manifest : track_node_list_manifest) {
				exchange_floating(track_node_list_data, track_node_manifest.time);
				exchange_floating(track_node_list_data, track_node_manifest.low_value);
				exchange_floating(track_node_list_data, track_node_manifest.high_value);
				exchange_integer(track_node_list_data, track_node_manifest.curve);
				exchange_integer(track_node_list_data, track_node_manifest.distribution);
			}
			return;
		}

		static auto process_trail (
			OByteStreamView &                trail_data,
			typename Manifest::Trail const & trail_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			trail_data.write_constant(k_magic_identifier);
			exchange_integer_platform(trail_data, ignored);
			exchange_integer(trail_data, trail_manifest.maximum_point);
			exchange_floating(trail_data, trail_manifest.minimum_point_distance);
			exchange_integer(trail_data, trail_manifest.flag);
			if constexpr (version.variant_64) {
				exchange_integer(trail_data, ignored);
			}
			for (auto & index : SizeRange{5_sz}) {
				exchange_integer_platform(trail_data, ignored);
				exchange_integer_platform(trail_data, ignored);
			}
			if constexpr (version.platform == VersionPlatform::Constant::desktop() || version.platform == VersionPlatform::Constant::television()) {
				exchange_string(trail_data, trail_manifest.image);
			}
			if constexpr (version.platform == VersionPlatform::Constant::mobile()) {
				exchange_integer(trail_data, trail_manifest.image);
			}
			if constexpr (version.platform == VersionPlatform::Constant::television()) {
				exchange_string(trail_data, trail_manifest.image_resource);
			}
			process_track_node_list(trail_data, trail_manifest.width_over_length);
			process_track_node_list(trail_data, trail_manifest.width_over_time);
			process_track_node_list(trail_data, trail_manifest.alpha_over_length);
			process_track_node_list(trail_data, trail_manifest.alpha_over_time);
			process_track_node_list(trail_data, trail_manifest.duration);
			return;
		}

	public:

		static auto do_process_trail (
			OByteStreamView &                trail_data_,
			typename Manifest::Trail const & trail_manifest
		) -> Void {
			M_use_zps_of(trail_data);
			return process_trail(trail_data, trail_manifest);
		}

	};

	template <auto version> requires (check_version(version, {}, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::IntegerOfPlatform;

		// ----------------

		static auto exchange_integer_platform (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerOfPlatform>());
			return;
		}

		static auto exchange_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU32>());
			return;
		}

		static auto exchange_floating (
			IByteStreamView & data,
			Floating &        value
		) -> Void {
			value = cbw<Floating>(data.read_of<Floating32>());
			return;
		}

		static auto exchange_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			data.read(self_cast<StringBlock32>(value));
			return;
		}

		static auto exchange_integer_constant (
			IByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.read_constant(cbw<IntegerU32>(value));
			return;
		}

		// ----------------

		static auto process_track_node_list (
			IByteStreamView &                    track_node_list_data,
			List<typename Manifest::TrackNode> & track_node_list_manifest
		) -> Void {
			auto track_node_count = Integer{};
			exchange_integer(track_node_list_data, track_node_count);
			track_node_list_manifest.allocate_full(cbw<Size>(track_node_count));
			for (auto & track_node_manifest : track_node_list_manifest) {
				exchange_floating(track_node_list_data, track_node_manifest.time);
				exchange_floating(track_node_list_data, track_node_manifest.low_value);
				exchange_floating(track_node_list_data, track_node_manifest.high_value);
				exchange_integer(track_node_list_data, track_node_manifest.curve);
				exchange_integer(track_node_list_data, track_node_manifest.distribution);
			}
			return;
		}

		static auto process_trail (
			IByteStreamView &          trail_data,
			typename Manifest::Trail & trail_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			trail_data.read_constant(k_magic_identifier);
			exchange_integer_platform(trail_data, ignored);
			exchange_integer(trail_data, trail_manifest.maximum_point);
			exchange_floating(trail_data, trail_manifest.minimum_point_distance);
			exchange_integer(trail_data, trail_manifest.flag);
			if constexpr (version.variant_64) {
				exchange_integer(trail_data, ignored);
			}
			for (auto & index : SizeRange{5_sz}) {
				exchange_integer_platform(trail_data, ignored);
				exchange_integer_platform(trail_data, ignored);
			}
			if constexpr (version.platform == VersionPlatform::Constant::desktop() || version.platform == VersionPlatform::Constant::television()) {
				exchange_string(trail_data, trail_manifest.image);
			}
			if constexpr (version.platform == VersionPlatform::Constant::mobile()) {
				exchange_integer(trail_data, trail_manifest.image);
			}
			if constexpr (version.platform == VersionPlatform::Constant::television()) {
				exchange_string(trail_data, trail_manifest.image_resource);
			}
			process_track_node_list(trail_data, trail_manifest.width_over_length);
			process_track_node_list(trail_data, trail_manifest.width_over_time);
			process_track_node_list(trail_data, trail_manifest.alpha_over_length);
			process_track_node_list(trail_data, trail_manifest.alpha_over_time);
			process_track_node_list(trail_data, trail_manifest.duration);
			return;
		}

	public:

		static auto do_process_trail (
			IByteStreamView &          trail_data_,
			typename Manifest::Trail & trail_manifest
		) -> Void {
			M_use_zps_of(trail_data);
			restruct(trail_manifest);
			return process_trail(trail_data, trail_manifest);
		}

	};

}
