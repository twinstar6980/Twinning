#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/wwise/sound_bank/version.hpp"

namespace TwinKleS::Core::Tool::Wwise::SoundBank::Manifest {

	template <auto t_version>
	struct HIRC;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct HIRC<t_version> {
		Integer  id;
		Integer  type;
		ByteList data;
	};

	// ----------------

	template <auto t_version>
	struct SoundBank;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct SoundBank<t_version> {
		Integer                         id;
		ByteList                        header_expand;
		Optional<ByteList>              stmg;
		Optional<List<Integer>>         embedded_audio;
		Optional<List<HIRC<t_version>>> hirc;
		Optional<List<String>>          referenced_sound_bank;
		Optional<ByteList>              envs;
	};

	// ----------------

	using SoundBankVariant = VariantOfVersion<VersionEnum, SoundBank>;

}

namespace TwinKleS::Core::JSON {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Manifest::HIRC<t_version>),
		M_wrap(id, type, data),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_json_value_adapter_for_aggregate_as_object_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Manifest::SoundBank<t_version>),
		M_wrap(id, header_expand, stmg, embedded_audio, hirc, referenced_sound_bank, envs),
	);

}
