#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/re_animation/version.hpp"
#include "core/tool/popcap/re_animation/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::ReAnimation {

	template <auto version> requires (check_version(version, {}, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = [] {
			if constexpr (version.platform == VersionPlatform::Constant::desktop() && version.variant_64 == k_false) {
				return MagicIdentifier{0xB393B4C0_iu32};
			}
			if constexpr (version.platform == VersionPlatform::Constant::mobile() && version.variant_64 == k_false) {
				return MagicIdentifier{0xFF2565B5_iu32};
			}
			if constexpr (version.platform == VersionPlatform::Constant::mobile() && version.variant_64 == k_true) {
				return MagicIdentifier{0xC046E570_iu32};
			}
			if constexpr (version.platform == VersionPlatform::Constant::television() && version.variant_64 == k_false) {
				return MagicIdentifier{0x00000000_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;

		// ----------------

		inline static constexpr auto k_transform_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<Floating32>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (version.platform == VersionPlatform::Constant::television()) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			return size;
		}();

		inline static constexpr auto k_track_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (version.platform == VersionPlatform::Constant::mobile() || version.platform == VersionPlatform::Constant::television()) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (version.platform == VersionPlatform::Constant::television()) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			return size;
		}();

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

		using Common::k_transform_data_size;

		using Common::k_track_data_size;

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

		static auto process_animation (
			OByteStreamView &                    animation_data,
			typename Manifest::Animation const & animation_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			animation_data.write_constant(k_magic_identifier);
			exchange_integer_platform(animation_data, ignored);
			exchange_integer(animation_data, cbw<Integer>(animation_manifest.track.size()));
			exchange_floating(animation_data, animation_manifest.frame_rate);
			exchange_integer_platform(animation_data, ignored);
			exchange_integer_constant(animation_data, cbw<Integer>(k_track_data_size));
			for (auto & track_manifest : animation_manifest.track) {
				exchange_integer_platform(animation_data, ignored);
				exchange_integer_platform(animation_data, ignored);
				if constexpr (version.platform == VersionPlatform::Constant::mobile() || version.platform == VersionPlatform::Constant::television()) {
					exchange_integer_platform(animation_data, ignored);
				}
				exchange_integer_platform(animation_data, cbw<Integer>(track_manifest.transform.size()));
				if constexpr (version.platform == VersionPlatform::Constant::television()) {
					exchange_integer_platform(animation_data, ignored);
				}
			}
			for (auto & track_manifest : animation_manifest.track) {
				exchange_string(animation_data, track_manifest.name);
				exchange_integer_constant(animation_data, cbw<Integer>(k_transform_data_size));
				for (auto & transform_manifest : track_manifest.transform) {
					exchange_floating(animation_data, transform_manifest.x);
					exchange_floating(animation_data, transform_manifest.y);
					exchange_floating(animation_data, transform_manifest.kx);
					exchange_floating(animation_data, transform_manifest.ky);
					exchange_floating(animation_data, transform_manifest.sx);
					exchange_floating(animation_data, transform_manifest.sy);
					exchange_floating(animation_data, transform_manifest.f);
					exchange_floating(animation_data, transform_manifest.a);
					exchange_integer_platform(animation_data, ignored);
					exchange_integer_platform(animation_data, ignored);
					exchange_integer_platform(animation_data, ignored);
					if constexpr (version.platform == VersionPlatform::Constant::television()) {
						exchange_integer_platform(animation_data, ignored);
					}
				}
				for (auto & transform_manifest : track_manifest.transform) {
					if constexpr (version.platform == VersionPlatform::Constant::desktop() || version.platform == VersionPlatform::Constant::television()) {
						exchange_string(animation_data, transform_manifest.image);
					}
					if constexpr (version.platform == VersionPlatform::Constant::mobile()) {
						exchange_integer(animation_data, transform_manifest.image);
					}
					if constexpr (version.platform == VersionPlatform::Constant::television()) {
						exchange_string(animation_data, transform_manifest.image_path);
						exchange_string(animation_data, transform_manifest.image_another);
						exchange_string(animation_data, transform_manifest.image_path_another);
					}
					exchange_string(animation_data, transform_manifest.font);
					exchange_string(animation_data, transform_manifest.text);
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

		using Common::k_transform_data_size;

		using Common::k_track_data_size;

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

		static auto process_animation (
			IByteStreamView &              animation_data,
			typename Manifest::Animation & animation_manifest
		) -> Void {
			auto ignored = Integer{0_i};
			animation_data.read_constant(k_magic_identifier);
			exchange_integer_platform(animation_data, ignored);
			auto track_count = Integer{};
			exchange_integer(animation_data, track_count);
			animation_manifest.track.allocate_full(cbw<Size>(track_count));
			exchange_floating(animation_data, animation_manifest.frame_rate);
			exchange_integer_platform(animation_data, ignored);
			exchange_integer_constant(animation_data, cbw<Integer>(k_track_data_size));
			for (auto & track_manifest : animation_manifest.track) {
				exchange_integer_platform(animation_data, ignored);
				exchange_integer_platform(animation_data, ignored);
				if constexpr (version.platform == VersionPlatform::Constant::mobile() || version.platform == VersionPlatform::Constant::television()) {
					exchange_integer_platform(animation_data, ignored);
				}
				auto transform_count = Integer{};
				exchange_integer_platform(animation_data, transform_count);
				track_manifest.transform.allocate_full(cbw<Size>(transform_count));
				if constexpr (version.platform == VersionPlatform::Constant::television()) {
					exchange_integer_platform(animation_data, ignored);
				}
			}
			for (auto & track_manifest : animation_manifest.track) {
				exchange_string(animation_data, track_manifest.name);
				exchange_integer_constant(animation_data, cbw<Integer>(k_transform_data_size));
				for (auto & transform_manifest : track_manifest.transform) {
					exchange_floating(animation_data, transform_manifest.x);
					exchange_floating(animation_data, transform_manifest.y);
					exchange_floating(animation_data, transform_manifest.kx);
					exchange_floating(animation_data, transform_manifest.ky);
					exchange_floating(animation_data, transform_manifest.sx);
					exchange_floating(animation_data, transform_manifest.sy);
					exchange_floating(animation_data, transform_manifest.f);
					exchange_floating(animation_data, transform_manifest.a);
					exchange_integer_platform(animation_data, ignored);
					exchange_integer_platform(animation_data, ignored);
					exchange_integer_platform(animation_data, ignored);
					if constexpr (version.platform == VersionPlatform::Constant::television()) {
						exchange_integer_platform(animation_data, ignored);
					}
				}
				for (auto & transform_manifest : track_manifest.transform) {
					if constexpr (version.platform == VersionPlatform::Constant::desktop() || version.platform == VersionPlatform::Constant::television()) {
						exchange_string(animation_data, transform_manifest.image);
					}
					if constexpr (version.platform == VersionPlatform::Constant::mobile()) {
						exchange_integer(animation_data, transform_manifest.image);
					}
					if constexpr (version.platform == VersionPlatform::Constant::television()) {
						exchange_string(animation_data, transform_manifest.image_path);
						exchange_string(animation_data, transform_manifest.image_another);
						exchange_string(animation_data, transform_manifest.image_path_another);
					}
					exchange_string(animation_data, transform_manifest.font);
					exchange_string(animation_data, transform_manifest.text);
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
