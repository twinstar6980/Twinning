#pragma once

#include "core/utility/string/format.hpp"
#include "core/utility/null.hpp"
#include "core/utility/miscellaneous/fourcc.hpp"

namespace TwinStar::Core {

	#pragma region string

	template <>
	struct BasicStringAdapter<Character, String> {

		using This = BasicString<Character>;

		using That = String;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix = that;
			return;
		}

		static auto from (
			This &  thix,
			That && that
		) -> Void {
			thix = as_moveable(that);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that = thix;
			return;
		}

	};

	#pragma endregion

	#pragma region character

	template <>
	struct BasicStringAdapter<Character, Character> {

		using This = BasicString<Character>;

		using That = Character;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.allocate_full(1_sz);
			thix.first() = that;
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			assert_test(thix.size() == 1_sz);
			that = thix.first();
			return;
		}

	};

	#pragma endregion

	#pragma region null

	template <>
	struct BasicStringAdapter<Character, Null> {

		using This = BasicString<Character>;

		using That = Null;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix = "null"_sv;
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			assert_test(thix == "null"_sv);
			return;
		}

	};

	#pragma endregion

	#pragma region boolean

	template <>
	struct BasicStringAdapter<Character, Boolean> {

		using This = BasicString<Character>;

		using That = Boolean;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix = that ? "true"_sv : "false"_sv;
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			if (thix == "true"_sv) {
				that = k_true;
			} else if (thix == "false"_sv) {
				that = k_false;
			} else {
				assert_fail(R"(thix == "true"_sv || thix == "false"_sv)");
			}
			return;
		}

	};

	#pragma endregion

	#pragma region number

	template <>
	struct BasicStringAdapter<Character, Integer> {

		using This = BasicString<Character>;

		using That = Integer;

		// ----------------

		static auto from (
			This &          thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			thix.allocate_full(20_sz);
			auto stream = OCharacterStreamView{thix};
			stream.write(that, disable_sign_when_positive);
			thix.set_size(stream.position());
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto stream = ICharacterStreamView{thix};
			stream.read(that);
			assert_test(stream.full());
			return;
		}

	};

	template <>
	struct BasicStringAdapter<Character, Floating> {

		using This = BasicString<Character>;

		using That = Floating;

		// ----------------

		static auto from (
			This &          thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			thix.allocate_full(32_sz);
			auto stream = OCharacterStreamView{thix};
			stream.write(that, disable_sign_when_positive);
			thix.set_size(stream.position());
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto stream = ICharacterStreamView{thix};
			stream.read(that);
			assert_test(stream.full());
			return;
		}

	};

	// ----------------

	template <>
	struct BasicStringAdapter<Character, NumberVariant> {

		using This = BasicString<Character>;

		using That = NumberVariant;

		// ----------------

		static auto from (
			This &          thix,
			That const &    that,
			Boolean const & disable_sign_when_positive = k_false
		) -> Void {
			thix.allocate_full(32_sz);
			auto stream = OCharacterStreamView{thix};
			stream.write(that, disable_sign_when_positive);
			thix.set_size(stream.position());
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto stream = ICharacterStreamView{thix};
			stream.read(that);
			assert_test(stream.full());
			return;
		}

	};

	#pragma endregion

	#pragma region enumeration

	template <typename TType> requires
		AutoConstraint
		&& (IsEnumerationWrapper<TType>)
		&& (IsDerivedFrom<TType, Enumeration<typename TType::Value>>)
		&& (!IsSame<TType, Enumeration<typename TType::Value>>)
	struct BasicStringAdapter<Character, TType> {

		using This = BasicString<Character>;

		using That = TType;

		using FieldPackage = typename TType::Reflection::EnumerationValue;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			auto has_case = k_false;
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					if (!has_case) {
						if (Field::value == that.value) {
							thix = make_string(Field::name.view());
							has_case = k_true;
						}
					}
				}
			);
			if (!has_case) {
				assert_fail(R"(/* enumeration value is invalid */)");
			}
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto has_case = k_false;
			auto thix_hash = thix.hash().value;
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					if (!has_case) {
						constexpr auto name_hash = hash_std_string_view(Field::name.view());
						if (name_hash == thix_hash) {
							that.value = Field::value;
							has_case = k_true;
						}
					}
				}
			);
			if (!has_case) {
				assert_fail(R"(/* enumeration name is invalid */)");
			}
			return;
		}

	};

	#pragma endregion

	#pragma region byte

	template <>
	struct BasicStringAdapter<Character, Byte> {

		using This = BasicString<Character>;

		using That = Byte;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.allocate_full(2_sz);
			auto stream = OCharacterStreamView{thix};
			stream.write(that);
			thix.set_size(stream.position());
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			auto stream = ICharacterStreamView{thix};
			stream.read(that);
			assert_test(stream.full());
			return;
		}

	};

	#pragma endregion

	#pragma region byte list

	template <>
	struct BasicStringAdapter<Character, ByteList> {

		using This = BasicString<Character>;

		using That = ByteList;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.allocate_full(that.size() * 2_sz + maximum(that.size(), 1_sz) - 1_sz);
			auto stream = OCharacterStreamView{thix};
			stream.write(that.view());
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			assert_test(thix.size() == 0_sz || thix.size() % 3_sz == 2_sz);
			that.allocate_full((thix.size() + 1_sz) / 3_sz);
			auto stream = ICharacterStreamView{thix};
			stream.read(that.view());
			return;
		}

	};

	#pragma endregion

	#pragma region miscellaneous

	template <>
	struct BasicStringAdapter<Character, FourCC> {

		using This = BasicString<Character>;

		using That = FourCC;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix.allocate_full(4_sz);
			thix[1_ix] = cbw<Character>(that.first);
			thix[2_ix] = cbw<Character>(that.second);
			thix[3_ix] = cbw<Character>(that.third);
			thix[4_ix] = cbw<Character>(that.fourth);
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			assert_test(thix.size() == 4_sz);
			that.first = cbw<Character8>(thix[1_ix]);
			that.second = cbw<Character8>(thix[2_ix]);
			that.third = cbw<Character8>(thix[3_ix]);
			that.fourth = cbw<Character8>(thix[4_ix]);
			return;
		}

	};

	// ----------------

	template <>
	struct BasicStringAdapter<Character, Path> {

		using This = BasicString<Character>;

		using That = Path;

		// ----------------

		static auto from (
			This &       thix,
			That const & that
		) -> Void {
			thix = that.to_string();
			return;
		}

		static auto to (
			This const & thix,
			That &       that
		) -> Void {
			that.from_string(thix);
			return;
		}

	};

	#pragma endregion

}
