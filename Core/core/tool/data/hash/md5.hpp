#pragma once

#include "core/utility/utility.hpp"
#include "core/third_party/md5.hpp"

namespace TwinStar::Core::Tool::Data::Hash::MD5 {

	struct HashCommon {

	protected:

	};

	struct Hash :
		HashCommon {

	protected:

		static auto process_whole (
			CByteListView const & data,
			ByteArray &           value
		) -> Void {
			auto generator = ThirdParty::md5::MD5{std::span{cast_pointer<ThirdParty::md5::byte>(data.begin()).value, data.size().value}};
			value.assign(CByteListView{cast_pointer<Byte>(make_pointer(generator.getDigest())), 16_sz});
			return;
		}

	public:

		static auto do_process_whole (
			CByteListView const & data,
			ByteArray &           value
		) -> Void {
			restruct(value);
			return process_whole(data, value);
		}

	};

}
