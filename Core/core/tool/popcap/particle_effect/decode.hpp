#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/particle_effect/common.hpp"
#include "core/tool/common/byte_stream.hpp"

namespace TwinStar::Core::Tool::PopCap::ParticleEffect {

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		Common<version>,
		CommonByteStreamExchangeForInput {

		using Common = Common<version>;

		using typename Common::Definition;

		using typename Common::VersionNumber;

		// ----------------

		static auto exchange_effect (
			IByteStreamView &             data,
			typename Definition::Effect & value
		) -> Void {
			return;
		}

		// ----------------

		static auto process_whole (
			IByteStreamView &             data,
			typename Definition::Effect & definition
		) -> Void {
			throw IncompleteException{};
			return;
		}

		// ----------------

		static auto process (
			IByteStreamView &             data_,
			typename Definition::Effect & definition
		) -> Void {
			M_use_zps_of(data);
			restruct(definition);
			return process_whole(data, definition);
		}

	};

}
