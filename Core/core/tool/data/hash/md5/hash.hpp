#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/data/hash/md5/common.hpp"
#include "core/third/md5.hpp"

namespace TwinStar::Core::Tool::Data::Hash::MD5 {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		static auto process_whole (
			CByteListView const & data,
			ByteArray &           value
		) -> Void {
			auto generator = Third::md5::MD5{std::span{cast_pointer<Third::md5::byte>(data.begin()).value, data.size().value}};
			value.assign(CByteListView{cast_pointer<Byte>(make_pointer(generator.getDigest())), 16_sz});
			return;
		}

		// ----------------

		static auto process (
			CByteListView const & data,
			ByteArray &           value
		) -> Void {
			restruct(value);
			return process_whole(data, value);
		}

	};

}
