module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hash.fnv.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Data::Hash::FNV {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			m_0,
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

	struct Common {

		template <auto t_bit_count>
		struct Parameter;

		template <>
		struct Parameter<BitCount::Constant::b_32()> {

			using Value = IntegerU32;

			// ----------------

			inline static constexpr auto offset = Value{2166136261_iu32};

			inline static constexpr auto prime = Value{16777619_iu32};

		};

		template <>
		struct Parameter<BitCount::Constant::b_64()> {

			using Value = IntegerU64;

			// ----------------

			inline static constexpr auto offset = Value{14695981039346656037_iu64};

			inline static constexpr auto prime = Value{1099511628211_iu64};

		};

	};

}
