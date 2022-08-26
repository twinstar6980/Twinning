#pragma once

// NOTE :
// 在编解码PAM的一些细节上，与SexyFramework自身的逻辑有所出入。
// 对于一些应该为无符号整数的数值（列表大小、层索引），SexyFramework均使用有符号数存储，
// 加之代码本身并未判别符号位情况，因此，这导致了SexyFramework在二进制读写时出现了不合理之处：
// 对于Frame中的LayerRemove/Append/Change操作，三者都拥有一个指代层索引的index值，
// 这类index值在二进制读写时，额外携带了与之相关的flag值，SexyFramework首先读写一个short，高n位作为flag的bitset，剩余位存储index，若剩余位无法容纳index，则再读写index为int
// 因此，首先读入的short永远为非负数，对于Append/Change都是如此，因为二者都有一位以上的flag
// 但在对LayerRemove的index处理，由于Remove操作并无flag，却占用了5位作为flag，且在未对高位填0的情况下使用了>=操作符对short值进行判别，这导致的后果是，若读入的short为负数，则不会读写下一个int
// 由此导致的行为不一致应视为bug，因此，本程序不沿袭SexyFramework的处理方式，而将这类index视为无符号数，同时，也将列表大小这类数值视为无符号数，相关的操作使用的函数以_strictly作为后缀
// 此外，还有一些可能应视为无符号的值，SexyFramework也使用有符号数处理，暂时沿袭其处理方式

#include "core/utility/utility.hpp"
#include "core/tool/popcap/pam/version.hpp"
#include "core/tool/popcap/pam/manifest.hpp"
#include "core/tool/popcap/pam/structure.hpp"

namespace TwinKleS::Core::Tool::PopCap::PAM {

	inline namespace CommonOfEncode {

		namespace Detail {

			#pragma region flag

			namespace LayerRemoveFlag {
				inline constexpr auto k_count = Size{5_sz};
				inline constexpr auto unused_1 = Size{1_ix};
				inline constexpr auto unused_2 = Size{2_ix};
				inline constexpr auto unused_3 = Size{3_ix};
				inline constexpr auto unused_4 = Size{4_ix};
				inline constexpr auto unused_5 = Size{5_ix};
			}

			// ----------------

			namespace LayerAppendFlag {
				inline constexpr auto k_count = Size{5_sz};
				inline constexpr auto time_scale = Size{1_ix};
				inline constexpr auto name = Size{2_ix};
				inline constexpr auto preload_frame = Size{3_ix};
				inline constexpr auto additive = Size{4_ix};
				inline constexpr auto sprite = Size{5_ix};
			}

			// ----------------

			namespace LayerChangeFlag {
				inline constexpr auto k_count = Size{6_sz};
				inline constexpr auto sprite_frame_number = Size{1_ix};
				inline constexpr auto long_coord = Size{2_ix};
				inline constexpr auto matrix = Size{3_ix};
				inline constexpr auto color = Size{4_ix};
				inline constexpr auto rotate = Size{5_ix};
				inline constexpr auto source_rectangle = Size{6_ix};
			}

			// ----------------

			namespace FrameFlag {
				inline constexpr auto k_count = Size{8_sz};
				inline constexpr auto remove = Size{1_ix};
				inline constexpr auto append = Size{2_ix};
				inline constexpr auto change = Size{3_ix};
				inline constexpr auto label = Size{4_ix};
				inline constexpr auto stop = Size{5_ix};
				inline constexpr auto command = Size{6_ix};
				inline constexpr auto unused_7 = Size{7_ix};
				inline constexpr auto unused_8 = Size{8_ix};
			}

			#pragma endregion

			#pragma region value rate

			namespace ValueRate {
				inline constexpr auto time = Floating{65536.0_f};
				inline constexpr auto size = Floating{20.0_f};
				inline constexpr auto angle = Floating{1000.0_f};
				inline constexpr auto matrix = Floating{65536.0_f};
				inline constexpr auto matrix_exact = Floating{20.0_f * 65536.0_f};
				inline constexpr auto color = Floating{255.0_f};
			}

			#pragma endregion

		}

	}

	namespace Encode {

		namespace Detail {

			#pragma region using

			using namespace CommonOfEncode::Detail;

			#pragma endregion

			#pragma region atom

			inline auto write_boolean (
				OByteStreamView & data,
				Boolean const &   value
			) -> Void {
				data.write(value);
				return;
			}

			// ----------------

			inline auto write_integer_u8 (
				OByteStreamView & data,
				IntegerU8 const & value
			) -> Void {
				data.write(value);
				return;
			}

			inline auto write_integer_u16 (
				OByteStreamView &  data,
				IntegerU16 const & value
			) -> Void {
				data.write(value);
				return;
			}

			inline auto write_integer_s16 (
				OByteStreamView &  data,
				IntegerS16 const & value
			) -> Void {
				data.write(value);
				return;
			}

			inline auto write_integer_s32 (
				OByteStreamView &  data,
				IntegerS32 const & value
			) -> Void {
				data.write(value);
				return;
			}

			inline auto write_integer_u16_strictly (
				OByteStreamView &  data,
				IntegerU16 const & value
			) -> Void {
				data.write(value);
				return;
			}

			inline auto write_integer_u16_or_u8_strictly (
				OByteStreamView &  data,
				IntegerU16 const & value
			) -> Void {
				if (value >= cbw<IntegerU16>(~(0_iu8))) {
					data.write(~(0_iu8));
					data.write(value);
				} else {
					data.write(cbw<IntegerU8>(value));
				}
				return;
			}

			inline auto write_integer_u32_or_u16_with_flag_strictly (
				OByteStreamView &  data,
				IntegerU32 const & value,
				IntegerU16 const & flag,
				Size const &       flag_bit_count
			) -> Void {
				auto value_16_bit_count = k_type_bit_count<IntegerU16> - flag_bit_count;
				auto value_16_maximum = ~(~0_iu16 << value_16_bit_count);
				auto composite_value = flag << value_16_bit_count;
				if (value >= cbw<IntegerU32>(value_16_maximum)) {
					composite_value |= value_16_maximum;
					data.write(composite_value);
					data.write(value);
				} else {
					composite_value |= cbw<IntegerU16>(value);
					data.write(composite_value);
				}
				return;
			}

			// ----------------

			inline auto write_string_by_u16_strictly (
				OByteStreamView & data,
				String const &    value
			) -> Void {
				data.write(self_cast<StringBlock16>(value));
				return;
			}

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_frame (
				OByteStreamView &                data,
				Manifest::Frame<version> const & manifest
			) -> Void {
				auto flag = BitSet<FrameFlag::k_count.value>{};
				if (!manifest.remove.empty()) {
					flag.set(FrameFlag::remove);
				}
				if (!manifest.append.empty()) {
					flag.set(FrameFlag::append);
				}
				if (!manifest.change.empty()) {
					flag.set(FrameFlag::change);
				}
				if (manifest.label.has()) {
					flag.set(FrameFlag::label);
				}
				if (manifest.stop) {
					flag.set(FrameFlag::stop);
				}
				if (!manifest.command.empty()) {
					flag.set(FrameFlag::command);
				}
				write_integer_u8(data, flag.to_integer());
				if (flag.get(FrameFlag::remove)) {
					write_integer_u16_or_u8_strictly(data, cbw<IntegerU16>(manifest.remove.size()));
					for (auto & element : manifest.remove) {
						auto layer_flag = BitSet<LayerRemoveFlag::k_count.value>{};
						write_integer_u32_or_u16_with_flag_strictly(data, cbw<IntegerU32>(element.index), cbw<IntegerU16>(layer_flag.to_integer()), LayerRemoveFlag::k_count);
					}
				}
				if (flag.get(FrameFlag::append)) {
					write_integer_u16_or_u8_strictly(data, cbw<IntegerU16>(manifest.append.size()));
					for (auto & element : manifest.append) {
						auto layer_flag = BitSet<LayerAppendFlag::k_count.value>{};
						if (element.time_scale != 1.0_f) {
							layer_flag.set(LayerAppendFlag::time_scale);
						}
						if (element.name.has()) {
							layer_flag.set(LayerAppendFlag::name);
						}
						if (element.preload_frame != 0_i) {
							layer_flag.set(LayerAppendFlag::preload_frame);
						}
						if (element.additive) {
							layer_flag.set(LayerAppendFlag::additive);
						}
						if (element.sprite) {
							layer_flag.set(LayerAppendFlag::sprite);
						}
						write_integer_u32_or_u16_with_flag_strictly(data, cbw<IntegerU32>(element.index), cbw<IntegerU16>(layer_flag.to_integer()), LayerAppendFlag::k_count);
						if constexpr (version.number <= 5_i) {
							write_integer_u8(data, cbw<IntegerU8>(element.resource));
						} else {
							write_integer_u16_or_u8_strictly(data, cbw<IntegerU16>(element.resource));
						}
						if (layer_flag.get(LayerAppendFlag::preload_frame)) {
							write_integer_s16(data, cbw<IntegerS16>(element.preload_frame));
						}
						if (layer_flag.get(LayerAppendFlag::name)) {
							write_string_by_u16_strictly(data, element.name.get());
						}
						if (layer_flag.get(LayerAppendFlag::time_scale)) {
							write_integer_s32(data, Math::round<IntegerS32>(element.time_scale * ValueRate::time));
						}
					}
				}
				if (flag.get(FrameFlag::change)) {
					write_integer_u16_or_u8_strictly(data, cbw<IntegerU16>(manifest.change.size()));
					for (auto & element : manifest.change) {
						auto layer_flag = BitSet<LayerChangeFlag::k_count.value>{};
						if (element.sprite_frame_number.has()) {
							layer_flag.set(LayerChangeFlag::sprite_frame_number);
						}
						if (element.color.has()) {
							layer_flag.set(LayerChangeFlag::color);
						}
						if (element.source_rectangle.has()) {
							layer_flag.set(LayerChangeFlag::source_rectangle);
						}
						if (element.transform.template is<Manifest::TranslateTransform<version>>()) {
						} else if (element.transform.template is<Manifest::RotateTranslateTransform<version>>()) {
							layer_flag.set(LayerChangeFlag::rotate);
						} else if (element.transform.template is<Manifest::MatrixTranslateTransform<version>>()) {
							layer_flag.set(LayerChangeFlag::matrix);
						} else {
							throw NeverException{};
						}
						layer_flag.set(LayerChangeFlag::long_coord);
						write_integer_u32_or_u16_with_flag_strictly(data, cbw<IntegerU32>(element.index), cbw<IntegerU16>(layer_flag.to_integer()), LayerChangeFlag::k_count);
						if (element.transform.template is<Manifest::TranslateTransform<version>>()) {
							auto & transform = element.transform.template get<Manifest::TranslateTransform<version>>();
							write_integer_s32(data, Math::round<IntegerS32>(transform.x * ValueRate::size));
							write_integer_s32(data, Math::round<IntegerS32>(transform.y * ValueRate::size));
						} else if (element.transform.template is<Manifest::RotateTranslateTransform<version>>()) {
							auto & transform = element.transform.template get<Manifest::RotateTranslateTransform<version>>();
							write_integer_u16(data, Math::round<IntegerU16>(transform.angle * ValueRate::angle));
							write_integer_s32(data, Math::round<IntegerS32>(transform.x * ValueRate::size));
							write_integer_s32(data, Math::round<IntegerS32>(transform.y * ValueRate::size));
						} else if (element.transform.template is<Manifest::MatrixTranslateTransform<version>>()) {
							auto & transform = element.transform.template get<Manifest::MatrixTranslateTransform<version>>();
							write_integer_s32(data, Math::round<IntegerS32>(transform.a * ValueRate::matrix));
							write_integer_s32(data, Math::round<IntegerS32>(transform.c * ValueRate::matrix));
							write_integer_s32(data, Math::round<IntegerS32>(transform.b * ValueRate::matrix));
							write_integer_s32(data, Math::round<IntegerS32>(transform.d * ValueRate::matrix));
							write_integer_s32(data, Math::round<IntegerS32>(transform.x * ValueRate::size));
							write_integer_s32(data, Math::round<IntegerS32>(transform.y * ValueRate::size));
						} else {
							throw NeverException{};
						}
						if (layer_flag.get(LayerChangeFlag::source_rectangle)) {
							write_integer_s16(data, Math::round<IntegerS16>(element.source_rectangle.get().position.x * ValueRate::size));
							write_integer_s16(data, Math::round<IntegerS16>(element.source_rectangle.get().position.y * ValueRate::size));
							write_integer_s16(data, Math::round<IntegerS16>(element.source_rectangle.get().size.width * ValueRate::size));
							write_integer_s16(data, Math::round<IntegerS16>(element.source_rectangle.get().size.height * ValueRate::size));
						}
						if (layer_flag.get(LayerChangeFlag::color)) {
							write_integer_u8(data, Math::round<IntegerU8>(element.color.get().red * ValueRate::color));
							write_integer_u8(data, Math::round<IntegerU8>(element.color.get().green * ValueRate::color));
							write_integer_u8(data, Math::round<IntegerU8>(element.color.get().blue * ValueRate::color));
							write_integer_u8(data, Math::round<IntegerU8>(element.color.get().alpha * ValueRate::color));
						}
						if (layer_flag.get(LayerChangeFlag::sprite_frame_number)) {
							write_integer_s16(data, cbw<IntegerS16>(element.sprite_frame_number.get()));
						}
					}
				}
				if (flag.get(FrameFlag::label)) {
					write_string_by_u16_strictly(data, manifest.label.get());
				}
				if (flag.get(FrameFlag::command)) {
					write_integer_u8(data, cbw<IntegerU8>(manifest.command.size()));
					for (auto & element : manifest.command) {
						write_string_by_u16_strictly(data, element.command);
						write_string_by_u16_strictly(data, element.argument);
					}
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_sprite (
				OByteStreamView &                 data,
				Manifest::Sprite<version> const & manifest
			) -> Void {
				if constexpr (version.number >= 4_i) {
					write_string_by_u16_strictly(data, manifest.name);
					if constexpr (version.number >= 6_i) {
						data.pad(2_sz); // NOTE : unknown
					}
					write_integer_s32(data, Math::round<IntegerS32>(manifest.frame_rate * ValueRate::time));
				}
				write_integer_u16_strictly(data, cbw<IntegerU16>(manifest.frame.size()));
				if constexpr (version.number >= 5_i) {
					write_integer_s16(data, cbw<IntegerS16>(manifest.work_area.start));
					write_integer_s16(data, cbw<IntegerS16>(manifest.work_area.duration));
				}
				for (auto & element : manifest.frame) {
					process_frame<version>(data, element);
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_image (
				OByteStreamView &                data,
				Manifest::Image<version> const & manifest
			) -> Void {
				write_string_by_u16_strictly(data, manifest.name);
				if constexpr (version.number >= 4_i) {
					write_integer_s16(data, cbw<IntegerS16>(manifest.size.width));
					write_integer_s16(data, cbw<IntegerS16>(manifest.size.height));
				}
				if constexpr (version.number <= 1_i) {
					write_integer_s16(data, Math::round<IntegerS16>(manifest.transform.angle * ValueRate::angle));
				} else {
					write_integer_s32(data, Math::round<IntegerS32>(manifest.transform.a * ValueRate::matrix_exact));
					write_integer_s32(data, Math::round<IntegerS32>(manifest.transform.c * ValueRate::matrix_exact));
					write_integer_s32(data, Math::round<IntegerS32>(manifest.transform.b * ValueRate::matrix_exact));
					write_integer_s32(data, Math::round<IntegerS32>(manifest.transform.d * ValueRate::matrix_exact));
				}
				write_integer_s16(data, Math::round<IntegerS16>(manifest.transform.x * ValueRate::size));
				write_integer_s16(data, Math::round<IntegerS16>(manifest.transform.y * ValueRate::size));
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_animation (
				OByteStreamView &                    data,
				Manifest::Animation<version> const & manifest
			) -> Void {
				data.write(Structure::k_magic_identifier);
				data.write(cbw<Structure::VersionNumber>(version.number));
				write_integer_u8(data, cbw<IntegerU8>(manifest.frame_rate));
				write_integer_s16(data, Math::round<IntegerS16>(manifest.position.x * ValueRate::size));
				write_integer_s16(data, Math::round<IntegerS16>(manifest.position.y * ValueRate::size));
				write_integer_u16(data, Math::round<IntegerU16>(manifest.size.width * ValueRate::size));
				write_integer_u16(data, Math::round<IntegerU16>(manifest.size.height * ValueRate::size));
				write_integer_u16_strictly(data, cbw<IntegerU16>(manifest.image.size()));
				for (auto & element : manifest.image) {
					process_image<version>(data, element);
				}
				write_integer_u16_strictly(data, cbw<IntegerU16>(manifest.sprite.size()));
				for (auto & element : manifest.sprite) {
					process_sprite<version>(data, element);
				}
				if constexpr (version.number <= 3_i) {
					process_sprite<version>(data, manifest.main_sprite);
				} else {
					write_boolean(data, manifest.main_sprite.has());
					if (manifest.main_sprite.has()) {
						process_sprite<version>(data, manifest.main_sprite.get());
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

			#pragma region atom

			inline auto read_boolean (
				IByteStreamView & data
			) -> Boolean {
				return data.read_of<Boolean>();
			}

			// ----------------

			inline auto read_integer_u8 (
				IByteStreamView & data
			) -> IntegerU8 {
				return data.read_of<IntegerU8>();
			}

			inline auto read_integer_u16 (
				IByteStreamView & data
			) -> IntegerU16 {
				return data.read_of<IntegerU16>();
			}

			inline auto read_integer_s16 (
				IByteStreamView & data
			) -> IntegerS16 {
				return data.read_of<IntegerS16>();
			}

			inline auto read_integer_s32 (
				IByteStreamView & data
			) -> IntegerS32 {
				return data.read_of<IntegerS32>();
			}

			inline auto read_integer_u16_strictly (
				IByteStreamView & data
			) -> IntegerU16 {
				return data.read_of<IntegerU16>();
			}

			inline auto read_integer_u16_or_u8_strictly (
				IByteStreamView & data
			) -> IntegerU16 {
				auto value = IntegerU16{};
				auto value_8 = data.read_of<IntegerU8>();
				if (value_8 == ~(0_iu8)) {
					value = data.read_of<IntegerU16>();
				} else {
					value = cbw<IntegerU16>(value_8);
				}
				return value;
			}

			inline auto read_integer_u32_or_u16_with_flag_strictly (
				IByteStreamView & data,
				IntegerU32 &      value,
				IntegerU16 &      flag,
				Size const &      flag_bit_count
			) -> Void {
				auto value_16_bit_count = k_type_bit_count<IntegerU16> - flag_bit_count;
				auto value_16_maximum = ~(~0_iu16 << value_16_bit_count);
				auto composite_value = data.read_of<IntegerU16>();
				auto value_16 = clip_bit(composite_value, k_begin_index, value_16_bit_count);
				auto flag_16 = clip_bit(composite_value, value_16_bit_count, flag_bit_count);
				if (value_16 == value_16_maximum) {
					value = data.read_of<IntegerU32>();
				} else {
					value = cbw<IntegerU32>(value_16);
				}
				flag = flag_16;
				return;
			}

			// ----------------

			inline auto read_string_by_u16_strictly (
				IByteStreamView & data
			) -> String {
				return self_cast<String>(data.read_of<StringBlock16>());
			}

			#pragma endregion

			#pragma region process

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_frame (
				IByteStreamView &          data,
				Manifest::Frame<version> & manifest
			) -> Void {
				auto flag_integer = read_integer_u8(data);
				auto flag = BitSet<FrameFlag::k_count.value>{};
				flag.from_integer(flag_integer);
				if (flag.get(FrameFlag::remove)) {
					manifest.remove.allocate_full(cbw<Size>(read_integer_u16_or_u8_strictly(data)));
					for (auto & element : manifest.remove) {
						auto layer_index = IntegerU32{};
						auto layer_flag_integer = IntegerU16{};
						read_integer_u32_or_u16_with_flag_strictly(data, layer_index, layer_flag_integer, LayerRemoveFlag::k_count);
						element.index = cbw<Integer>(layer_index);
						auto layer_flag = BitSet<LayerRemoveFlag::k_count.value>{};
						layer_flag.from_integer(cbw<IntegerU8>(layer_flag_integer));
					}
				} else {
					manifest.remove.reset();
				}
				if (flag.get(FrameFlag::append)) {
					manifest.append.allocate_full(cbw<Size>(read_integer_u16_or_u8_strictly(data)));
					for (auto & element : manifest.append) {
						auto layer_index = IntegerU32{};
						auto layer_flag_integer = IntegerU16{};
						read_integer_u32_or_u16_with_flag_strictly(data, layer_index, layer_flag_integer, LayerAppendFlag::k_count);
						element.index = cbw<Integer>(layer_index);
						auto layer_flag = BitSet<LayerAppendFlag::k_count.value>{};
						layer_flag.from_integer(cbw<IntegerU8>(layer_flag_integer));
						if constexpr (version.number <= 5_i) {
							element.resource = cbw<Integer>(read_integer_u8(data));
						} else {
							element.resource = cbw<Integer>(read_integer_u16_or_u8_strictly(data));
						}
						if (layer_flag.get(LayerAppendFlag::sprite)) {
							element.sprite = k_true;
						} else {
							element.sprite = k_false;
						}
						if (layer_flag.get(LayerAppendFlag::additive)) {
							element.additive = k_true;
						} else {
							element.additive = k_false;
						}
						if (layer_flag.get(LayerAppendFlag::preload_frame)) {
							element.preload_frame = cbw<Integer>(read_integer_s16(data));
						} else {
							element.preload_frame = 0_i;
						}
						if (layer_flag.get(LayerAppendFlag::name)) {
							element.name.set();
							element.name.get() = read_string_by_u16_strictly(data);
						} else {
							element.name.reset();
						}
						if (layer_flag.get(LayerAppendFlag::time_scale)) {
							element.time_scale = cbw<Floating>(read_integer_s32(data)) / ValueRate::time;
						} else {
							element.time_scale = 1.0_f;
						}
					}
				} else {
					manifest.append.reset();
				}
				if (flag.get(FrameFlag::change)) {
					manifest.change.allocate_full(cbw<Size>(read_integer_u16_or_u8_strictly(data)));
					for (auto & element : manifest.change) {
						auto layer_index = IntegerU32{};
						auto layer_flag_integer = IntegerU16{};
						read_integer_u32_or_u16_with_flag_strictly(data, layer_index, layer_flag_integer, LayerChangeFlag::k_count);
						element.index = cbw<Integer>(layer_index);
						auto layer_flag = BitSet<LayerChangeFlag::k_count.value>{};
						layer_flag.from_integer(cbw<IntegerU8>(layer_flag_integer));
						auto translate_x = VWrapperView<Floating>{};
						auto translate_y = VWrapperView<Floating>{};
						if (layer_flag.get(LayerChangeFlag::matrix)) {
							assert_condition(!layer_flag.get(LayerChangeFlag::rotate));
							element.transform.template set<Manifest::MatrixTranslateTransform<version>>();
							auto & transform = element.transform.template get<Manifest::MatrixTranslateTransform<version>>();
							transform.a = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix;
							transform.c = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix;
							transform.b = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix;
							transform.d = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix;
							translate_x.set(transform.x);
							translate_y.set(transform.y);
						} else if (layer_flag.get(LayerChangeFlag::rotate)) {
							assert_condition(!layer_flag.get(LayerChangeFlag::matrix));
							element.transform.template set<Manifest::RotateTranslateTransform<version>>();
							auto & transform = element.transform.template get<Manifest::RotateTranslateTransform<version>>();
							transform.angle = cbw<Floating>(read_integer_s16(data)) / ValueRate::angle;
							translate_x.set(transform.x);
							translate_y.set(transform.y);
						} else {
							element.transform.template set<Manifest::TranslateTransform<version>>();
							auto & transform = element.transform.template get<Manifest::TranslateTransform<version>>();
							translate_x.set(transform.x);
							translate_y.set(transform.y);
						}
						if (layer_flag.get(LayerChangeFlag::long_coord)) {
							translate_x.get() = cbw<Floating>(read_integer_s32(data)) / ValueRate::size;
							translate_y.get() = cbw<Floating>(read_integer_s32(data)) / ValueRate::size;
						} else {
							translate_x.get() = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
							translate_y.get() = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
						}
						if (layer_flag.get(LayerChangeFlag::source_rectangle)) {
							element.source_rectangle.set();
							element.source_rectangle.get().position.x = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
							element.source_rectangle.get().position.y = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
							element.source_rectangle.get().size.width = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
							element.source_rectangle.get().size.height = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
						} else {
							element.source_rectangle.reset();
						}
						if (layer_flag.get(LayerChangeFlag::color)) {
							element.color.set();
							element.color.get().red = cbw<Floating>(read_integer_u8(data)) / ValueRate::color;
							element.color.get().green = cbw<Floating>(read_integer_u8(data)) / ValueRate::color;
							element.color.get().blue = cbw<Floating>(read_integer_u8(data)) / ValueRate::color;
							element.color.get().alpha = cbw<Floating>(read_integer_u8(data)) / ValueRate::color;
						} else {
							element.color.reset();
						}
						if (layer_flag.get(LayerChangeFlag::sprite_frame_number)) {
							element.sprite_frame_number.set();
							element.sprite_frame_number.get() = cbw<Integer>(read_integer_s16(data));
						} else {
							element.sprite_frame_number.reset();
						}
					}
				} else {
					manifest.change.reset();
				}
				if (flag.get(FrameFlag::label)) {
					manifest.label.set();
					manifest.label.get() = read_string_by_u16_strictly(data);
				} else {
					manifest.label.reset();
				}
				if (flag.get(FrameFlag::stop)) {
					manifest.stop = k_true;
				} else {
					manifest.stop = k_false;
				}
				if (flag.get(FrameFlag::command)) {
					manifest.command.allocate_full(cbw<Size>(read_integer_u8(data)));
					for (auto & element : manifest.command) {
						element.command = read_string_by_u16_strictly(data);
						element.argument = read_string_by_u16_strictly(data);
					}
				} else {
					manifest.command.reset();
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_sprite (
				IByteStreamView &           data,
				Manifest::Sprite<version> & manifest
			) -> Void {
				if constexpr (version.number >= 4_i) {
					manifest.name = read_string_by_u16_strictly(data);
					if constexpr (version.number >= 6_i) {
						data.forward(2_sz); // NOTE : unknown
					}
					manifest.frame_rate = cbw<Floating>(read_integer_s32(data)) / ValueRate::time;
				}
				manifest.frame.allocate_full(cbw<Size>(read_integer_u16_strictly(data)));
				if constexpr (version.number >= 5_i) {
					manifest.work_area.start = cbw<Integer>(read_integer_s16(data));
					manifest.work_area.duration = cbw<Integer>(read_integer_s16(data));
				}
				for (auto & element : manifest.frame) {
					process_frame<version>(data, element);
				}
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_image (
				IByteStreamView &          data,
				Manifest::Image<version> & manifest
			) -> Void {
				manifest.name = read_string_by_u16_strictly(data);
				if constexpr (version.number >= 4_i) {
					manifest.size.width = cbw<Integer>(read_integer_s16(data));
					manifest.size.height = cbw<Integer>(read_integer_s16(data));
				}
				if constexpr (version.number <= 1_i) {
					manifest.transform.angle = cbw<Floating>(read_integer_s16(data)) / ValueRate::angle;
				} else {
					manifest.transform.a = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix_exact;
					manifest.transform.c = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix_exact;
					manifest.transform.b = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix_exact;
					manifest.transform.d = cbw<Floating>(read_integer_s32(data)) / ValueRate::matrix_exact;
				}
				manifest.transform.x = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
				manifest.transform.y = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
				return;
			}

			template <auto version> requires
				CategoryConstraint<>
				&& (IsSameV<version, Version>)
				&& (VersionEnum::has(version))
			inline auto process_animation (
				IByteStreamView &              data,
				Manifest::Animation<version> & manifest
			) -> Void {
				assert_condition(data.read_of<Structure::MagicIdentifier>() == Structure::k_magic_identifier);
				assert_condition(data.read_of<Structure::VersionNumber>() == cbw<Structure::VersionNumber>(version.number));
				manifest.frame_rate = cbw<Integer>(read_integer_u8(data));
				manifest.position.x = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
				manifest.position.y = cbw<Floating>(read_integer_s16(data)) / ValueRate::size;
				manifest.size.width = cbw<Floating>(read_integer_u16(data)) / ValueRate::size;
				manifest.size.height = cbw<Floating>(read_integer_u16(data)) / ValueRate::size;
				manifest.image.allocate_full(cbw<Size>(read_integer_u16_strictly(data)));
				for (auto & element : manifest.image) {
					process_image<version>(data, element);
				}
				manifest.sprite.allocate_full(cbw<Size>(read_integer_u16_strictly(data)));
				for (auto & element : manifest.sprite) {
					process_sprite<version>(data, element);
				}
				if constexpr (version.number <= 3_i) {
					process_sprite<version>(data, manifest.main_sprite);
				} else {
					auto has_main_script = read_boolean(data);
					if (has_main_script) {
						manifest.main_sprite.set();
						process_sprite<version>(data, manifest.main_sprite.get());
					} else {
						manifest.main_sprite.reset();
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
