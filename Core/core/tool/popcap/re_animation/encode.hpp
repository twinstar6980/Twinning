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
			if constexpr (check_version(version, {VersionPlatform::Constant::desktop()}, {false})) {
				return MagicIdentifier{0xB393B4C0_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {false})) {
				return MagicIdentifier{0xFF2565B5_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {true})) {
				return MagicIdentifier{0xC046E570_iu32};
			}
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {false})) {
				return MagicIdentifier{0x00000000_iu32};
			}
		}();

		// ----------------

		using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;

		// ----------------

		inline static constexpr auto k_transform_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<FloaterS32>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			return size;
		}();

		inline static constexpr auto k_track_data_size = [] {
			auto size = k_none_size;
			size += bs_static_size<IntegerOfPlatform>();
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (check_version(version, {VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {})) {
				size += bs_static_size<IntegerOfPlatform>();
			}
			size += bs_static_size<IntegerOfPlatform>();
			if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
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

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			OByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.write_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU32>(value));
			return;
		}

		static auto exchange_unit_integer_platform (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerOfPlatform>(value));
			return;
		}

		static auto exchange_unit_floater (
			OByteStreamView & data,
			Floater const &   value
		) -> Void {
			data.write(cbw<FloaterS32>(value));
			return;
		}

		static auto exchange_unit_string (
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
			auto ignored = Integer{0_i};
			animation_data.write_constant(k_magic_identifier);
			exchange_unit_integer_platform(animation_data, ignored);
			exchange_unit_integer(animation_data, cbw<Integer>(animation_manifest.track.size()));
			exchange_unit_floater(animation_data, animation_manifest.frame_rate);
			exchange_unit_integer_platform(animation_data, ignored);
			exchange_unit_constant(animation_data, cbw<IntegerU32>(k_track_data_size));
			for (auto & track_manifest : animation_manifest.track) {
				exchange_unit_integer_platform(animation_data, ignored);
				exchange_unit_integer_platform(animation_data, ignored);
				if constexpr (check_version(version, {VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {})) {
					exchange_unit_integer_platform(animation_data, ignored);
				}
				exchange_unit_integer_platform(animation_data, cbw<Integer>(track_manifest.transform.size()));
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_unit_integer_platform(animation_data, ignored);
				}
			}
			for (auto & track_manifest : animation_manifest.track) {
				exchange_unit_string(animation_data, track_manifest.name);
				exchange_unit_constant(animation_data, cbw<IntegerU32>(k_transform_data_size));
				for (auto & transform_manifest : track_manifest.transform) {
					exchange_unit_floater(animation_data, transform_manifest.x);
					exchange_unit_floater(animation_data, transform_manifest.y);
					exchange_unit_floater(animation_data, transform_manifest.kx);
					exchange_unit_floater(animation_data, transform_manifest.ky);
					exchange_unit_floater(animation_data, transform_manifest.sx);
					exchange_unit_floater(animation_data, transform_manifest.sy);
					exchange_unit_floater(animation_data, transform_manifest.f);
					exchange_unit_floater(animation_data, transform_manifest.a);
					exchange_unit_integer_platform(animation_data, ignored);
					exchange_unit_integer_platform(animation_data, ignored);
					exchange_unit_integer_platform(animation_data, ignored);
					if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
						exchange_unit_integer_platform(animation_data, ignored);
					}
				}
				for (auto & transform_manifest : track_manifest.transform) {
					if constexpr (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::television()}, {})) {
						exchange_unit_string(animation_data, transform_manifest.image);
					}
					if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {})) {
						exchange_unit_integer(animation_data, transform_manifest.image);
					}
					if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
						exchange_unit_string(animation_data, transform_manifest.image_path);
						exchange_unit_string(animation_data, transform_manifest.image_another);
						exchange_unit_string(animation_data, transform_manifest.image_path_another);
					}
					exchange_unit_string(animation_data, transform_manifest.font);
					exchange_unit_string(animation_data, transform_manifest.text);
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

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			IByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.read_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU32>());
			return;
		}

		static auto exchange_unit_integer_platform (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerOfPlatform>());
			return;
		}

		static auto exchange_unit_floater (
			IByteStreamView & data,
			Floater &         value
		) -> Void {
			value = cbw<Floater>(data.read_of<FloaterS32>());
			return;
		}

		static auto exchange_unit_string (
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
			auto ignored = Integer{0_i};
			animation_data.read_constant(k_magic_identifier);
			exchange_unit_integer_platform(animation_data, ignored);
			animation_manifest.track.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(animation_data, it); }))));
			exchange_unit_floater(animation_data, animation_manifest.frame_rate);
			exchange_unit_integer_platform(animation_data, ignored);
			exchange_unit_constant(animation_data, cbw<IntegerU32>(k_track_data_size));
			for (auto & track_manifest : animation_manifest.track) {
				exchange_unit_integer_platform(animation_data, ignored);
				exchange_unit_integer_platform(animation_data, ignored);
				if constexpr (check_version(version, {VersionPlatform::Constant::mobile(), VersionPlatform::Constant::television()}, {})) {
					exchange_unit_integer_platform(animation_data, ignored);
				}
				track_manifest.transform.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer_platform(animation_data, it); }))));
				if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
					exchange_unit_integer_platform(animation_data, ignored);
				}
			}
			for (auto & track_manifest : animation_manifest.track) {
				exchange_unit_string(animation_data, track_manifest.name);
				exchange_unit_constant(animation_data, cbw<IntegerU32>(k_transform_data_size));
				for (auto & transform_manifest : track_manifest.transform) {
					exchange_unit_floater(animation_data, transform_manifest.x);
					exchange_unit_floater(animation_data, transform_manifest.y);
					exchange_unit_floater(animation_data, transform_manifest.kx);
					exchange_unit_floater(animation_data, transform_manifest.ky);
					exchange_unit_floater(animation_data, transform_manifest.sx);
					exchange_unit_floater(animation_data, transform_manifest.sy);
					exchange_unit_floater(animation_data, transform_manifest.f);
					exchange_unit_floater(animation_data, transform_manifest.a);
					exchange_unit_integer_platform(animation_data, ignored);
					exchange_unit_integer_platform(animation_data, ignored);
					exchange_unit_integer_platform(animation_data, ignored);
					if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
						exchange_unit_integer_platform(animation_data, ignored);
					}
				}
				for (auto & transform_manifest : track_manifest.transform) {
					if constexpr (check_version(version, {VersionPlatform::Constant::desktop(), VersionPlatform::Constant::television()}, {})) {
						exchange_unit_string(animation_data, transform_manifest.image);
					}
					if constexpr (check_version(version, {VersionPlatform::Constant::mobile()}, {})) {
						exchange_unit_integer(animation_data, transform_manifest.image);
					}
					if constexpr (check_version(version, {VersionPlatform::Constant::television()}, {})) {
						exchange_unit_string(animation_data, transform_manifest.image_path);
						exchange_unit_string(animation_data, transform_manifest.image_another);
						exchange_unit_string(animation_data, transform_manifest.image_path_another);
					}
					exchange_unit_string(animation_data, transform_manifest.font);
					exchange_unit_string(animation_data, transform_manifest.text);
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
