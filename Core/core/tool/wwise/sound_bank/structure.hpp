#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/wwise/sound_bank/version.hpp"
#include "core/tool/common/wave_structure.hpp"

namespace TwinKleS::Core::Tool::Wwise::SoundBank::Structure {

	using WaveStructure::ChunkSign;

	namespace ChunkSignFlag {
		inline constexpr auto bkhd = FourCC{"BKHD"_fourcc};
		inline constexpr auto didx = FourCC{"DIDX"_fourcc};
		inline constexpr auto data = FourCC{"DATA"_fourcc};
		inline constexpr auto stmg = FourCC{"STMG"_fourcc};
		inline constexpr auto hirc = FourCC{"HIRC"_fourcc};
		inline constexpr auto stid = FourCC{"STID"_fourcc};
		inline constexpr auto envs = FourCC{"ENVS"_fourcc};
		inline constexpr auto fxpr = FourCC{"FXPR"_fourcc};
	}

	// ----------------

	using VersionNumber = IntegerU32;

	// ----------------

	template <auto t_version>
	struct BKHDBegin;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct BKHDBegin<t_version> {
		VersionNumber       version;
		IntegerU32          id;
		PaddingBlock<8_szz> unused_1;
	};

	// ----------------

	template <auto t_version>
	struct DIDXItem;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct DIDXItem<t_version> {
		IntegerU32 id;
		IntegerU32 offset;
		IntegerU32 size;
	};

	// ----------------

	template <auto t_version>
	struct STIDBegin;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct STIDBegin<t_version> {
		IntegerU32 unknown_1;
		IntegerU32 item_count;
	};

	// ----------------

	template <auto t_version>
	struct STIDItem;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct STIDItem<t_version> {
		IntegerU32   id;
		StringBlock8 name;
	};

	// ----------------

	template <auto t_version>
	struct HIRCBegin;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct HIRCBegin<t_version> {
		IntegerU32 item_count;
	};

	// ----------------

	template <auto t_version>
	struct HIRCItemBegin;

	template <auto t_version> requires
		CategoryConstraint<>
		&& (check_version(t_version, {112}))
	struct HIRCItemBegin<t_version> {
		IntegerU8  type;
		IntegerU32 size;
		IntegerU32 id;
	};

	// ----------------

	inline constexpr auto k_data_block_padding_size = Size{0x10_sz};

}

namespace TwinKleS::Core {

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Structure::BKHDBegin<t_version>),
		M_wrap(version, id, unused_1),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Structure::DIDXItem<t_version>),
		M_wrap(id, offset, size),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Structure::STIDBegin<t_version>),
		M_wrap(unknown_1, item_count),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Structure::STIDItem<t_version>),
		M_wrap(id, name),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Structure::HIRCBegin<t_version>),
		M_wrap(item_count),
	);

	// ----------------

	template <auto t_version> requires
		AutoConstraint
		&& (Tool::Wwise::SoundBank::check_version(t_version, {112}))
	M_byte_stream_adapter_for_aggregate_by_field_of(
		M_wrap(Tool::Wwise::SoundBank::Structure::HIRCItemBegin<t_version>),
		M_wrap(type, size, id),
	);

}
