module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hash.fnv.hash;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hash.fnv.common;

export namespace Twinning::Kernel::Tool::Data::Hash::Fnv {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		template <auto t_mode, auto t_bit_count> requires
			CategoryConstraint<>
			&& (IsSameOf<t_mode, Mode>)
			&& (IsSameOf<t_bit_count, BitCount>)
		inline static auto process_whole_integer(
			ConstantByteListView const &    data,
			Parameter<t_bit_count>::Value & value
		) -> Void {
			using Parameter = Parameter<t_bit_count>;
			if constexpr (t_mode == Mode::Constant::m_0()) {
				value = mbox<typename Parameter::Value>(0);
			}
			if constexpr (t_mode == Mode::Constant::m_1() || t_mode == Mode::Constant::m_1a()) {
				value = Parameter::offset;
			}
			for (auto & element : data) {
				if constexpr (t_mode == Mode::Constant::m_0()) {
					value *= Parameter::prime;
					value ^= cbox<typename Parameter::Value>(element);
				}
				if constexpr (t_mode == Mode::Constant::m_1()) {
					value *= Parameter::prime;
					value ^= cbox<typename Parameter::Value>(element);
				}
				if constexpr (t_mode == Mode::Constant::m_1a()) {
					value ^= cbox<typename Parameter::Value>(element);
					value *= Parameter::prime;
				}
			}
			return;
		}

		inline static auto process_whole(
			ConstantByteListView const & data,
			ByteArray &                  value,
			Mode const &                 mode,
			BitCount const &             bit_count
		) -> Void {
			Generalization::match<ValuePackage<
				Mode::Constant::m_0(),
				Mode::Constant::m_1(),
				Mode::Constant::m_1a()
			>>(
				mode,
				[&] <auto t_mode_index, auto t_mode>(ValuePackage<t_mode_index>, ValuePackage<t_mode>) {
					Generalization::match<ValuePackage<
						BitCount::Constant::b_32(),
						BitCount::Constant::b_64()
					>>(
						bit_count,
						[&] <auto t_bit_count_index, auto t_bit_count>(ValuePackage<t_bit_count_index>, ValuePackage<t_bit_count>) {
							auto value_integer = typename Parameter<t_bit_count>::Value{};
							process_whole_integer<t_mode, t_bit_count>(data, value_integer);
							value.allocate(k_type_size<typename Parameter<t_bit_count>::Value>);
							OutputByteStreamView{value}.write(value_integer);
						}
					);
				}
			);
			return;
		}

		// ----------------

		inline static auto process(
			ConstantByteListView const & data,
			ByteArray &                  value,
			Mode const &                 mode,
			BitCount const &             bit_count
		) -> Void {
			restruct(value);
			return process_whole(data, value, mode, bit_count);
		}

	};

}
