#pragma once

#include "kernel/utility/miscellaneous/character_series/stream.hpp"
#include "kernel/utility/string/string.hpp"
#include "kernel/utility/string/parser.hpp"

namespace TwinStar::Kernel {

	#pragma region character

	template <>
	struct CharacterStreamAdapter<Character> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Character;

		// ----------------

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			up_cast<OStreamView<Character, BasicCharacterListView>>(thix).write(that);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			up_cast<IStreamView<Character, BasicCharacterListView>>(thix).read(that);
			return;
		}

	};

	#pragma endregion

	#pragma region string

	template <auto t_constant> requires
		AutoConstraint
	struct CharacterStreamAdapter<StringView<t_constant>> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = StringView<t_constant>;

		// ----------------

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				up_cast<OStreamView<Character, BasicCharacterListView>>(thix).write(element);
			}
			return;
		}

		static auto read (
			ThisI &      thix,
			That const & that
		) -> Void requires
			(!t_constant) {
			for (auto & element : that) {
				up_cast<IStreamView<Character, BasicCharacterListView>>(thix).read(element);
			}
			return;
		}

	};

	#pragma endregion

	#pragma region null

	template <>
	struct CharacterStreamAdapter<Null> {

		using ThisI = ICharacterStreamView;

		using ThisO = OCharacterStreamView;

		using That = Null;

		// ----------------

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_null(thix, that);
			return;
		}

		static auto read (
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

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_boolean(thix, that);
			return;
		}

		static auto read (
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

		static auto write (
			ThisO &         thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		static auto read (
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

		static auto write (
			ThisO &         thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		static auto read (
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

		static auto write (
			ThisO &         thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			StringParser::write_number(thix, that, disable_sign_when_positive);
			return;
		}

		static auto read (
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

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_byte(thix, that);
			return;
		}

		static auto read (
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

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			StringParser::write_byte_list(thix, that);
			return;
		}

		static auto read (
			ThisI &      thix,
			That const & that
		) -> Void requires
			(!t_constant) {
			StringParser::read_byte_list(thix, that);
			return;
		}

	};

	#pragma endregion

}
