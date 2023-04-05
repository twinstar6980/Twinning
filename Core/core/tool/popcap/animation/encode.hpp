#pragma once

// NOTE
// 在编解码PAM的一些细节上，与SexyFramework本身的逻辑有所出入。
// 对于一些应该为无符号整数的数值（列表大小、层索引），SexyFramework均使用有符号数存储，
// 加之代码本身并未判别符号位情况，因此，这导致了SexyFramework在二进制读写时出现了不合理之处：
// 对于Frame中的LayerRemove/Append/Change操作，三者都拥有一个指代层索引的index值，
// 这类index值在二进制读写时，额外携带了与之相关的flag值，SexyFramework首先读写一个short，高n位作为flag的bitset，剩余位存储index，若剩余位无法容纳index，则再读写index为int
// 因此，首先读入的short永远为非负数，对于Append/Change都是如此，因为二者都有一位以上的flag
// 但在对LayerRemove的index处理，由于Remove操作并无flag，却占用了5位作为flag，且在未对高位填0的情况下使用了>=操作符对short值进行判别，这导致的后果是，若读入的short为负数，则不会读写下一个int
// 由此导致的行为不一致应视为bug，因此，本程序不沿袭SexyFramework的处理方式，而将这类index视为无符号数，同时，也将列表大小这类数值视为无符号数，相关的操作使用的函数以_strictly作为后缀
// 此外，还有一些可能应视为无符号的值，SexyFramework也使用有符号数处理，暂时沿袭其处理方式

#include "core/utility/utility.hpp"
#include "core/tool/popcap/animation/version.hpp"
#include "core/tool/popcap/animation/manifest.hpp"

namespace TwinStar::Core::Tool::PopCap::Animation {

	template <auto version> requires (check_version(version, {}))
	struct EncodeCommon {

	protected:

		using Manifest = Manifest<version>;

		// ----------------

		using MagicIdentifier = IntegerU32;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0xBAF01954_iu32};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		struct LayerRemoveFlag {
			inline static constexpr auto k_count = Size{5_sz};
			inline static constexpr auto unused_1 = Size{1_ix};
			inline static constexpr auto unused_2 = Size{2_ix};
			inline static constexpr auto unused_3 = Size{3_ix};
			inline static constexpr auto unused_4 = Size{4_ix};
			inline static constexpr auto unused_5 = Size{5_ix};
		};

		struct LayerAppendFlag {
			inline static constexpr auto k_count = Size{5_sz};
			inline static constexpr auto time_scale = Size{1_ix};
			inline static constexpr auto name = Size{2_ix};
			inline static constexpr auto preload_frame = Size{3_ix};
			inline static constexpr auto additive = Size{4_ix};
			inline static constexpr auto sprite = Size{5_ix};
		};

		struct LayerChangeFlag {
			inline static constexpr auto k_count = Size{6_sz};
			inline static constexpr auto sprite_frame_number = Size{1_ix};
			inline static constexpr auto long_coord = Size{2_ix};
			inline static constexpr auto matrix = Size{3_ix};
			inline static constexpr auto color = Size{4_ix};
			inline static constexpr auto rotate = Size{5_ix};
			inline static constexpr auto source_rectangle = Size{6_ix};
		};

		struct FrameFlag {
			inline static constexpr auto k_count = Size{8_sz};
			inline static constexpr auto remove = Size{1_ix};
			inline static constexpr auto append = Size{2_ix};
			inline static constexpr auto change = Size{3_ix};
			inline static constexpr auto label = Size{4_ix};
			inline static constexpr auto stop = Size{5_ix};
			inline static constexpr auto command = Size{6_ix};
			inline static constexpr auto unused_7 = Size{7_ix};
			inline static constexpr auto unused_8 = Size{8_ix};
		};

		// ----------------

		struct ValueRate {
			inline static constexpr auto time = Floater{65536.0_f};
			inline static constexpr auto size = Floater{20.0_f};
			inline static constexpr auto angle = Floater{1000.0_f};
			inline static constexpr auto matrix = Floater{65536.0_f};
			inline static constexpr auto matrix_exact = Floater{20.0_f * 65536.0_f};
			inline static constexpr auto color = Floater{255.0_f};
		};

	};

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::LayerRemoveFlag;

		using typename Common::LayerAppendFlag;

		using typename Common::LayerChangeFlag;

		using typename Common::FrameFlag;

		using typename Common::ValueRate;

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

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBooleanWrapper<RawValue>)
		static auto exchange_unit_boolean (
			OByteStreamView & data,
			Boolean const &   value
		) -> Void {
			data.write(cbw<RawValue>(value));
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<RawValue>(value));
			return;
		}

		template <typename = None> requires
			CategoryConstraint<>
		static auto exchange_unit_integer_variant_16 (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			auto value_16 = cbw<IntegerU16>(value);
			if (value_16 >= cbw<IntegerU16>(~(0_iu8))) {
				data.write(~(0_iu8));
				data.write(value_16);
			} else {
				data.write(cbw<IntegerU8>(value_16));
			}
			return;
		}

		template <auto flag_count> requires
			CategoryConstraint<>
			&& (IsSameV<flag_count, Size>)
		static auto exchange_unit_integer_variant_32_with_flag_16 (
			OByteStreamView &          data,
			Integer const &            value,
			BitSet<flag_count> const & flag
		) -> Void {
			auto value_32 = cbw<IntegerU32>(value);
			auto flag_16 = cbw<IntegerU16>(flag.to_integer());
			auto value_16_bit_count = k_type_bit_count<IntegerU16> - flag_count;
			auto value_16_maximum = ~(~0_iu16 << value_16_bit_count);
			auto composite_value = flag_16 << value_16_bit_count;
			if (value_32 >= cbw<IntegerU32>(value_16_maximum)) {
				composite_value |= value_16_maximum;
				data.write(composite_value);
				data.write(value_32);
			} else {
				composite_value |= cbw<IntegerU16>(value_32);
				data.write(composite_value);
			}
			return;
		}

		template <typename RawValue, auto value_rate> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
			&& (IsSameV<value_rate, Floater>)
		static auto exchange_unit_floater (
			OByteStreamView & data,
			Floater const &   value
		) -> Void {
			auto value_integer = value * value_rate;
			data.write(cbw<RawValue>(Math::round<Floater>(value_integer)));
			return;
		}

		template <typename = None> requires
			CategoryConstraint<>
		static auto exchange_unit_string (
			OByteStreamView & data,
			String const &    value
		) -> Void {
			data.write(self_cast<StringBlock16>(value));
			return;
		}

		// ----------------

		static auto process_frame (
			OByteStreamView &                frame_data,
			typename Manifest::Frame const & frame_manifest
		) -> Void {
			auto flag = BitSet<FrameFlag::k_count>{};
			if (!frame_manifest.remove.empty()) {
				flag.set(FrameFlag::remove);
			}
			if (!frame_manifest.append.empty()) {
				flag.set(FrameFlag::append);
			}
			if (!frame_manifest.change.empty()) {
				flag.set(FrameFlag::change);
			}
			if (frame_manifest.label.has()) {
				flag.set(FrameFlag::label);
			}
			if (frame_manifest.stop) {
				flag.set(FrameFlag::stop);
			}
			if (!frame_manifest.command.empty()) {
				flag.set(FrameFlag::command);
			}
			exchange_unit_integer<IntegerU8>(frame_data, cbw<Integer>(flag.to_integer()));
			if (flag.get(FrameFlag::remove)) {
				exchange_unit_integer_variant_16<>(frame_data, cbw<Integer>(frame_manifest.remove.size()));
				for (auto & element : frame_manifest.remove) {
					auto layer_flag = BitSet<LayerRemoveFlag::k_count>{};
					exchange_unit_integer_variant_32_with_flag_16<>(frame_data, element.index, layer_flag);
				}
			}
			if (flag.get(FrameFlag::append)) {
				exchange_unit_integer_variant_16<>(frame_data, cbw<Integer>(frame_manifest.append.size()));
				for (auto & element : frame_manifest.append) {
					auto layer_flag = BitSet<LayerAppendFlag::k_count>{};
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
					exchange_unit_integer_variant_32_with_flag_16<>(frame_data, element.index, layer_flag);
					if constexpr (check_version(version, {1, 6})) {
						exchange_unit_integer<IntegerU8>(frame_data, element.resource);
					}
					if constexpr (check_version(version, {6})) {
						exchange_unit_integer_variant_16<>(frame_data, cbw<Integer>(element.resource));
					}
					if (layer_flag.get(LayerAppendFlag::preload_frame)) {
						exchange_unit_integer<IntegerS16>(frame_data, element.preload_frame);
					}
					if (layer_flag.get(LayerAppendFlag::name)) {
						exchange_unit_string(frame_data, element.name.get());
					}
					if (layer_flag.get(LayerAppendFlag::time_scale)) {
						exchange_unit_floater<IntegerS32, ValueRate::time>(frame_data, element.time_scale);
					}
				}
			}
			if (flag.get(FrameFlag::change)) {
				exchange_unit_integer_variant_16<>(frame_data, cbw<Integer>(frame_manifest.change.size()));
				for (auto & element : frame_manifest.change) {
					auto layer_flag = BitSet<LayerChangeFlag::k_count>{};
					if (element.sprite_frame_number.has()) {
						layer_flag.set(LayerChangeFlag::sprite_frame_number);
					}
					if (element.color.has()) {
						layer_flag.set(LayerChangeFlag::color);
					}
					if (element.source_rectangle.has()) {
						layer_flag.set(LayerChangeFlag::source_rectangle);
					}
					if (element.transform.template is<typename Manifest::TranslateTransform>()) {
					} else if (element.transform.template is<typename Manifest::RotateTranslateTransform>()) {
						layer_flag.set(LayerChangeFlag::rotate);
					} else if (element.transform.template is<typename Manifest::MatrixTranslateTransform>()) {
						layer_flag.set(LayerChangeFlag::matrix);
					} else {
						throw ImpossibleException{};
					}
					layer_flag.set(LayerChangeFlag::long_coord);
					exchange_unit_integer_variant_32_with_flag_16<>(frame_data, element.index, layer_flag);
					if (element.transform.template is<typename Manifest::TranslateTransform>()) {
						auto & transform = element.transform.template get<typename Manifest::TranslateTransform>();
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, transform.x);
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, transform.y);
					} else if (element.transform.template is<typename Manifest::RotateTranslateTransform>()) {
						auto & transform = element.transform.template get<typename Manifest::RotateTranslateTransform>();
						exchange_unit_floater<IntegerU16, ValueRate::angle>(frame_data, transform.angle);
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, transform.x);
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, transform.y);
					} else if (element.transform.template is<typename Manifest::MatrixTranslateTransform>()) {
						auto & transform = element.transform.template get<typename Manifest::MatrixTranslateTransform>();
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.a);
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.c);
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.b);
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.d);
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, transform.x);
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, transform.y);
					} else {
						throw ImpossibleException{};
					}
					if (layer_flag.get(LayerChangeFlag::source_rectangle)) {
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().position.x);
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().position.y);
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().size.width);
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().size.height);
					}
					if (layer_flag.get(LayerChangeFlag::color)) {
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().red);
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().green);
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().blue);
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().alpha);
					}
					if (layer_flag.get(LayerChangeFlag::sprite_frame_number)) {
						exchange_unit_integer<IntegerS16>(frame_data, element.sprite_frame_number.get());
					}
				}
			}
			if (flag.get(FrameFlag::label)) {
				exchange_unit_string(frame_data, frame_manifest.label.get());
			}
			if (flag.get(FrameFlag::command)) {
				exchange_unit_integer<IntegerU8>(frame_data, cbw<Integer>(frame_manifest.command.size()));
				for (auto & element : frame_manifest.command) {
					exchange_unit_string(frame_data, element.command);
					exchange_unit_string(frame_data, element.argument);
				}
			}
			return;
		}

		static auto process_sprite (
			OByteStreamView &                 sprite_data,
			typename Manifest::Sprite const & sprite_manifest
		) -> Void {
			if constexpr (check_version(version, {4})) {
				exchange_unit_string(sprite_data, sprite_manifest.name);
				if constexpr (check_version(version, {6})) {
					exchange_unit_constant(sprite_data, 0x0000_iu16);
				}
				exchange_unit_floater<IntegerS32, ValueRate::time>(sprite_data, sprite_manifest.frame_rate);
			}
			exchange_unit_integer<IntegerU16>(sprite_data, cbw<Integer>(sprite_manifest.frame.size()));
			if constexpr (check_version(version, {5})) {
				exchange_unit_integer<IntegerS16>(sprite_data, sprite_manifest.work_area.start);
				exchange_unit_integer<IntegerS16>(sprite_data, sprite_manifest.work_area.duration);
			}
			for (auto & element : sprite_manifest.frame) {
				process_frame(sprite_data, element);
			}
			return;
		}

		static auto process_image (
			OByteStreamView &                image_data,
			typename Manifest::Image const & image_manifest
		) -> Void {
			exchange_unit_string(image_data, image_manifest.name);
			if constexpr (check_version(version, {4})) {
				exchange_unit_integer<IntegerS16>(image_data, image_manifest.size.width);
				exchange_unit_integer<IntegerS16>(image_data, image_manifest.size.height);
			}
			if constexpr (check_version(version, {1, 2})) {
				exchange_unit_floater<IntegerS16, ValueRate::angle>(image_data, image_manifest.transform.angle);
			}
			if constexpr (check_version(version, {2})) {
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.a);
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.c);
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.b);
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.d);
			}
			exchange_unit_floater<IntegerS16, ValueRate::size>(image_data, image_manifest.transform.x);
			exchange_unit_floater<IntegerS16, ValueRate::size>(image_data, image_manifest.transform.y);
			return;
		}

		static auto process_animation (
			OByteStreamView &                    animation_data,
			typename Manifest::Animation const & animation_manifest
		) -> Void {
			animation_data.write_constant(k_magic_identifier);
			animation_data.write_constant(cbw<VersionNumber>(version.number));
			exchange_unit_integer<IntegerU8>(animation_data, animation_manifest.frame_rate);
			exchange_unit_floater<IntegerS16, ValueRate::size>(animation_data, animation_manifest.position.x);
			exchange_unit_floater<IntegerS16, ValueRate::size>(animation_data, animation_manifest.position.y);
			exchange_unit_floater<IntegerU16, ValueRate::size>(animation_data, animation_manifest.size.width);
			exchange_unit_floater<IntegerU16, ValueRate::size>(animation_data, animation_manifest.size.height);
			exchange_unit_integer<IntegerU16>(animation_data, cbw<Integer>(animation_manifest.image.size()));
			for (auto & element : animation_manifest.image) {
				process_image(animation_data, element);
			}
			exchange_unit_integer<IntegerU16>(animation_data, cbw<Integer>(animation_manifest.sprite.size()));
			for (auto & element : animation_manifest.sprite) {
				process_sprite(animation_data, element);
			}
			if constexpr (check_version(version, {1, 4})) {
				process_sprite(animation_data, animation_manifest.main_sprite);
			}
			if constexpr (check_version(version, {4})) {
				exchange_unit_boolean<Boolean8>(animation_data, animation_manifest.main_sprite.has());
				if (animation_manifest.main_sprite.has()) {
					process_sprite(animation_data, animation_manifest.main_sprite.get());
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

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::Manifest;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::LayerRemoveFlag;

		using typename Common::LayerAppendFlag;

		using typename Common::LayerChangeFlag;

		using typename Common::FrameFlag;

		using typename Common::ValueRate;

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

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBooleanWrapper<RawValue>)
		static auto exchange_unit_boolean (
			IByteStreamView & data,
			Boolean &         value
		) -> Void {
			value = cbw<Boolean>(data.read_of<RawValue>());
			return;
		}

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<RawValue>());
			return;
		}

		template <typename = None> requires
			CategoryConstraint<>
		static auto exchange_unit_integer_variant_16 (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			auto value_16 = IntegerU16{};
			auto value_8 = data.read_of<IntegerU8>();
			if (value_8 == ~(0_iu8)) {
				value_16 = data.read_of<IntegerU16>();
			} else {
				value_16 = cbw<IntegerU16>(value_8);
			}
			value = cbw<Integer>(value_16);
			return;
		}

		template <auto flag_count> requires
			CategoryConstraint<>
			&& (IsSameV<flag_count, Size>)
		static auto exchange_unit_integer_variant_32_with_flag_16 (
			IByteStreamView &    data,
			Integer &            value,
			BitSet<flag_count> & flag
		) -> Void {
			auto value_16_bit_count = k_type_bit_count<IntegerU16> - flag_count;
			auto value_16_maximum = ~(~0_iu16 << value_16_bit_count);
			auto composite_value = data.read_of<IntegerU16>();
			auto value_16 = clip_bit(composite_value, k_begin_index, value_16_bit_count);
			auto flag_16 = clip_bit(composite_value, value_16_bit_count, flag_count);
			auto value_32 = IntegerU32{};
			if (value_16 == value_16_maximum) {
				value_32 = data.read_of<IntegerU32>();
			} else {
				value_32 = cbw<IntegerU32>(value_16);
			}
			value = cbw<Integer>(value_32);
			flag.from_integer(cbw<typename BitSet<flag_count>::BoundedInteger>(flag_16));
			return;
		}

		template <typename RawValue, auto value_rate> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerWrapper<RawValue>)
			&& (IsSameV<value_rate, Floater>)
		static auto exchange_unit_floater (
			IByteStreamView & data,
			Floater &         value
		) -> Void {
			auto value_integer = cbw<Floater>(data.read_of<RawValue>());
			value = value_integer / value_rate;
			return;
		}

		template <typename = None> requires
			CategoryConstraint<>
		static auto exchange_unit_string (
			IByteStreamView & data,
			String &          value
		) -> Void {
			data.read(self_cast<StringBlock16>(value));
			return;
		}

		// ----------------

		static auto process_frame (
			IByteStreamView &          frame_data,
			typename Manifest::Frame & frame_manifest
		) -> Void {
			auto flag_integer = Integer{};
			exchange_unit_integer<IntegerU8>(frame_data, flag_integer);
			auto flag = BitSet<FrameFlag::k_count>{};
			flag.from_integer(cbw<IntegerU8>(flag_integer));
			if (flag.get(FrameFlag::remove)) {
				auto layer_count = Integer{};
				exchange_unit_integer_variant_16<>(frame_data, layer_count);
				frame_manifest.remove.allocate_full(cbw<Size>(layer_count));
				for (auto & element : frame_manifest.remove) {
					auto layer_flag = BitSet<LayerRemoveFlag::k_count>{};
					exchange_unit_integer_variant_32_with_flag_16<>(frame_data, element.index, layer_flag);
				}
			} else {
				frame_manifest.remove.reset();
			}
			if (flag.get(FrameFlag::append)) {
				auto layer_count = Integer{};
				exchange_unit_integer_variant_16<>(frame_data, layer_count);
				frame_manifest.append.allocate_full(cbw<Size>(layer_count));
				for (auto & element : frame_manifest.append) {
					auto layer_flag = BitSet<LayerAppendFlag::k_count>{};
					exchange_unit_integer_variant_32_with_flag_16<>(frame_data, element.index, layer_flag);
					if constexpr (check_version(version, {1, 6})) {
						exchange_unit_integer<IntegerU8>(frame_data, element.resource);
					}
					if constexpr (check_version(version, {6})) {
						exchange_unit_integer_variant_16<>(frame_data, element.resource);
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
						exchange_unit_integer<IntegerS16>(frame_data, element.preload_frame);
					} else {
						element.preload_frame = 0_i;
					}
					if (layer_flag.get(LayerAppendFlag::name)) {
						element.name.set();
						exchange_unit_string(frame_data, element.name.get());
					} else {
						element.name.reset();
					}
					if (layer_flag.get(LayerAppendFlag::time_scale)) {
						exchange_unit_floater<IntegerS32, ValueRate::time>(frame_data, element.time_scale);
					} else {
						element.time_scale = 1.0_f;
					}
				}
			} else {
				frame_manifest.append.reset();
			}
			if (flag.get(FrameFlag::change)) {
				auto layer_count = Integer{};
				exchange_unit_integer_variant_16<>(frame_data, layer_count);
				frame_manifest.change.allocate_full(cbw<Size>(layer_count));
				for (auto & element : frame_manifest.change) {
					auto layer_flag = BitSet<LayerChangeFlag::k_count>{};
					exchange_unit_integer_variant_32_with_flag_16<>(frame_data, element.index, layer_flag);
					auto translate_x = VWrapperView<Floater>{};
					auto translate_y = VWrapperView<Floater>{};
					if (layer_flag.get(LayerChangeFlag::matrix)) {
						assert_test(!layer_flag.get(LayerChangeFlag::rotate));
						element.transform.template set<typename Manifest::MatrixTranslateTransform>();
						auto & transform = element.transform.template get<typename Manifest::MatrixTranslateTransform>();
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.a);
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.c);
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.b);
						exchange_unit_floater<IntegerS32, ValueRate::matrix>(frame_data, transform.d);
						translate_x.set(transform.x);
						translate_y.set(transform.y);
					} else if (layer_flag.get(LayerChangeFlag::rotate)) {
						assert_test(!layer_flag.get(LayerChangeFlag::matrix));
						element.transform.template set<typename Manifest::RotateTranslateTransform>();
						auto & transform = element.transform.template get<typename Manifest::RotateTranslateTransform>();
						exchange_unit_floater<IntegerS16, ValueRate::angle>(frame_data, transform.angle);
						translate_x.set(transform.x);
						translate_y.set(transform.y);
					} else {
						element.transform.template set<typename Manifest::TranslateTransform>();
						auto & transform = element.transform.template get<typename Manifest::TranslateTransform>();
						translate_x.set(transform.x);
						translate_y.set(transform.y);
					}
					if (layer_flag.get(LayerChangeFlag::long_coord)) {
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, translate_x.get());
						exchange_unit_floater<IntegerS32, ValueRate::size>(frame_data, translate_y.get());
					} else {
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, translate_x.get());
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, translate_y.get());
					}
					if (layer_flag.get(LayerChangeFlag::source_rectangle)) {
						element.source_rectangle.set();
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().position.x);
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().position.y);
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().size.width);
						exchange_unit_floater<IntegerS16, ValueRate::size>(frame_data, element.source_rectangle.get().size.height);
					} else {
						element.source_rectangle.reset();
					}
					if (layer_flag.get(LayerChangeFlag::color)) {
						element.color.set();
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().red);
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().green);
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().blue);
						exchange_unit_floater<IntegerU8, ValueRate::color>(frame_data, element.color.get().alpha);
					} else {
						element.color.reset();
					}
					if (layer_flag.get(LayerChangeFlag::sprite_frame_number)) {
						element.sprite_frame_number.set();
						exchange_unit_integer<IntegerS16>(frame_data, element.sprite_frame_number.get());
					} else {
						element.sprite_frame_number.reset();
					}
				}
			} else {
				frame_manifest.change.reset();
			}
			if (flag.get(FrameFlag::label)) {
				frame_manifest.label.set();
				exchange_unit_string(frame_data, frame_manifest.label.get());
			} else {
				frame_manifest.label.reset();
			}
			if (flag.get(FrameFlag::stop)) {
				frame_manifest.stop = k_true;
			} else {
				frame_manifest.stop = k_false;
			}
			if (flag.get(FrameFlag::command)) {
				auto command_count = Integer{};
				exchange_unit_integer<IntegerU8>(frame_data, command_count);
				frame_manifest.command.allocate_full(cbw<Size>(command_count));
				for (auto & element : frame_manifest.command) {
					exchange_unit_string(frame_data, element.command);
					exchange_unit_string(frame_data, element.argument);
				}
			} else {
				frame_manifest.command.reset();
			}
			return;
		}

		static auto process_sprite (
			IByteStreamView &           sprite_data,
			typename Manifest::Sprite & sprite_manifest
		) -> Void {
			if constexpr (check_version(version, {4})) {
				exchange_unit_string(sprite_data, sprite_manifest.name);
				if constexpr (check_version(version, {6})) {
					exchange_unit_constant(sprite_data, 0x0000_iu16);
				}
				exchange_unit_floater<IntegerS32, ValueRate::time>(sprite_data, sprite_manifest.frame_rate);
			}
			sprite_manifest.frame.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer<IntegerU16>(sprite_data, it); }))));
			if constexpr (check_version(version, {5})) {
				exchange_unit_integer<IntegerS16>(sprite_data, sprite_manifest.work_area.start);
				exchange_unit_integer<IntegerS16>(sprite_data, sprite_manifest.work_area.duration);
			}
			for (auto & element : sprite_manifest.frame) {
				process_frame(sprite_data, element);
			}
			return;
		}

		static auto process_image (
			IByteStreamView &          image_data,
			typename Manifest::Image & image_manifest
		) -> Void {
			exchange_unit_string(image_data, image_manifest.name);
			if constexpr (check_version(version, {4})) {
				exchange_unit_integer<IntegerS16>(image_data, image_manifest.size.width);
				exchange_unit_integer<IntegerS16>(image_data, image_manifest.size.height);
			}
			if constexpr (check_version(version, {1, 2})) {
				exchange_unit_floater<IntegerS16, ValueRate::angle>(image_data, image_manifest.transform.angle);
			}
			if constexpr (check_version(version, {2})) {
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.a);
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.c);
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.b);
				exchange_unit_floater<IntegerS32, ValueRate::matrix_exact>(image_data, image_manifest.transform.d);
			}
			exchange_unit_floater<IntegerS16, ValueRate::size>(image_data, image_manifest.transform.x);
			exchange_unit_floater<IntegerS16, ValueRate::size>(image_data, image_manifest.transform.y);
			return;
		}

		static auto process_animation (
			IByteStreamView &              animation_data,
			typename Manifest::Animation & animation_manifest
		) -> Void {
			animation_data.read_constant(k_magic_identifier);
			animation_data.read_constant(cbw<VersionNumber>(version.number));
			exchange_unit_integer<IntegerU8>(animation_data, animation_manifest.frame_rate);
			exchange_unit_floater<IntegerS16, ValueRate::size>(animation_data, animation_manifest.position.x);
			exchange_unit_floater<IntegerS16, ValueRate::size>(animation_data, animation_manifest.position.y);
			exchange_unit_floater<IntegerU16, ValueRate::size>(animation_data, animation_manifest.size.width);
			exchange_unit_floater<IntegerU16, ValueRate::size>(animation_data, animation_manifest.size.height);
			animation_manifest.image.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({exchange_unit_integer<IntegerU16>(animation_data, it);}))));
			for (auto & element : animation_manifest.image) {
				process_image(animation_data, element);
			}
			animation_manifest.sprite.allocate_full(cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({exchange_unit_integer<IntegerU16>(animation_data, it);}))));
			for (auto & element : animation_manifest.sprite) {
				process_sprite(animation_data, element);
			}
			if constexpr (check_version(version, {1, 4})) {
				process_sprite(animation_data, animation_manifest.main_sprite);
			}
			if constexpr (check_version(version, {4})) {
				auto has_main_script = Boolean{};
				exchange_unit_boolean<Boolean8>(animation_data, has_main_script);
				if (has_main_script) {
					animation_manifest.main_sprite.set();
					process_sprite(animation_data, animation_manifest.main_sprite.get());
				} else {
					animation_manifest.main_sprite.reset();
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
