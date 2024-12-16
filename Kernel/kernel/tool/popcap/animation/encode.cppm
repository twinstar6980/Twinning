module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.animation.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.animation.version;
import twinning.kernel.tool.popcap.animation.definition;
import twinning.kernel.tool.popcap.animation.common;
import twinning.kernel.tool.common.byte_stream;

export namespace Twinning::Kernel::Tool::PopCap::Animation {

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		Common<version>,
		CommonByteStreamExchangeForOutput {

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
			OByteStreamView & data,
			Size const &      value
		) -> Void {
			auto value_integer = cbox<Integer>(value);
			exchange_integer_variant<RawShortValue, RawLongValue>(data, value_integer);
			return;
		}

		template <typename RawShortValue, typename RawLongValue> requires
			CategoryConstraint<IsPureInstance<RawShortValue> && IsPureInstance<RawLongValue>>
			&& (IsIntegerBox<RawShortValue>)
			&& (IsIntegerBox<RawLongValue>)
		inline static auto exchange_integer_variant (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			auto value_short_maximum = ~mbox<RawShortValue>(0);
			auto value_long = cbox<RawLongValue>(value);
			if (value_long < cbox<RawLongValue>(value_short_maximum)) {
				data.write(cbox<RawShortValue>(value_long));
			}
			else {
				data.write(value_short_maximum);
				data.write(value_long);
			}
			return;
		}

		template <typename RawShortValue, typename RawLongValue, auto flag_count> requires
			CategoryConstraint<IsPureInstance<RawShortValue> && IsPureInstance<RawLongValue>>
			&& (IsIntegerBox<RawShortValue>)
			&& (IsIntegerBox<RawLongValue>)
			&& (IsSameV<flag_count, Size>)
		inline static auto exchange_integer_variant_with_flag (
			OByteStreamView &          data,
			Integer const &            value,
			BitSet<flag_count> const & flag
		) -> Void {
			auto value_short_bit_count = k_type_bit_count<RawShortValue> - flag_count;
			auto value_short_maximum = ~(~mbox<RawShortValue>(0) << value_short_bit_count);
			auto value_long = cbox<RawLongValue>(value);
			auto value_flag = cbox<RawShortValue>(flag.to_integer());
			auto value_short_with_flag = value_flag << value_short_bit_count;
			if (value_long < cbox<RawLongValue>(value_short_maximum)) {
				value_short_with_flag |= cbox<RawShortValue>(value_long);
				data.write(value_short_with_flag);
			}
			else {
				value_short_with_flag |= value_short_maximum;
				data.write(value_short_with_flag);
				data.write(value_long);
			}
			return;
		}

		template <typename RawValue, auto rate> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsIntegerBox<RawValue>)
			&& (IsSameV<rate, Floater>)
		inline static auto exchange_floater_with_rate (
			OByteStreamView & data,
			Floater const &   value
		) -> Void {
			auto value_integer = value * rate;
			data.write(cbox<RawValue>(Math::round<Floater>(value_integer)));
			return;
		}

		// ----------------

		inline static auto exchange_layer_remove (
			OByteStreamView &                        data,
			typename Definition::LayerRemove const & value
		) -> Void {
			auto flag = BitSet<LayerRemoveFlag::k_count>{};
			exchange_integer_variant_with_flag<IntegerU16, IntegerU32>(data, value.index, flag);
			return;
		}

		inline static auto exchange_layer_append (
			OByteStreamView &                        data,
			typename Definition::LayerAppend const & value
		) -> Void {
			auto flag = BitSet<LayerAppendFlag::k_count>{};
			if (value.time_scale != 1.0_f) {
				flag.set(LayerAppendFlag::time_scale);
			}
			if (value.name.has()) {
				flag.set(LayerAppendFlag::name);
			}
			if (value.preload_frame != 0_i) {
				flag.set(LayerAppendFlag::preload_frame);
			}
			if (value.additive) {
				flag.set(LayerAppendFlag::additive);
			}
			if (value.sprite) {
				flag.set(LayerAppendFlag::sprite);
			}
			exchange_integer_variant_with_flag<IntegerU16, IntegerU32>(data, value.index, flag);
			if constexpr (check_version(version, {1, 6})) {
				exchange_integer_fixed<IntegerU8>(data, value.resource);
			}
			if constexpr (check_version(version, {6})) {
				exchange_integer_variant<IntegerU8, IntegerU16>(data, value.resource);
			}
			if (flag.get(LayerAppendFlag::sprite)) {
			}
			if (flag.get(LayerAppendFlag::additive)) {
			}
			if (flag.get(LayerAppendFlag::preload_frame)) {
				exchange_integer_fixed<IntegerS16>(data, value.preload_frame);
			}
			if (flag.get(LayerAppendFlag::name)) {
				exchange_string_block<IntegerU16>(data, value.name.get());
			}
			if (flag.get(LayerAppendFlag::time_scale)) {
				exchange_floater_with_rate<IntegerS32, ValueRate::time>(data, value.time_scale);
			}
			return;
		}

		inline static auto exchange_layer_change (
			OByteStreamView &                        data,
			typename Definition::LayerChange const & value
		) -> Void {
			auto flag = BitSet<LayerChangeFlag::k_count>{};
			if (value.sprite_frame_number.has()) {
				flag.set(LayerChangeFlag::sprite_frame_number);
			}
			if (value.color.has()) {
				flag.set(LayerChangeFlag::color);
			}
			if (value.source_rectangle.has()) {
				flag.set(LayerChangeFlag::source_rectangle);
			}
			if (value.transform.template is<typename Definition::RotateTranslateTransform>()) {
				flag.set(LayerChangeFlag::rotate);
			}
			if (value.transform.template is<typename Definition::MatrixTranslateTransform>()) {
				flag.set(LayerChangeFlag::matrix);
			}
			{
				flag.set(LayerChangeFlag::long_coord);
			}
			exchange_integer_variant_with_flag<IntegerU16, IntegerU32>(data, value.index, flag);
			if (!flag.get(LayerChangeFlag::rotate) && !flag.get(LayerChangeFlag::matrix)) {
				exchange(
					data,
					value.transform.template get<typename Definition::TranslateTransform>(),
					[] (auto & data, auto & value) {
					}
				);
			}
			if (flag.get(LayerChangeFlag::rotate)) {
				exchange(
					data,
					value.transform.template get<typename Definition::RotateTranslateTransform>(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS16, ValueRate::angle>(data, value.angle);
					}
				);
			}
			if (flag.get(LayerChangeFlag::matrix)) {
				exchange(
					data,
					value.transform.template get<typename Definition::MatrixTranslateTransform>(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.a);
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.c);
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.b);
						exchange_floater_with_rate<IntegerS32, ValueRate::matrix>(data, value.d);
					}
				);
			}
			{
				exchange_variant_value(
					data,
					value.transform,
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS32, ValueRate::size>(data, value.x);
						exchange_floater_with_rate<IntegerS32, ValueRate::size>(data, value.y);
					}
				);
			}
			if (flag.get(LayerChangeFlag::source_rectangle)) {
				exchange(
					data,
					value.source_rectangle.get(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.position.x);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.position.y);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.size.width);
						exchange_floater_with_rate<IntegerS16, ValueRate::size>(data, value.size.height);
					}
				);
			}
			if (flag.get(LayerChangeFlag::color)) {
				exchange(
					data,
					value.color.get(),
					[] (auto & data, auto & value) {
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.red);
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.green);
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.blue);
						exchange_floater_with_rate<IntegerU8, ValueRate::color>(data, value.alpha);
					}
				);
			}
			if (flag.get(LayerChangeFlag::sprite_frame_number)) {
				exchange(
					data,
					value.sprite_frame_number.get(),
					[] (auto & data, auto & value) {
						exchange_integer_fixed<IntegerS16>(data, value);
					}
				);
			}
			return;
		}

		inline static auto exchange_frame (
			OByteStreamView &                  data,
			typename Definition::Frame const & value
		) -> Void {
			auto flag = BitSet<FrameFlag::k_count>{};
			if (!value.remove.empty()) {
				flag.set(FrameFlag::remove);
			}
			if (!value.append.empty()) {
				flag.set(FrameFlag::append);
			}
			if (!value.change.empty()) {
				flag.set(FrameFlag::change);
			}
			if (value.label.has()) {
				flag.set(FrameFlag::label);
			}
			if (value.stop) {
				flag.set(FrameFlag::stop);
			}
			if (!value.command.empty()) {
				flag.set(FrameFlag::command);
			}
			exchange_integer_fixed<IntegerU8>(data, cbox<Integer>(flag.to_integer()));
			if (flag.get(FrameFlag::remove)) {
				exchange_list(data, value.remove, &exchange_size_variant<IntegerU8, IntegerU16>, &exchange_layer_remove);
			}
			if (flag.get(FrameFlag::append)) {
				exchange_list(data, value.append, &exchange_size_variant<IntegerU8, IntegerU16>, &exchange_layer_append);
			}
			if (flag.get(FrameFlag::change)) {
				exchange_list(data, value.change, &exchange_size_variant<IntegerU8, IntegerU16>, &exchange_layer_change);
			}
			if (flag.get(FrameFlag::label)) {
				exchange_string_block<IntegerU16>(data, value.label.get());
			}
			if (flag.get(FrameFlag::stop)) {
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
			return;
		}

		inline static auto exchange_sprite (
			OByteStreamView &                   data,
			typename Definition::Sprite const & value
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
			OByteStreamView &                  data,
			typename Definition::Image const & value
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
			OByteStreamView &                      data,
			typename Definition::Animation const & value
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
			OByteStreamView &                      data,
			typename Definition::Animation const & definition
		) -> Void {
			data.write_constant(k_magic_identifier);
			data.write_constant(cbox<VersionNumber>(version.number));
			exchange_animation(data, definition);
			return;
		}

		// ----------------

		inline static auto process (
			OByteStreamView &                      data_,
			typename Definition::Animation const & definition
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, definition);
		}

	};

}
