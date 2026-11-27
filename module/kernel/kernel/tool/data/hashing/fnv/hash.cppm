module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hashing.fnv.hash;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hashing.fnv.common;

export namespace Twinning::Kernel::Tool::Data::Hashing::Fnv {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		template <auto t_mode, auto t_length> requires
			CategoryConstraint<>
			&& (IsSameOf<t_mode, Mode>)
			&& (IsSameOf<t_length, Size>)
		inline static auto process_whole_integer(
			ConstantBinaryView const &      data,
			Parameter<t_length>::Value & value
		) -> Void {
			using Parameter = Parameter<t_length>;
			if constexpr (t_mode == Mode::Constant::v0()) {
				value = make_box<typename Parameter::Value>(0);
			}
			if constexpr (t_mode == Mode::Constant::v1() || t_mode == Mode::Constant::v1a()) {
				value = Parameter::offset;
			}
			for (auto & element : data) {
				if constexpr (t_mode == Mode::Constant::v0() || t_mode == Mode::Constant::v1()) {
					value = unchecked_number_multiply(value, Parameter::prime);
					value ^= cast_box_unsafe<typename Parameter::Value>(element);
				}
				if constexpr (t_mode == Mode::Constant::v1a()) {
					value ^= cast_box_unsafe<typename Parameter::Value>(element);
					value = unchecked_number_multiply(value, Parameter::prime);
				}
			}
			return;
		}

		inline static auto process_whole(
			ConstantBinaryView const & data,
			Binary &                   value,
			Mode const &               mode,
			Size const &           length
		) -> Void {
			auto value_data = Binary{};
			do {
				if (mode == Mode::Constant::v0() && length == 32_sz) {
					value_data.allocate_full(Math::compute_padded_unit_count(length, type_bit_size_of<Byte>()));
					process_whole_integer<Mode::Constant::v0(), 32_sz>(data, *cast_pointer<Parameter<32_sz>::Value>(value_data.data()));
					break;
				}
				if (mode == Mode::Constant::v0() && length == 64_sz) {
					value_data.allocate_full(Math::compute_padded_unit_count(length, type_bit_size_of<Byte>()));
					process_whole_integer<Mode::Constant::v0(), 64_sz>(data, *cast_pointer<Parameter<64_sz>::Value>(value_data.data()));
					break;
				}
				if (mode == Mode::Constant::v1() && length == 32_sz) {
					value_data.allocate_full(Math::compute_padded_unit_count(length, type_bit_size_of<Byte>()));
					process_whole_integer<Mode::Constant::v1(), 32_sz>(data, *cast_pointer<Parameter<32_sz>::Value>(value_data.data()));
					break;
				}
				if (mode == Mode::Constant::v1() && length == 64_sz) {
					value_data.allocate_full(Math::compute_padded_unit_count(length, type_bit_size_of<Byte>()));
					process_whole_integer<Mode::Constant::v1(), 64_sz>(data, *cast_pointer<Parameter<64_sz>::Value>(value_data.data()));
					break;
				}
				if (mode == Mode::Constant::v1a() && length == 32_sz) {
					value_data.allocate_full(Math::compute_padded_unit_count(length, type_bit_size_of<Byte>()));
					process_whole_integer<Mode::Constant::v1a(), 32_sz>(data, *cast_pointer<Parameter<32_sz>::Value>(value_data.data()));
					break;
				}
				if (mode == Mode::Constant::v1a() && length == 64_sz) {
					value_data.allocate_full(Math::compute_padded_unit_count(length, type_bit_size_of<Byte>()));
					process_whole_integer<Mode::Constant::v1a(), 64_sz>(data, *cast_pointer<Parameter<64_sz>::Value>(value_data.data()));
					break;
				}
				throw UnsupportedException{};
			} while (false);
			value = as_moveable(value_data);
			return;
		}

		// ----------------

		inline static auto process(
			ConstantBinaryView const & data,
			Binary &                   value,
			Mode const &               mode,
			Size const &           length
		) -> Void {
			restruct(value);
			return process_whole(data, value, mode, length);
		}

	};

}
