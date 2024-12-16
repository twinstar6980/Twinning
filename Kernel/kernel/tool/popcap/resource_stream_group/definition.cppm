module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.utility;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.shared;

export namespace Twinning::Kernel::Tool::PopCap::ResourceStreamGroup {

	template <auto version> requires (check_version(version, {}))
	struct Definition {

		#pragma region resource

		template <typename = None>
		struct GeneralResourceAdditional_;

		using GeneralResourceAdditional = GeneralResourceAdditional_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(GeneralResourceAdditional_<_>),
			M_wrap(
			),
		);

		// ----------------

		template <typename = None>
		struct TextureResourceAdditional_;

		using TextureResourceAdditional = TextureResourceAdditional_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(TextureResourceAdditional_<_>),
			M_wrap(
				(Integer) index,
				(Size2<Integer>) size,
			),
		);

		// ----------------

		template <typename = None>
		struct Resource_;

		using Resource = Resource_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(Resource_<_>),
			M_wrap(
				(Path) path,
				(EnumerableVariant<ResourceType, GeneralResourceAdditional, TextureResourceAdditional>) additional,
			),
		);

		#pragma endregion

		#pragma region package

		template <typename = None>
		struct Package_;

		using Package = Package_<>;

		template <typename _> requires (check_version(version, {1}))
		M_record_of_map(
			M_wrap(Package_<_>),
			M_wrap(
				(PacketCompression) compression,
				(List<Resource>) resource,
			),
		);

		#pragma endregion

	};

}
