module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.string_stream;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.string.basic_string_stream_view;
import twinning.kernel.utility.string.basic_string_stream_flag;
import twinning.kernel.utility.string.basic_string_stream_adapter;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.string;

export namespace Twinning::Kernel {

	#pragma region character

	template <>
	struct BasicStringStreamAdapter<Character, Character> {

		using ThisInput = InputStringStreamView;

		using ThisOutput = OutputStringStreamView;

		using That = Character;

		// ----------------

		inline static auto capacity(
			That const & that
		) -> Size {
			return 1_sz;
		}

		inline static auto write(
			ThisOutput & thix,
			That const & that
		) -> Void {
			up_cast<OutputStreamView<Character, BasicStringView, BasicStringStreamFlag>>(thix).write(that);
			return;
		}

		inline static auto read(
			ThisInput & thix,
			That &      that
		) -> Void {
			up_cast<InputStreamView<Character, BasicStringView, BasicStringStreamFlag>>(thix).read(that);
			return;
		}

	};

	#pragma endregion

}
