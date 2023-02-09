#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/reanim/version.hpp"
#include "core/tool/popcap/reanim/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::Reanim {

	template <auto version> requires (check_version(version, {}, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = [] {
			if constexpr (check_version(version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicIdentifier{0xB393B4C0_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::phone()}, {false})) {
				return MagicIdentifier{0xFF2565B5_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::phone()}, {true})) {
				return MagicIdentifier{0xC046E570_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;

		// ----------------

		inline static constexpr auto k_track_data_size = Size{bs_static_size<IntegerOfPlatform>() + bs_static_size<IntegerOfPlatform>() + (version.platform == VersionPlatform::Constant::phone() ? (bs_static_size<IntegerOfPlatform>()) : (k_none_size)) + bs_static_size<IntegerOfPlatform>()};

		inline static constexpr auto k_t_data_size = Size{bs_static_size<Floating32>() * 8_sz + bs_static_size<IntegerOfPlatform>() * 3_sz};

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

		using Common::k_track_data_size;

		using Common::k_t_data_size;

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

		// ----------------

		static auto process_animation (
			OByteStreamView &                    animation_data,
			typename Manifest::Animation const & animation_manifest
		) -> Void {
			auto integer_of_platform = Integer{};
			animation_data.write_constant(k_magic_identifier);
			exchange_integer_platform(animation_data, integer_of_platform);
			exchange_integer(animation_data, cbw<Integer>(animation_manifest.track.size()));
			exchange_floating(animation_data, animation_manifest.fps);
			exchange_integer_platform(animation_data, integer_of_platform);
			exchange_integer(animation_data, cbw<Integer>(k_track_data_size));
			for (auto & track_manifest : animation_manifest.track) {
				exchange_integer_platform(animation_data, integer_of_platform);
				exchange_integer_platform(animation_data, integer_of_platform);
				if constexpr (version.platform == VersionPlatform::Constant::phone()) {
					exchange_integer_platform(animation_data, integer_of_platform);
				}
				exchange_integer_platform(animation_data, cbw<Integer>(track_manifest.t.size()));
			}
			for (auto & track_manifest : animation_manifest.track) {
				exchange_string(animation_data, track_manifest.name);
				exchange_integer(animation_data, cbw<Integer>(k_t_data_size));
				for (auto & t_manifest : track_manifest.t) {
					exchange_floating(animation_data, t_manifest.x);
					exchange_floating(animation_data, t_manifest.y);
					exchange_floating(animation_data, t_manifest.kx);
					exchange_floating(animation_data, t_manifest.ky);
					exchange_floating(animation_data, t_manifest.sx);
					exchange_floating(animation_data, t_manifest.sy);
					exchange_floating(animation_data, t_manifest.f);
					exchange_floating(animation_data, t_manifest.a);
					exchange_integer_platform(animation_data, integer_of_platform);
					exchange_integer_platform(animation_data, integer_of_platform);
					exchange_integer_platform(animation_data, integer_of_platform);
				}
				for (auto & t_manifest : track_manifest.t) {
					if constexpr (version.platform == VersionPlatform::Constant::desktop()) {
						exchange_string(animation_data, t_manifest.i);
					}
					if constexpr (version.platform == VersionPlatform::Constant::phone()) {
						exchange_integer(animation_data, t_manifest.i);
					}
					exchange_string(animation_data, t_manifest.font);
					exchange_string(animation_data, t_manifest.text);
				}
			}
			return;
		}

	public:

		static auto do_process_animation (
			OByteStreamView &                    animation_data_,
			typename Manifest::Animation const & animation_manifest
		) -> Void {
			M_use_zps_of(animation_data);
			return process_animation(animation_data, animation_manifest);
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

		using Common::k_track_data_size;

		using Common::k_t_data_size;

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

		// ----------------

		static auto process_animation (
			IByteStreamView &              animation_data,
			typename Manifest::Animation & animation_manifest
		) -> Void {
			auto integer_of_platform = Integer{};
			animation_data.read_constant(k_magic_identifier);
			exchange_integer_platform(animation_data, integer_of_platform);
			auto track_count = Integer{};
			exchange_integer(animation_data, track_count);
			animation_manifest.track.allocate_full(cbw<Size>(track_count));
			exchange_floating(animation_data, animation_manifest.fps);
			exchange_integer_platform(animation_data, integer_of_platform);
			auto track_data_size = Integer{};
			exchange_integer(animation_data, track_data_size);
			assert_test(cbw<Size>(track_data_size) == k_track_data_size);
			for (auto & track_manifest : animation_manifest.track) {
				exchange_integer_platform(animation_data, integer_of_platform);
				exchange_integer_platform(animation_data, integer_of_platform);
				if constexpr (version.platform == VersionPlatform::Constant::phone()) {
					exchange_integer_platform(animation_data, integer_of_platform);
				}
				auto t_count = Integer{};
				exchange_integer_platform(animation_data, t_count);
				track_manifest.t.allocate_full(cbw<Size>(t_count));
			}
			for (auto & track_manifest : animation_manifest.track) {
				exchange_string(animation_data, track_manifest.name);
				auto t_data_size = Integer{};
				exchange_integer(animation_data, t_data_size);
				assert_test(cbw<Size>(t_data_size) == k_t_data_size);
				for (auto & t_manifest : track_manifest.t) {
					exchange_floating(animation_data, t_manifest.x);
					exchange_floating(animation_data, t_manifest.y);
					exchange_floating(animation_data, t_manifest.kx);
					exchange_floating(animation_data, t_manifest.ky);
					exchange_floating(animation_data, t_manifest.sx);
					exchange_floating(animation_data, t_manifest.sy);
					exchange_floating(animation_data, t_manifest.f);
					exchange_floating(animation_data, t_manifest.a);
					exchange_integer_platform(animation_data, integer_of_platform);
					exchange_integer_platform(animation_data, integer_of_platform);
					exchange_integer_platform(animation_data, integer_of_platform);
				}
				for (auto & t_manifest : track_manifest.t) {
					if constexpr (version.platform == VersionPlatform::Constant::desktop()) {
						exchange_string(animation_data, t_manifest.i);
					}
					if constexpr (version.platform == VersionPlatform::Constant::phone()) {
						exchange_integer(animation_data, t_manifest.i);
					}
					exchange_string(animation_data, t_manifest.font);
					exchange_string(animation_data, t_manifest.text);
				}
			}
			return;
		}

	public:

		static auto do_process_animation (
			IByteStreamView &              animation_data_,
			typename Manifest::Animation & animation_manifest
		) -> Void {
			M_use_zps_of(animation_data);
			restruct(animation_manifest);
			return process_animation(animation_data, animation_manifest);
		}

	};

}
