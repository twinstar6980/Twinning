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

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Null;

		// ----------------

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_null(thix, that);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			StringParser::read_null(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region boolean

	template <>
	struct CharacterStreamAdapter<Boolean> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Boolean;

		// ----------------

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_boolean(thix, that);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			StringParser::read_boolean(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region number

	template <>
	struct CharacterStreamAdapter<Integer> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Integer;

		// ----------------

		inline static auto write (
			ThisO &         thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			StringParser::read_number(thix, that);
			return;
		}

	};

	template <>
	struct CharacterStreamAdapter<Floater> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Floater;

		// ----------------

		inline static auto write (
			ThisO &         thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			StringParser::read_number(thix, that);
			return;
		}

	};

	// ----------------

	template <>
	struct CharacterStreamAdapter<NumberVariant> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = NumberVariant;

		// ----------------

		inline static auto write (
			ThisO &         thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			StringParser::read_number(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region byte

	template <>
	struct CharacterStreamAdapter<Byte> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Byte;

		// ----------------

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_byte(thix, that);
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			StringParser::read_byte(thix, that);
			return;
		}

	};

	#pragma endregion

	#pragma region byte list

	template <auto t_constant> requires
		AutoConstraint
	struct CharacterStreamAdapter<ByteListView<t_constant>> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = ByteListView<t_constant>;

		// ----------------

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_byte_list(thix, that);
			return;
		}

		inline static auto read (
			ThisI &      thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
			StringParser::read_byte_list(thix, that);
			return;
		}

	};

	#pragma endregion

}
