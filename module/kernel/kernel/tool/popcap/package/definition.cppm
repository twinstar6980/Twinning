module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.package.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.package.version;

export namespace Twinning::Kernel::Tool::Popcap::Package {

	template <auto t_version> requires (check_version(t_version, {}, {}))
	struct Definition {

		#pragma region resource

		M_nested_template_declaration(Resource);

		M_nested_template_definition_check(check_version(t_version, {0}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Resource),
			M_wrap(
				(Path) path,
				(Integer) time,
			),
		);

		#pragma endregion

		#pragma region package

		M_nested_template_declaration(Package);

		M_nested_template_definition_check(check_version(t_version, {0}, {}))
		M_record_of_map(
			M_nested_template_definition_name(Package),
			M_wrap(
				(List<Resource>) resource,
			),
		);

		#pragma endregion

	};

}
