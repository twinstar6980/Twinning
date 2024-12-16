module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.animation.decode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.animation.version;
import twinning.kernel.tool.popcap.animation.definition;
import twinning.kernel.tool.popcap.animation.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::Animation {

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		Common<version>,
		CommonByteStreamExchangeForInput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::LayerRemoveFlag;

		using typename Common::LayerAppendFlag;

		using typename Common::LayerChangeFlag;

		using typename Common::FrameFlag;

		using typename Common::ValueRate;

		// ----------------

		template <typename RawShortValue, typename RawLongValue> requires
			CategoryConstraint<IsPureInstance<RawShortValue> && IsPureInstance<RawLongValue>>
			&& (IsIntegerBox<RawShortValue>)
			&& (IsIntegerBox<RawLongValue>)
		inline static auto exchange_size_variant (
			IByteStreamView & data,
			Size &            value
		) -> Void {
			auto value_integer = Integer{};
			exchange_integer_variant<RawShortValue, RawLongValue>(data, value_integer);
			value = cbox<Size>(value_integer);
			return;
		}

		template <typename RawShortValue, typename RawLongValue> requires
			CategoryConstraint<IsPureInstance<RawShortValue> && IsPureInstance<RawLongValue>>
			&& (IsIntegerBox<RawShortValue>)
			&& (IsIntegerBox<RawLongValue>)
		inline static auto exchange_integer_variant (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			auto value_short_maximum = ~mbox<RawShortValue>(0);
			auto value_short = data.read_of<RawShortValue>();
			auto value_long = RawLongValue{};
			if (value_short != value_short_maximum) {
				value_long = cbox<RawLongValue>(value_short);
			}
			else {
				value_long = data.read_of<RawLongValue>();
			}
			value = cbox<Integer>(value_long);
			return;
		}

		template <typename RawShortValue, typename RawLongValue, auto flag_count> requires
			CategoryConstraint<IsPureInstance<RawShortValue> && IsPureInstance<RawLongValue>>
			&& (IsIntegerBox<RawShortValue>)
			&& (IsIntegerBox<RawLongValue>)
			&& (IsSameV<flag_count, Size>)
		inline static auto exchange_integer_variant_with_flag (
			IByteStreamView &    data,
			Integer &            value,
			BitSet<flag_count> & flag
		) -> Void {
			auto value_short_bit_count = k_type_bit_count<RawShortValue> - flag_count;
			auto value_short_maximum = ~(~mbox<RawShortValue>(0) << value_short_bit_count);
			auto value_short_with_flag = data.read_of<RawShortValue>();
			auto value_short = clip_bit(value_short_with_flag, k_begin_index, value_short_bit_count);
			auto value_flag = clip_bit(value_short_with_flag, value_short_bit_count, flag_count);
			auto value_long = RawLongValue{};
			if (value_short != value_short_maximum) {
				value_long = cbox<RawLongValue>(value_short);
			}
			else {
				value_long = data.read_of<RawLongValue>();
			}
			value = cbox<Integer>(value_long);
			flag.from_integer(cbox<typename BitSet<flag_count>::BoundedInteger>(value_flag));
			return;
		}

		template <typename RawValue, auto rate> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerBox<RawValue>)
			&& (IsSameV<rate, Floater>)
		inline static auto exchange_floater_with_rate (
			IByteStreamView & data,
			Floater &         value
		) -> Void {
			auto value_integer = cbox<Floater>(data.read_of<RawValue>());
			value = value_integer / rate;
			return;
		}

		// ----------------

		inline static auto exchange_layer_remove (
			IByteStreamView &                  data,
			typename Definition::LayerRemove & value
		) -> Void {
			auto flag = BitSet<LayerRemoveFlag::k_count>{};
			exchange_integer_variant_with_flag<IntegerU16, IntegerU32>(data, value.index, flag);
			return;
		}

		inline static auto exchange_layer_append (
			IByteStreamView &                  data,
			typename Definition::LayerAppend & value
		) -> Void {
			auto flag = BitSet<LayerAppendFlag::k_count>{};
			exchange_integer_variant_with_flag<IntegerU16, IntegerU32>(data, value.index, flag);
			if constexpr (check_version(version, {1, 6})) {
				exchange_integer_fixed<IntegerU8>(data, value.resource);
			}
			if constexpr (check_version(version, {6})) {
				exchange_integer_variant<IntegerU8, IntegerU16>(data, value.resource);
			}
			if (flag.get(LayerAppendFlag::sprite)) {
				value.sprite = k_true;
			}
			else {
				value.sprite = k_false;
			}
			if (flag.get(LayerAppendFlag::additive)) {
				value.additive = k_true;
			}
			else {
				value.additive = k_false;
			}
			if (flag.get(LayerAppendFlag::preload_frame)) {
				exchange_integer_fixed<IntegerS16>(data, value.preload_frame);
			}
			else {
				value.preload_frame = 0_i;
			}
			if (flag.get(LayerAppendFlag::name)) {
				exchange_string_block<IntegerU16>(data, value.name.set());
			}
			else {
				value.name.reset();
			}
			if (flag.get(LayerAppendFlag::time_scale)) {
				exchange_floater_with_rate<IntegerS32, ValueRate::time>(data, value.time_scale);
			}
			else {
				value.time_scale = 1.0_f;
			}
			return;
		}

		inline static auto exchange_layer_change (
			IByteStreamView &                  data,
			typename Definition::LayerChange & value
		) -> Void {
			auto flag = BitSet<LayerChangeFlag::k_count>{};
			exchange_integer_variant_with_flag<IntegerU16, IntegerU32>(data, value.index, flag);
			if (!flag.get(LayerChangeFlag::rotate) && !flag.get(LayerChangeFlag::matrix)) {
				exchange(
					data,
					value.transform.template set<typename Definition::TranslateTransform>(),
					[] (auto & data, auto & value) {
					}
				);
			}
			if (flag.get(LayerChangeFlag::rotate)) {
				assert_test(!flag.get(LayerChangeFlag::matrix));
				exchange(
					data,
					value.transform.template set<typename Definition::RotateTranslateTransform>(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS16, ValueRate::angle>(data, value.angle);
					}
				);
			}
			if (flag.get(LayerChangeFlag::matrix)) {
				assert_test(!flag.get(LayerChangeFlag::rotate));
				exchange(
					data,
					value.transform.template set<typename Definition::MatrixTranslateTransform>(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.a);
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.c);
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.b);
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.d);
					}
				);
			}
			if (flag.get(LayerChangeFlag::long_coord)) {
				exchange_variant_value(
					data,
					value.transform,
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS32, ValueRate::size>(data, value.x);
						exchange_floater_with_rate<IntegerS32, ValueRate::size>(data, value.y);
					}
				);
			}
			else {
				exchange_variant_value(
					data,
					value.transform,
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.x);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.y);
					}
				);
			}
			if (flag.get(LayerChangeFlag::source_rectangle)) {
				exchange(
					data,
					value.source_rectangle.set(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.position.x);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.position.y);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.size.width);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.size.height);
					}
				);
			}
			else {
				value.source_rectangle.reset();
			}
			if (flag.get(LayerChangeFlag::color)) {
				exchange(
					data,
					value.color.set(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.red);
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.green);
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.blue);
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.alpha);
					}
				);
			}
			else {
				value.color.reset();
			}
			if (flag.get(LayerChangeFlag::sprite_frame_number)) {
				exchange(
					data,
					value.sprite_frame_number.set(),
					[] (auto & data, auto & value) {
						exchange_integer_fixed<IntegerS16>(data, value);
					}
				);
			}
			else {
				value.sprite_frame_number.reset();
			}
			return;
		}

		inline static auto exchange_frame (
			IByteStreamView &            data,
			typename Definition::Frame & value
		) -> Void {
			auto flag_integer = Integer{};
			exchange_integer_fixed<IntegerU8>(data, flag_integer);
			auto flag = BitSet<FrameFlag::k_count>{};
			flag.from_integer(cbox<IntegerU8>(flag_integer));
			if (flag.get(FrameFlag::remove)) {
				exchange_list(data, value.remove, &exchange_size_variant<IntegerU8, IntegerU16>, &exchange_layer_remove);
			}
			else {
				value.remove.reset();
			}
			if (flag.get(FrameFlag::append)) {
				exchange_list(data, value.append, &exchange_size_variant<IntegerU8, IntegerU16>, &exchange_layer_append);
			}
			else {
				value.append.reset();
			}
			if (flag.get(FrameFlag::change)) {
				exchange_list(data, value.change, &exchange_size_variant<IntegerU8, IntegerU16>, &exchange_layer_change);
			}
			else {
				value.change.reset();
			}
			if (flag.get(FrameFlag::label)) {
				exchange_string_block<IntegerU16>(data, value.label.set());
			}
			else {
				value.label.reset();
			}
			if (flag.get(FrameFlag::stop)) {
				value.stop = k_true;
			}
			else {
				value.stop = k_false;
			}
			if (flag.get(FrameFlag::command)) {
				exchange_list(
					data,
					value.command,
					&exchange_size_fixed<IntegerU8>,
					[] (auto & data, auto & value) {
						exchange_string_block<IntegerU16>(data, value.command);
						exchange_string_block<IntegerU16>(data, value.argument);
					}
				);
			}
			else {
				value.command.reset();
			}
			return;
		}

		inline static auto exchange_sprite (
			IByteStreamView &             data,
			typename Definition::Sprite & value
		) -> Void {
			if constexpr (check_version(version, {4})) {
				exchange_string_block<IntegerU16>(data, value.name);
				if constexpr (check_version(version, {6})) {
					exchange_raw_constant(data, 0x0000_iu16);
				}
				exchange_floater_with_rate<IntegerS32, ValueRate::time>(data, value.frame_rate);
			}
			exchange_list_size(data, value.frame, &exchange_size_fixed<IntegerU16>);
			if constexpr (check_version(version, {5})) {
				exchange_integer_fixed<IntegerS16>(data, value.work_area.start);
				exchange_integer_fixed<IntegerS16>(data, value.work_area.duration);
			}
			exchange_list_element(data, value.frame, &exchange_frame);
			return;
		}

		inline static auto exchange_image (
			IByteStreamView &            data,
			typename Definition::Image & value
		) -> Void {
			exchange_string_block<IntegerU16>(data, value.name);
			if constexpr (check_version(version, {4})) {
				exchange_integer_fixed<IntegerS16>(data, value.size.width);
				exchange_integer_fixed<IntegerS16>(data, value.size.height);
			}
			if constexpr (check_version(version, {1, 2})) {
				exchange_floater_with_rate<IntegerS16, ValueRate::angle>(data, value.transform.angle);
			}
			if constexpr (check_version(version, {2})) {
				exchange_floater_with_rate<IntegerS32, ValueRate::matrix_exact>(data, value.transform.a);
				exchange_floater_with_rate<IntegerS32, ValueRate::matrix_exact>(data, value.transform.c);
				exchange_floater_with_rate<IntegerS32, ValueRate::matrix_exact>(data, value.transform.b);
				exchange_floater_with_rate<IntegerS32, ValueRate::matrix_exact>(data, value.transform.d);
			}
			exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.transform.x);
			exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.transform.y);
			return;
		}

		inline static auto exchange_animation (
			IByteStreamView &                data,
			typename Definition::Animation & value
		) -> Void {
			exchange_integer_fixed<IntegerU8>(data, value.frame_rate);
			exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.position.x);
			exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.position.y);
			exchange_floater_with_rate<IntegerU16, ValueRate::size>(data, value.size.width);
			exchange_floater_with_rate<IntegerU16, ValueRate::size>(data, value.size.height);
			exchange_list(data, value.image, &exchange_size_fixed<IntegerU16>, &exchange_image);
			exchange_list(data, value.sprite, &exchange_size_fixed<IntegerU16>, &exchange_sprite);
			if constexpr (check_version(version, {1, 4})) {
				exchange(data, value.main_sprite, &exchange_sprite);
			}
			if constexpr (check_version(version, {4})) {
				exchange_optional(data, value.main_sprite, &exchange_boolean_fixed<Boolean8>, &exchange_sprite);
			}
			return;
		}

		// ----------------

		inline static auto process_whole (
			IByteStreamView &                data,
			typename Definition::Animation & definition
		) -> Void {
			data.read_constant(k_magic_identifier);
			data.read_constant(cbox<VersionNumber>(version.number));
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
