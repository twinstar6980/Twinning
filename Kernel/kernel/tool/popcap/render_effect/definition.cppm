module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.render_effect.version;

export namespace Twinning::Kernel::Tool::PopCap::RenderEffect {

	template <auto version> requires (check_version(version, {}, {}))
	struct Definition {

		#pragma region block

		template <typename = None>
		struct Block1_;

		using Block1 = Block1_<>;

		template <typename _> requires (check_version(version, {1}, {1}))
		M_record_of_map(
			M_wrap(Block1_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
				(Integer) unknown_6,
			),
		);

		// ----------------

		template <typename = None>
		struct Block2_;

		using Block2 = Block2_<>;

		template <typename _> requires (check_version(version, {1}, {1, 2}))
		M_record_of_map(
			M_wrap(Block2_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
			),
		);

		template <typename _> requires (check_version(version, {1}, {2}))
		M_record_of_map(
			M_wrap(Block2_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
			),
		);

		// ----------------

		template <typename = None>
		struct Block3_;

		using Block3 = Block3_<>;

		template <typename _> requires (check_version(version, {1}, {1}))
		M_record_of_map(
			M_wrap(Block3_<_>),
			M_wrap(
				(Integer) unknown_2,
				(String) string,
			),
		);

		// ----------------

		template <typename = None>
		struct Block4_;

		using Block4 = Block4_<>;

		template <typename _> requires (check_version(version, {1}, {1}))
		M_record_of_map(
			M_wrap(Block4_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		// ----------------

		template <typename = None>
		struct Block5_;

		using Block5 = Block5_<>;

		template <typename _> requires (check_version(version, {1}, {1}))
		M_record_of_map(
			M_wrap(Block5_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
				(Integer) unknown_6,
				(Integer) unknown_7,
			),
		);

		// ----------------

		template <typename = None>
		struct Block6_;

		using Block6 = Block6_<>;

		template <typename _> requires (check_version(version, {1}, {1}))
		M_record_of_map(
			M_wrap(Block6_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		// ----------------

		template <typename = None>
		struct Block7_;

		using Block7 = Block7_<>;

		template <typename _> requires (check_version(version, {1}, {3}))
		M_record_of_map(
			M_wrap(Block7_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
			),
		);

		// ----------------

		template <typename = None>
		struct Block8_;

		using Block8 = Block8_<>;

		template <typename _> requires (check_version(version, {1}, {1, 3}))
		M_record_of_map(
			M_wrap(Block8_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
			),
		);

		template <typename _> requires (check_version(version, {1}, {3}))
		M_record_of_map(
			M_wrap(Block8_<_>),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		#pragma endregion

		#pragma region effect

		template <typename = None>
		struct Effect_;

		using Effect = Effect_<>;

		template <typename _> requires (check_version(version, {1}, {1, 3}))
		M_record_of_map(
			M_wrap(Effect_<_>),
			M_wrap(
				(List<Block1>) block_1,
				(List<Block2>) block_2,
				(List<Block3>) block_3,
				(List<Block4>) block_4,
				(List<Block5>) block_5,
				(List<Block6>) block_6,
				(List<Block8>) block_8,
			),
		);

		template <typename _> requires (check_version(version, {1}, {3}))
		M_record_of_map(
			M_wrap(Effect_<_>),
			M_wrap(
				(List<Block1>) block_1,
				(List<Block2>) block_2,
				(List<Block3>) block_3,
				(List<Block4>) block_4,
				(List<Block5>) block_5,
				(List<Block6>) block_6,
				(List<Block7>) block_7,
				(List<Block8>) block_8,
			),
		);

		#pragma endregion

	};

}
