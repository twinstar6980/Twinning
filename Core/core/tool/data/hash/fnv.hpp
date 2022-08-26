#pragma once

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::Data::Hash::FNV {

	inline namespace Common {

		namespace Detail {

			#pragma region parameter

			enum class Method : ZIntegerU8 {
				m_1,
				m_1a,
			};

			enum class BitCount : ZIntegerU8 {
				b_32,
				b_64,
			};

			// ----------------

			template <auto t_bit_count> requires
				CategoryConstraint<>
				&& (IsSameV<t_bit_count, BitCount>)
			struct Parameter;

			template <>
			struct Parameter<BitCount::b_32> {
				using Value = IntegerU32;
				inline static constexpr auto offset = Value{2166136261_iu32};
				inline static constexpr auto prime = Value{16777619_iu32};
			};

			template <>
			struct Parameter<BitCount::b_64> {
				using Value = IntegerU64;
				inline static constexpr auto offset = Value{14695981039346656037_iu64};
				inline static constexpr auto prime = Value{1099511628211_iu64};
			};

			#pragma endregion

		}

		using Detail::Method;

		using Detail::BitCount;

	}

	namespace Hash {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			template <auto method, auto bit_count> requires
				CategoryConstraint<>
				&& (IsSameV<method, Method>)
				&& (IsSameV<bit_count, BitCount>)
			inline auto process (
				CByteListView const & data
			) -> typename Parameter<bit_count>::Value {
				auto result = Parameter<bit_count>::offset;
				for (auto & element : data) {
					if constexpr (method == Method::m_1) {
						result *= Parameter<bit_count>::prime;
						result ^= cbw<typename Parameter<bit_count>::Value>(element);
					} else if constexpr (method == Method::m_1a) {
						result ^= cbw<typename Parameter<bit_count>::Value>(element);
						result *= Parameter<bit_count>::prime;
					}
				}
				return result;
			}

			// ----------------

			inline auto process_1_32 (
				CByteListView const & data
			) -> Parameter<BitCount::b_32>::Value {
				return process<Method::m_1, BitCount::b_32>(data);
			}

			inline auto process_1_64 (
				CByteListView const & data
			) -> Parameter<BitCount::b_64>::Value {
				return process<Method::m_1, BitCount::b_64>(data);
			}

			inline auto process_1a_32 (
				CByteListView const & data
			) -> Parameter<BitCount::b_32>::Value {
				return process<Method::m_1a, BitCount::b_32>(data);
			}

			inline auto process_1a_64 (
				CByteListView const & data
			) -> Parameter<BitCount::b_64>::Value {
				return process<Method::m_1a, BitCount::b_64>(data);
			}

			#pragma endregion

		}

		using Detail::process;

		using Detail::process_1_32;

		using Detail::process_1_64;

		using Detail::process_1a_32;

		using Detail::process_1a_64;

	}

}
