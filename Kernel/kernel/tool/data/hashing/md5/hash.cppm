module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hashing.md5.hash;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hashing.md5.common;
import twinning.kernel.third.cpp_md5_lib;

export namespace Twinning::Kernel::Tool::Data::Hashing::Md5 {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			ConstantByteListView const & data,
			ByteArray &                  value
		) -> Void {
			auto generator = Third::cpp_md5_lib::MD5{};
			for (auto part_position = 0_sz; part_position < data.size();) {
				auto part_size = Math::minimum(data.size() - part_position, 0x7FFFFFFF_sz);
				generator.update(unmake_pointer_unsafe<unsigned char>(data.begin() + part_position), unmake_box<unsigned int>(part_size));
				part_position += part_size;
			}
			generator.finalize();
			value.assign(ConstantByteListView{make_pointer_unsafe<Byte>(generator.get_digest()), 16_sz});
			return;
		}

		// ----------------

		inline static auto process(
			ConstantByteListView const & data,
			ByteArray &                  value
		) -> Void {
			restruct(value);
			return process_whole(data, value);
		}

	};

}
