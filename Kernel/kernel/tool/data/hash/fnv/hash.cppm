module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hash.fnv.hash;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hash.fnv.common;

export namespace Twinning::Kernel::Tool::Data::Hash::FNV {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		template <auto mode, auto bit_count> requires
			CategoryConstraint<>
			&& (IsSameV<mode, Mode>)
			&& (IsSameV<bit_count, BitCount>)
		inline static auto process_whole_integer (
			CByteListView const &                  data,
			typename Parameter<bit_count>::Value & value
		) -> Void {
			using Parameter = Parameter<bit_count>;
			if constexpr (mode == Mode::Constant::m_0()) {
				value = mbox<typename Parameter::Value>(0);
			}
			if constexpr (mode == Mode::Constant::m_1() || mode == Mode::Constant::m_1a()) {
				value = Parameter::offset;
			}
			for (auto & element : data) {
				if constexpr (mode == Mode::Constant::m_0()) {
					value *= Parameter::prime;
					value ^= cbox<typename Parameter::Value>(element);
				}
				if constexpr (mode == Mode::Constant::m_1()) {
					value *= Parameter::prime;
					value ^= cbox<typename Parameter::Value>(element);
				}
				if constexpr (mode == Mode::Constant::m_1a()) {
					value ^= cbox<typename Parameter::Value>(element);
					value *= Parameter::prime;
				}
			}
			return;
		}

		inline static auto process_whole (
			CByteListView const & data,
			ByteArray &           value,
			Mode const &          mode,
			BitCount const &      bit_count
		) -> Void {
			Generalization::match<ValuePackage<
				Mode::Constant::m_0(),
				Mode::Constant::m_1(),
				Mode::Constant::m_1a()
			>>(
				mode,
				[&] <auto index, auto mode> (ValuePackage<index>, ValuePackage<mode>) {
					Generalization::match<ValuePackage<
						BitCount::Constant::b_32(),
						BitCount::Constant::b_64()
					>>(
						bit_count,
						// NOTE : CLANG - compiler error if use same name
						[&] <auto index_, auto bit_count_> (ValuePackage<index_>, ValuePackage<bit_count_>) {
							auto value_integer = typename Parameter<bit_count_>::Value{};
							process_whole_integer<mode, bit_count_>(data, value_integer);
							value.allocate(k_type_size<typename Parameter<bit_count_>::Value>);
							OByteStreamView{value}.write(value_integer);
						}
					);
				}
			);
			return;
		}

		// ----------------

		inline static auto process (
			CByteListView const & data,
			ByteArray &           value,
			Mode const &          mode,
			BitCount const &      bit_count
		) -> Void {
			restruct(value);
			return process_whole(data, value, mode, bit_count);
		}

	};

}
