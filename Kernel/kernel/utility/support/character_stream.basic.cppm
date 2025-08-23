module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.character_stream.basic;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.miscellaneous.character_series.stream;
import twinning.kernel.utility.miscellaneous.character_series.stream_adapter;
import twinning.kernel.utility.string.string;

export namespace Twinning::Kernel {

	#pragma region character

	template <>
	struct CharacterStreamAdapter<Character> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = Character;

		// ----------------

		inline static auto write (
			ThisOutput & thix,
			That const & that
		) -> Void {
			up_cast<OutputStreamView<Character, BasicCharacterListView>>(thix).write(that);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			up_cast<InputStreamView<Character, BasicCharacterListView>>(thix).read(that);
			return;
		}

	};

	#pragma endregion

	#pragma region string

	template <auto t_constant> requires
		AutoConstraint
	struct CharacterStreamAdapter<StringView<t_constant>> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = StringView<t_constant>;

		// ----------------

		inline static auto write (
			ThisOutput & thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				up_cast<OutputStreamView<Character, BasicCharacterListView>>(thix).write(element);
			}
			return;
		}

		inline static auto read (
			ThisInput &  thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
			for (auto & element : that) {
				up_cast<InputStreamView<Character, BasicCharacterListView>>(thix).read(element);
			}
			return;
		}

	};

	#pragma endregion

}
