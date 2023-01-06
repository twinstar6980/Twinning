#pragma once

#include "core/utility/utility.hpp"

namespace TwinStar::Core::Tool::Data::Hash::FNV {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			m_1,
			m_1a,
		),
	);

	M_enumeration(
		M_wrap(BitCount),
		M_wrap(
			b_32,
			b_64,
		),
	);

	// ----------------

	struct HashCommon {

	protected:

		template <auto t_bit_count>
		struct Parameter;

		template <>
		struct Parameter<BitCount::Constant::b_32()> {
			using Value = IntegerU32;
			inline static constexpr auto offset = Value{2166136261_iu32};
			inline static constexpr auto prime = Value{16777619_iu32};
		};

		template <>
		struct Parameter<BitCount::Constant::b_64()> {
			using Value = IntegerU64;
			inline static constexpr auto offset = Value{14695981039346656037_iu64};
			inline static constexpr auto prime = Value{1099511628211_iu64};
		};

	};

	struct Hash :
		HashCommon {

	protected:

		template <auto mode, auto bit_count> requires
			CategoryConstraint<>
			&& (IsSameV<mode, Mode>)
			&& (IsSameV<bit_count, BitCount>)
		static auto process_whole_integer (
			CByteListView const &                  data,
			typename Parameter<bit_count>::Value & value
		) -> Void {
			using Parameter = Parameter<bit_count>;
			value = Parameter::offset;
			for (auto & element : data) {
				if constexpr (mode == Mode::Constant::m_1()) {
					value *= Parameter::prime;
					value ^= cbw<typename Parameter::Value>(element);
				}
				if constexpr (mode == Mode::Constant::m_1a()) {
					value ^= cbw<typename Parameter::Value>(element);
					value *= Parameter::prime;
				}
			}
			return;
		}

		static auto process_whole (
			CByteListView const & data,
			ByteArray &           value,
			Mode const &          mode,
			BitCount const &      bit_count
		) -> Void {
			Generalization::match<ValuePackage<
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
						// todo : compiler error if use same name
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

	public:

		static auto do_process_whole (
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
