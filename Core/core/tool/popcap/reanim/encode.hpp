#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/reanim/version.hpp"
#include "core/tool/popcap/reanim/manifest.hpp"
#include "core/tool/popcap/reanim/structure.hpp"

namespace TwinKleS::Core::Tool::PopCap::Reanim {

	inline namespace CommonOfEncode {

		namespace Detail {

		}

	}

	namespace Encode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_animation (
				OByteStreamView &                    data,
				Manifest::Animation<version> const & manifest
			) -> Void {
				using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;
				if constexpr (version.platform == Version::Platform::desktop) {
					data.write(Structure::k_magic_identifier_desktop_32);
				}
				if constexpr (version.platform == Version::Platform::phone) {
					data.write(!version.variant_64 ? (Structure::k_magic_identifier_phone_32) : (Structure::k_magic_identifier_phone_64));
				}
				data.write(IntegerOfPlatform{});
				data.write(cbw<IntegerU32>(manifest.track.size()));
				data.write(cbw<Floating32>(manifest.fps));
				data.write(IntegerOfPlatform{});
				data.write(cbw<IntegerU32>(bs_static_size<IntegerOfPlatform>() + bs_static_size<IntegerOfPlatform>() + (version.platform == Version::Platform::phone ? (bs_static_size<IntegerOfPlatform>()) : (k_none_size)) + bs_static_size<IntegerOfPlatform>()));
				for (auto & track_manifest : manifest.track) {
					data.write(IntegerOfPlatform{});
					data.write(IntegerOfPlatform{});
					if constexpr (version.platform == Version::Platform::phone) {
						data.write(IntegerOfPlatform{});
					}
					data.write(cbw<IntegerOfPlatform>(track_manifest.t.size()));
				}
				for (auto & track_manifest : manifest.track) {
					data.write(self_cast<StringBlock32>(track_manifest.name));
					data.write(cbw<IntegerU32>(bs_static_size<Floating32>() * 8_sz + bs_static_size<IntegerOfPlatform>() * 3_sz));
					for (auto & t_manifest : track_manifest.t) {
						data.write(cbw<Floating32>(t_manifest.x));
						data.write(cbw<Floating32>(t_manifest.y));
						data.write(cbw<Floating32>(t_manifest.kx));
						data.write(cbw<Floating32>(t_manifest.ky));
						data.write(cbw<Floating32>(t_manifest.sx));
						data.write(cbw<Floating32>(t_manifest.sy));
						data.write(cbw<Floating32>(t_manifest.f));
						data.write(cbw<Floating32>(t_manifest.a));
						data.write(IntegerOfPlatform{});
						data.write(IntegerOfPlatform{});
						data.write(IntegerOfPlatform{});
					}
					for (auto & t_manifest : track_manifest.t) {
						if constexpr (version.platform == Version::Platform::desktop) {
							data.write(self_cast<StringBlock32>(t_manifest.i));
						}
						if constexpr (version.platform == Version::Platform::phone) {
							data.write(cbw<IntegerU32>(t_manifest.i));
						}
						data.write(self_cast<StringBlock32>(t_manifest.font));
						data.write(self_cast<StringBlock32>(t_manifest.text));
					}
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				OByteStreamView &                    data,
				Manifest::Animation<version> const & manifest
			) -> Void {
				process_animation<version>(data, manifest);
				return;
			}

			inline auto process (
				OByteStreamView &                  data,
				Manifest::AnimationVariant const & manifest,
				Version const &                    version
			) -> Void {
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.get<Manifest::Animation<version>>());
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Decode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_animation (
				IByteStreamView &              data,
				Manifest::Animation<version> & manifest
			) -> Void {
				using IntegerOfPlatform = AsSwitch<version.variant_64.value, IntegerU64, IntegerU32>;
				if constexpr (version.platform == Version::Platform::desktop) {
					assert_condition(data.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier_desktop_32);
				}
				if constexpr (version.platform == Version::Platform::phone) {
					assert_condition(data.read_of<Structure::MagicIdentifier>() == (!version.variant_64 ? (Structure::k_magic_identifier_phone_32) : (Structure::k_magic_identifier_phone_64)));
				}
				data.read_of<IntegerOfPlatform>();
				manifest.track.allocate_full(cbw<Size>(data.read_of<IntegerU32>()));
				manifest.fps = cbw<Floating>(data.read_of<Floating32>());
				data.read_of<IntegerOfPlatform>();
				assert_condition(data.read_of<IntegerU32>() == cbw<IntegerU32>(bs_static_size<IntegerOfPlatform>() + bs_static_size<IntegerOfPlatform>() + (version.platform == Version::Platform::phone ? (bs_static_size<IntegerOfPlatform>()) : (k_none_size)) + bs_static_size<IntegerOfPlatform>()));
				for (auto & track_manifest : manifest.track) {
					data.read_of<IntegerOfPlatform>();
					data.read_of<IntegerOfPlatform>();
					if constexpr (version.platform == Version::Platform::phone) {
						data.read_of<IntegerOfPlatform>();
					}
					track_manifest.t.allocate_full(cbw<Size>(data.read_of<IntegerOfPlatform>()));
				}
				for (auto & track_manifest : manifest.track) {
					track_manifest.name = data.read_of<StringBlock32>().value;
					assert_condition(data.read_of<IntegerU32>() == cbw<IntegerU32>(bs_static_size<Floating32>() * 8_sz + bs_static_size<IntegerOfPlatform>() * 3_sz));
					for (auto & t_manifest : track_manifest.t) {
						t_manifest.x = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.y = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.kx = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.ky = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.sx = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.sy = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.f = cbw<Floating>(data.read_of<Floating32>());
						t_manifest.a = cbw<Floating>(data.read_of<Floating32>());
						data.read_of<IntegerOfPlatform>();
						data.read_of<IntegerOfPlatform>();
						data.read_of<IntegerOfPlatform>();
					}
					for (auto & t_manifest : track_manifest.t) {
						if constexpr (version.platform == Version::Platform::desktop) {
							t_manifest.i = data.read_of<StringBlock32>().value;
						}
						if constexpr (version.platform == Version::Platform::phone) {
							t_manifest.i = cbw<Integer>(data.read_of<IntegerU32>());
						}
						t_manifest.font = data.read_of<StringBlock32>().value;
						t_manifest.text = data.read_of<StringBlock32>().value;
					}
				}
				return;
			}

			// ----------------

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process (
				IByteStreamView &              data,
				Manifest::Animation<version> & manifest
			) -> Void {
				process_animation<version>(data, manifest);
				return;
			}

			inline auto process (
				IByteStreamView &            data,
				Manifest::AnimationVariant & manifest,
				Version const &              version
			) -> Void {
				Generalization::execute<VersionEnum>(
					version,
					[&] <auto index, auto version> (ValuePackage<index, version>) {
						process<version>(data, manifest.set<Manifest::Animation<version>>());
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
