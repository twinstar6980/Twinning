module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::Popcap::ResourceStreamGroup {

	template <auto t_version> requires (check_version(t_version, {}))
	struct Definition {

		#pragma region resource

		M_nested_template_declaration(GeneralResourceAdditional);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(GeneralResourceAdditional),
			M_wrap(
			),
		);

		// ----------------

		M_nested_template_declaration(TextureResourceAdditional);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(TextureResourceAdditional),
			M_wrap(
				(Integer) index,
				(Size2<Integer>) size,
			),
		);

		// ----------------

		M_nested_template_declaration(Resource);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Resource),
			M_wrap(
				(Path) path,
				(EnumerableVariant<ResourceType, GeneralResourceAdditional, TextureResourceAdditional>) additional,
			),
		);

		#pragma endregion

		#pragma region package

		M_nested_template_declaration(Package);

		M_nested_template_definition_check(check_version(t_version, {1}))
		M_record_of_map(
			M_nested_template_definition_name(Package),
			M_wrap(
				(PacketCompression) compression,
				(List<Resource>) resource,
			),
		);

		#pragma endregion

	};

}
