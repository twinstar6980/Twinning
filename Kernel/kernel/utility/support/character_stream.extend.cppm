module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.character_stream.extend;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.string.parser;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.miscellaneous.character_series.stream;
import twinning.kernel.utility.miscellaneous.character_series.stream_adapter;
import twinning.kernel.utility.miscellaneous.number_variant;
import twinning.kernel.utility.null;

export namespace Twinning::Kernel {

	#pragma region null

	template <>
	struct CharacterStreamAdapter<Null> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = Null;

		// ----------------

		inline static auto write (
			ThisOutput & thix,
			That const & that
		) -> Void {
			StringParser::write_null(thix, that);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			StringParser::read_null(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region boolean

	template <>
	struct CharacterStreamAdapter<Boolean> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = Boolean;

		// ----------------

		inline static auto write (
			ThisOutput & thix,
			That const & that
		) -> Void {
			StringParser::write_boolean(thix, that);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			StringParser::read_boolean(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region number

	template <>
	struct CharacterStreamAdapter<Integer> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = Integer;

		// ----------------

		inline static auto write (
			ThisOutput &    thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			StringParser::read_number(thix, that);
			return;
		}

	};

	template <>
	struct CharacterStreamAdapter<Floater> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = Floater;

		// ----------------

		inline static auto write (
			ThisOutput &    thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			StringParser::read_number(thix, that);
			return;
		}

	};

	// ----------------

	template <>
	struct CharacterStreamAdapter<NumberVariant> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = NumberVariant;

		// ----------------

		inline static auto write (
			ThisOutput &    thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			StringParser::read_number(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region byte

	template <>
	struct CharacterStreamAdapter<Byte> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = Byte;

		// ----------------

		inline static auto write (
			ThisOutput & thix,
			That const & that
		) -> Void {
			StringParser::write_byte(thix, that);
			return;
		}

		inline static auto read (
			ThisInput & thix,
			That &      that
		) -> Void {
			StringParser::read_byte(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region byte list

	template <auto t_constant> requires
		AutomaticConstraint
	struct CharacterStreamAdapter<ByteListView<t_constant>> {

		using ThisInput = InputCharacterStreamView;

		using ThisOutput = OutputCharacterStreamView;

		using That = ByteListView<t_constant>;

		// ----------------

		inline static auto write (
			ThisOutput & thix,
			That const & that
		) -> Void {
			StringParser::write_byte_list(thix, that);
			return;
		}

		inline static auto read (
			ThisInput &  thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
			StringParser::read_byte_list(thix, that);
			return;
		}

	};

	#pragma endregion

}
