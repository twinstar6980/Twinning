module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.decrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.common;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.tool.data.encryption.rijndael.decrypt;

export namespace Twinning::Kernel::Tool::Miscellaneous::Pvz2cnCryptData {

	struct Decrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			OutputByteStreamView & raw,
			InputByteStreamView &  ripe,
			String const &         key
		) -> Void {
			auto rijndael_data_size = Size{};
			estimate_whole(rijndael_data_size, ripe.reserve());
			ripe.read_constant(k_magic_marker);
			auto rijndael_ripe = InputByteStreamView{ripe.forward_view(rijndael_data_size)};
			auto rijndael_raw = OutputByteStreamView{raw.forward_view(rijndael_data_size)};
			auto rijndael_key = compute_rijndael_key(key);
			auto rijndael_initialization_vector = compute_rijndael_initialization_vector(rijndael_key);
			Data::Encryption::Rijndael::Decrypt::process(rijndael_raw, rijndael_ripe, Data::Encryption::Rijndael::Mode::Constant::cbc(), k_rijndael_block_size, rijndael_key, rijndael_initialization_vector);
			return;
		}

		// ----------------

		inline static auto estimate_whole(
			Size &       raw_size,
			Size const & ripe_size
		) -> Void {
			raw_size = 0_sz;
			assert_test(ripe_size >= bs_static_size<MagicMarker>());
			auto rijndael_data_size = ripe_size - bs_static_size<MagicMarker>();
			assert_test(Math::is_padded_size(rijndael_data_size, cast_box<Size>(k_rijndael_block_size)));
			raw_size += rijndael_data_size;
			return;
		}

		// ----------------

		inline static auto process(
			OutputByteStreamView & raw_,
			InputByteStreamView &  ripe_,
			String const &         key
		) -> Void {
			M_use_zps_of(raw);
			M_use_zps_of(ripe);
			return process_whole(raw, ripe, key);
		}

		inline static auto estimate(
			Size &       raw_size,
			Size const & ripe_size
		) -> Void {
			restruct(raw_size);
			return estimate_whole(raw_size, ripe_size);
		}

	};

}
