module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.common;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.tool.data.encryption.rijndael.encrypt;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnCryptData {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &  raw,
			OutputBinaryStreamView & ripe,
			String const &           key
		) -> Void {
			auto rijndael_raw_container = Binary{};
			auto rijndael_raw = InputBinaryStreamView{};
			auto rijndael_data_size = Math::compute_padded_size(raw.reserve(), cast_box<Size>(k_rijndael_block_size));
			if (raw.reserve() == rijndael_data_size) {
				rijndael_raw = raw.split_reserve();
			}
			else {
				rijndael_raw_container.allocate_full(rijndael_data_size);
				Range::assign_from(rijndael_raw_container.head(raw.reserve()), raw.split_forward(raw.reserve()).view());
				rijndael_raw = like_binary_stream<StreamMode::Constant::input()>(raw, rijndael_raw_container);
			}
			ripe.write_constant(k_magic_marker);
			auto rijndael_ripe = ripe.split_forward(rijndael_data_size);
			auto rijndael_key = compute_rijndael_key(key);
			auto rijndael_initialization_vector = compute_rijndael_initialization_vector(rijndael_key);
			Data::Encryption::Rijndael::Encrypt::process(rijndael_raw, rijndael_ripe, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_rijndael_block_size, rijndael_key, rijndael_initialization_vector);
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			ripe_size = 0_sz;
			ripe_size += binary_static_size_of<MagicMarker>();
			ripe_size += Math::compute_padded_size(raw_size, cast_box<Size>(k_rijndael_block_size));
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &  raw_,
			OutputBinaryStreamView & ripe_,
			String const &           key
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, key);
		}

		inline static auto estimate(
			Size const & raw_size,
			Size &       ripe_size
		) -> Void {
			restruct(ripe_size);
			return estimate_whole(raw_size, ripe_size);
		}

	};

}
