module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_bundle.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::Popcap::ResourceStreamBundle {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Definition {

		#pragma region resource

		M_nested_template_declaration(GeneralResourceAdditional);

		M_nested_template_definition_check(check_version(t_version, {1}, {}))
		M_record_of_map(
			M_nested_template_definition_name(GeneralResourceAdditional),
			M_wrap(
			),
		);

		// ----------------

		M_nested_template_declaration(TextureResourceAdditional);

		M_nested_template_definition_check(check_version(t_version, {1}, {0, 1}))
		M_record_of_map(
			M_nested_template_definition_name(TextureResourceAdditional),
			M_wrap(
				(Size2<Integer>) size,
				(Integer) format,
				(Integer) pitch,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {4}, {1, 2}))
		M_record_of_map(
			M_nested_template_definition_name(TextureResourceAdditional),
			M_wrap(
				(Size2<Integer>) size,
				(Integer) format,
				(Integer) pitch,
				(Integer) additional_byte_count,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {4}, {2}))
		M_record_of_map(
			M_nested_template_definition_name(TextureResourceAdditional),
			M_wrap(
				(Size2<Integer>) size,
				(Integer) format,
				(Integer) pitch,
				(Integer) additional_byte_count,
				(Integer) scale,
			),
		);

		// ----------------

		M_nested_template_declaration(Resource);

		M_nested_template_definition_check(check_version(t_version, {1}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Resource),
			M_wrap(
				(Path) path,
				(EnumerableVariant<ResourceType, GeneralResourceAdditional, TextureResourceAdditional>) additional,
			),
		);

		#pragma endregion

		#pragma region group

		M_nested_template_declaration(SubgroupCategory);

		M_nested_template_definition_check(check_version(t_version, {1, 3}, {}))
		M_record_of_map(
			M_nested_template_definition_name(SubgroupCategory),
			M_wrap(
				(Optional<Integer>) resolution,
			),
		);

		M_nested_template_definition_check(check_version(t_version, {3}, {}))
		M_record_of_map(
			M_nested_template_definition_name(SubgroupCategory),
			M_wrap(
				(Optional<Integer>) resolution,
				(Optional<String>) locale,
			),
		);

		// ----------------

		M_nested_template_declaration(Subgroup);

		M_nested_template_definition_check(check_version(t_version, {1}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Subgroup),
			M_wrap(
				(String) identifier,
				(SubgroupCategory) category,
				(PacketCompression) compression,
				(List<Resource>) resource,
			),
		);

		// ----------------

		M_nested_template_declaration(Group);

		M_nested_template_definition_check(check_version(t_version, {1}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Group),
			M_wrap(
				(String) identifier,
				(Boolean) composite,
				(List<Subgroup>) subgroup,
			),
		);

		#pragma endregion

		#pragma region package

		M_nested_template_declaration(Package);

		M_nested_template_definition_check(check_version(t_version, {1}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Package),
			M_wrap(
				(List<Group>) group,
			),
		);

		#pragma endregion

	};

}
