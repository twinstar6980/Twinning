module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encoding.base.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Data::Encoding::Base {

	M_enumeration(
		M_wrap(Mode),
		M_wrap(
			v16_std,
			v32_std,
			v32_hex,
			v64_std,
			v64_url,
		),
	);

	// ----------------

	struct Common {

		template <auto t_size> requires
			CategoryConstraint<>
			&& (IsSameOf<t_size, Size>)
		inline static constexpr auto generate_reverse_table(
			StaticArray<Byte, t_size> const & table,
			Boolean const & ignore_case
		) -> StaticArray<IntegerS32, 256_sz> {
			auto result = StaticArray<IntegerS32, 256_sz>{};
			for (auto & item : result) {
				item = -1_is32;
			}
			for (auto & index : SizeRange{table.size()}) {
				if (ignore_case && StringUtility::is_letter_character(cast_box<Character>(table[index]))) {
					result[cast_box<Size>(StringUtility::to_lower_case_character(cast_box<Character>(table[index])))] = cast_box<IntegerS32>(index);
					result[cast_box<Size>(StringUtility::to_upper_case_character(cast_box<Character>(table[index])))] = cast_box<IntegerS32>(index);
				}
				else {
					result[cast_box<Size>(table[index])] = cast_box<IntegerS32>(index);
				}
			}
			return result;
		}

		// ----------------

		inline static constexpr auto k_encode_table_v16_std = StaticArray<Byte, 16_sz>{{
			'0'_b, '1'_b, '2'_b, '3'_b, '4'_b, '5'_b, '6'_b, '7'_b, '8'_b, '9'_b,
			'A'_b, 'B'_b, 'C'_b, 'D'_b, 'E'_b, 'F'_b,
		}};

		inline static constexpr auto k_encode_table_v32_std = StaticArray<Byte, 32_sz>{{
			'A'_b, 'B'_b, 'C'_b, 'D'_b, 'E'_b, 'F'_b, 'G'_b, 'H'_b, 'I'_b,
			'J'_b, 'K'_b, 'L'_b, 'M'_b, 'N'_b, 'O'_b, 'P'_b, 'Q'_b, 'R'_b,
			'S'_b, 'T'_b, 'U'_b, 'V'_b, 'W'_b, 'X'_b, 'Y'_b, 'Z'_b,
			'2'_b, '3'_b, '4'_b, '5'_b, '6'_b, '7'_b,
		}};

		inline static constexpr auto k_encode_table_v32_hex = StaticArray<Byte, 32_sz>{{
			'0'_b, '1'_b, '2'_b, '3'_b, '4'_b, '5'_b, '6'_b, '7'_b, '8'_b, '9'_b,
			'A'_b, 'B'_b, 'C'_b, 'D'_b, 'E'_b, 'F'_b, 'G'_b, 'H'_b, 'I'_b, 'J'_b, 'K'_b,
			'L'_b, 'M'_b, 'N'_b, 'O'_b, 'P'_b, 'Q'_b, 'R'_b, 'S'_b, 'T'_b, 'U'_b, 'V'_b,
		}};

		inline static constexpr auto k_encode_table_v64_std = StaticArray<Byte, 64_sz>{{
			'A'_b, 'B'_b, 'C'_b, 'D'_b, 'E'_b, 'F'_b, 'G'_b, 'H'_b, 'I'_b, 'J'_b, 'K'_b, 'L'_b, 'M'_b,
			'N'_b, 'O'_b, 'P'_b, 'Q'_b, 'R'_b, 'S'_b, 'T'_b, 'U'_b, 'V'_b, 'W'_b, 'X'_b, 'Y'_b, 'Z'_b,
			'a'_b, 'b'_b, 'c'_b, 'd'_b, 'e'_b, 'f'_b, 'g'_b, 'h'_b, 'i'_b, 'j'_b, 'k'_b, 'l'_b, 'm'_b,
			'n'_b, 'o'_b, 'p'_b, 'q'_b, 'r'_b, 's'_b, 't'_b, 'u'_b, 'v'_b, 'w'_b, 'x'_b, 'y'_b, 'z'_b,
			'0'_b, '1'_b, '2'_b, '3'_b, '4'_b, '5'_b, '6'_b, '7'_b, '8'_b, '9'_b,
			'+'_b, '/'_b,
		}};

		inline static constexpr auto k_encode_table_v64_url = StaticArray<Byte, 64_sz>{{
			'A'_b, 'B'_b, 'C'_b, 'D'_b, 'E'_b, 'F'_b, 'G'_b, 'H'_b, 'I'_b, 'J'_b, 'K'_b, 'L'_b, 'M'_b,
			'N'_b, 'O'_b, 'P'_b, 'Q'_b, 'R'_b, 'S'_b, 'T'_b, 'U'_b, 'V'_b, 'W'_b, 'X'_b, 'Y'_b, 'Z'_b,
			'a'_b, 'b'_b, 'c'_b, 'd'_b, 'e'_b, 'f'_b, 'g'_b, 'h'_b, 'i'_b, 'j'_b, 'k'_b, 'l'_b, 'm'_b,
			'n'_b, 'o'_b, 'p'_b, 'q'_b, 'r'_b, 's'_b, 't'_b, 'u'_b, 'v'_b, 'w'_b, 'x'_b, 'y'_b, 'z'_b,
			'0'_b, '1'_b, '2'_b, '3'_b, '4'_b, '5'_b, '6'_b, '7'_b, '8'_b, '9'_b,
			'-'_b, '_'_b,
		}};

	};

}
