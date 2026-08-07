module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.render_effect.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.render_effect.version;

export namespace Twinning::Kernel::Tool::Popcap::RenderEffect {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Definition {

		#pragma region block

		M_nested_template_declaration(Block1);

		M_nested_template_definition_check(check_version(t_version, {1}, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Block1),
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

		M_nested_template_declaration(Block2);

		M_nested_template_definition_check(check_version(t_version, {1}, {1, 2}))
		M_record_of_map(
			M_nested_template_definition_name(Block2),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {1}, {2}))
		M_record_of_map(
			M_nested_template_definition_name(Block2),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
			),
		);

		// ----------------

		M_nested_template_declaration(Block3);

		M_nested_template_definition_check(check_version(t_version, {1}, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Block3),
			M_wrap(
				(Integer) unknown_2,
				(String) string,
			),
		);

		// ----------------

		M_nested_template_declaration(Block4);

		M_nested_template_definition_check(check_version(t_version, {1}, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Block4),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		// ----------------

		M_nested_template_declaration(Block5);

		M_nested_template_definition_check(check_version(t_version, {1}, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Block5),
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

		M_nested_template_declaration(Block6);

		M_nested_template_definition_check(check_version(t_version, {1}, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Block6),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
				(Integer) unknown_4,
				(Integer) unknown_5,
			),
		);

		// ----------------

		M_nested_template_declaration(Block7);

		M_nested_template_definition_check(check_version(t_version, {1}, {3}))
		M_record_of_map(
			M_nested_template_definition_name(Block7),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
			),
		);

		// ----------------

		M_nested_template_declaration(Block8);

		M_nested_template_definition_check(check_version(t_version, {1}, {1, 3}))
		M_record_of_map(
			M_nested_template_definition_name(Block8),
			M_wrap(
				(Integer) unknown_1,
				(Integer) unknown_2,
				(Integer) unknown_3,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {1}, {3}))
		M_record_of_map(
			M_nested_template_definition_name(Block8),
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

		M_nested_template_declaration(Effect);

		M_nested_template_definition_check(check_version(t_version, {1}, {1, 3}))
		M_record_of_map(
			M_nested_template_definition_name(Effect),
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

		M_nested_template_definition_check(check_version(t_version, {1}, {3}))
		M_record_of_map(
			M_nested_template_definition_name(Effect),
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
