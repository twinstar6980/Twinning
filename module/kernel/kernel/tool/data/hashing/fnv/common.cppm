module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hashing.fnv.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Data::Hashing::Fnv {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			v0,
			v1,
			v1a,
		),
	);

	// ----------------

	struct Common {

		template <auto t_length>
		struct Parameter;

		template <>
		struct Parameter<32_sz> {

			using Value = IntegerU32;

			// ----------------

			inline static constexpr auto offset = Value{2166136261_iu32};

			inline static constexpr auto prime = Value{16777619_iu32};

		};

		template <>
		struct Parameter<64_sz> {

			using Value = IntegerU64;

			// ----------------

			inline static constexpr auto offset = Value{14695981039346656037_iu64};

			inline static constexpr auto prime = Value{1099511628211_iu64};

		};

	};

}
