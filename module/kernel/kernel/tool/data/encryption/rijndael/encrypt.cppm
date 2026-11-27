module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.encryption.rijndael.encrypt;
import twinning.kernel.utility;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.dependency.Rijndael;

export namespace Twinning::Kernel::Tool::Data::Encryption::Rijndael {

	struct Encrypt :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_whole(
			InputBinaryStreamView &    raw,
			OutputBinaryStreamView &   ripe,
			Mode const &               mode,
			Integer const &            block_size,
			ConstantBinaryView const & key,
			ConstantBinaryView const & initialization_vector
		) -> Void {
			assert_test(Math::is_padded_size(raw.reserve(), cast_box<Size>(block_size)));
			assert_test(is_valid_block_size(block_size));
			assert_test(is_valid_block_size(cast_box<Integer>(key.size())));
			assert_test(Math::is_padded_size(binary_size_of(initialization_vector) * type_bit_size_of<Character>(), cast_box<Size>(block_size)));
			auto initialization_vector_view = mode == Mode::Constant::ecb() ? (ConstantBinaryView{k_empty_initialization_vector.data(), cast_box<Size>(block_size)}) : (initialization_vector);
			auto raw_size = raw.reserve();
			if (raw_size != 0_sz) {
				auto rijndael = Dependency::Rijndael::CRijndael{};
				rijndael.MakeKey(unmake_pointer_unsafe<char>(key.data()), unmake_pointer_unsafe<char>(initialization_vector_view.data()), unmake_box<int>(key.size()), unmake_box<int>(block_size));
				rijndael.Encrypt(unmake_pointer_unsafe<char>(raw.current()), unmake_pointer_unsafe<char>(ripe.current()), unmake_box<std::size_t>(raw.reserve()), unmake_box<int>(mode));
			}
			raw.forward(raw_size);
			ripe.forward(raw_size);
			return;
		}

		// ----------------

		inline static auto process(
			InputBinaryStreamView &    raw_,
			OutputBinaryStreamView &   ripe_,
			Mode const &               mode,
			Integer const &            block_size,
			ConstantBinaryView const & key,
			ConstantBinaryView const & initialization_vector
		) -> Void {
			M_use_split_stream_of(raw);
			M_use_split_stream_of(ripe);
			return process_whole(raw, ripe, mode, block_size, key, initialization_vector);
		}

	};

}
