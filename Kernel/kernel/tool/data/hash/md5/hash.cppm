module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.hash.md5.hash;
import twinning.kernel.utility;
import twinning.kernel.tool.data.hash.md5.common;
import twinning.kernel.third.cpp_md5_lib;

export namespace Twinning::Kernel::Tool::Data::Hash::Md5 {

	struct Hash :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole (
			ConstantByteListView const & data,
			ByteArray &                  value
		) -> Void {
			auto generator = Third::cpp_md5_lib::MD5{};
			for (auto part_position = k_none_size; part_position < data.size();) {
				auto part_size = minimum(data.size() - part_position, 0x7FFFFFFF_sz);
				generator.update(cast_pointer<unsigned char>(data.begin() + part_position).value, static_cast<unsigned int>(part_size.value));
				part_position += part_size;
			}
			generator.finalize();
			value.assign(ConstantByteListView{cast_pointer<Byte>(make_pointer(generator.get_digest())), 16_sz});
			return;
		}

		// ----------------

		inline static auto process (
			ConstantByteListView const & data,
			ByteArray &                  value
		) -> Void {
			restruct(value);
			return process_whole(data, value);
		}

	};

}
