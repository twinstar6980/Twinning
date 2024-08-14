#pragma once

#include "kernel/utility/miscellaneous/byte_series/stream.hpp"
#include <bit>

namespace Twinning::Kernel {

	#pragma region basic

	// TODO : should be member variable ?
	inline thread_local auto g_byte_stream_use_big_endian = Boolean{k_false};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<BaseWrapper<TValue>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = BaseWrapper<TValue>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return k_type_size<That>;
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			return k_type_size<That>;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			if (g_byte_stream_use_big_endian != (std::endian::native == std::endian::little)) [[likely]]
			{
				std::memcpy(thix.current_pointer().value, &that, k_type_size<TValue>.value);
			}
			else [[unlikely]]
			{
				auto that_reversed = reverse_endian(that);
				std::memcpy(thix.current_pointer().value, &that_reversed, k_type_size<TValue>.value);
			}
			thix.forward(k_type_size<TValue>);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			std::memcpy(&that, thix.current_pointer().value, k_type_size<TValue>.value);
			if (g_byte_stream_use_big_endian == (std::endian::native == std::endian::little)) [[unlikely]]
			{
				that = reverse_endian(that);
			}
			thix.forward(k_type_size<TValue>);
			return;
		}

	};

	// ----------------

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<BooleanWrapper<TValue>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = BooleanWrapper<TValue>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return bs_static_size<BaseWrapper<TValue>>();
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			return bs_size<BaseWrapper<TValue>>(that);
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using RawValueAlternative = AsSwitch<IsSame<TValue, ZBoolean>, ZIntegerU8, TValue>;
			using RawWrapper = IntegerWrapper<std::make_unsigned_t<RawValueAlternative>>;
			thix.write(self_cast<RawWrapper>(that));
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using RawValueAlternative = AsSwitch<IsSame<TValue, ZBoolean>, ZIntegerU8, TValue>;
			using RawWrapper = IntegerWrapper<std::make_unsigned_t<RawValueAlternative>>;
			thix.read(self_cast<RawWrapper>(that));
			assert_test((self_cast<RawWrapper>(that) & ~mbw<RawWrapper>(0b1)) == mbw<RawWrapper>(0b0));
			return;
		}

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<NumberWrapper<TValue>> :
		ByteStreamAdapter<BaseWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<IntegerWrapper<TValue>> :
		ByteStreamAdapter<NumberWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<FloaterWrapper<TValue>> :
		ByteStreamAdapter<NumberWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<CharacterWrapper<TValue>> :
		ByteStreamAdapter<IntegerWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<ByteWrapper<TValue>> :
		ByteStreamAdapter<IntegerWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<EnumeratedWrapper<TValue>> :
		ByteStreamAdapter<IntegerWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<EnumerationWrapper<TValue>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = EnumerationWrapper<TValue>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return bs_static_size<typename That::Underlying>();
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			return bs_size<typename That::Underlying>(that.underlying());
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.as_underlying());
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.as_underlying());
			return;
		}

	};

	// ----------------

	template <>
	struct ByteStreamAdapter<Character> :
		ByteStreamAdapter<CharacterN> {
	};

	template <>
	struct ByteStreamAdapter<Unicode> :
		ByteStreamAdapter<Character32> {
	};

	template <>
	struct ByteStreamAdapter<Byte> :
		ByteStreamAdapter<Byte8> {
	};

	template <>
	struct ByteStreamAdapter<Enumerated> :
		ByteStreamAdapter<Enumerated8> {
	};

	#pragma endregion

	#pragma region container

	template <typename TElement, auto t_constant> requires
		AutoConstraint
	struct ByteStreamAdapter<ListView<TElement, t_constant>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = ListView<TElement, t_constant>;

		// ----------------

		static constexpr auto static_size (
			Size const & size
		) -> Size {
			return bs_static_size<TElement>() * size;
		}

		static auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			for (auto & element : that) {
				result += bs_size(element);
			}
			return result;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		static auto read (
			ThisI &      thix,
			That const & that
		) -> Void requires
			(!t_constant) {
			for (auto & element : that) {
				thix.read(element);
			}
			return;
		}

	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<List<TElement>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = List<TElement>;

		// ----------------

		static constexpr auto static_size (
			Size const & size
		) -> Size {
			return bs_static_size<TElement>() * size;
		}

		static auto size (
			That const & that
		) -> Size {
			return bs_size(that.as_view());
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.as_view());
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.as_view());
			return;
		}

		static auto read (
			ThisI &      thix,
			That &       that,
			Size const & size
		) -> Void {
			that.allocate_full(size);
			thix.read(that.as_view());
			return;
		}

	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<Array<TElement>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Array<TElement>;

		// ----------------

		static constexpr auto static_size (
			Size const & size
		) -> Size {
			return bs_static_size<TElement>() * size;
		}

		static auto size (
			That const & that
		) -> Size {
			return bs_size(that.as_view());
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.as_view());
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.as_view());
			return;
		}

		static auto read (
			ThisI &      thix,
			That &       that,
			Size const & size
		) -> Void {
			that.allocate(size);
			thix.read(that.as_view());
			return;
		}

	};

	template <typename TElement, auto t_size> requires
		AutoConstraint
	struct ByteStreamAdapter<StaticArray<TElement, t_size>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = StaticArray<TElement, t_size>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return bs_static_size<TElement>() * t_size;
		}

		static auto size (
			That const & that
		) -> Size {
			return bs_size(that.view());
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.view());
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.view());
			return;
		}

	};

	#pragma endregion

	#pragma region byte container

	template <typename TElement, auto t_constant> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicByteListView<TElement, t_constant>> :
		ByteStreamAdapter<ListView<TElement, t_constant>> {
	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicByteList<TElement>> :
		ByteStreamAdapter<List<TElement>> {
	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicByteArray<TElement>> :
		ByteStreamAdapter<Array<TElement>> {
	};

	template <typename TElement, auto t_size> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicStaticByteArray<TElement, t_size>> :
		ByteStreamAdapter<StaticArray<TElement, t_size>> {
	};

	#pragma endregion

	#pragma region character container

	template <typename TElement, auto t_constant> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicCharacterListView<TElement, t_constant>> :
		ByteStreamAdapter<ListView<TElement, t_constant>> {
	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicCharacterList<TElement>> :
		ByteStreamAdapter<List<TElement>> {
	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicCharacterArray<TElement>> :
		ByteStreamAdapter<Array<TElement>> {
	};

	template <typename TElement, auto t_size> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicStaticCharacterArray<TElement, t_size>> :
		ByteStreamAdapter<StaticArray<TElement, t_size>> {
	};

	#pragma endregion

	#pragma region string

	template <typename TElement, auto t_constant> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicStringView<TElement, t_constant>> :
		ByteStreamAdapter<ListView<TElement, t_constant>> {
	};

	template <typename TElement> requires
		AutoConstraint
	struct ByteStreamAdapter<BasicString<TElement>> :
		ByteStreamAdapter<List<TElement>> {
	};

	#pragma endregion

	#pragma region record

	template <typename TType> requires
		AutoConstraint
		&& (IsDerivedFrom<TType, Record>)
	struct ByteStreamAdapter<TType> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = TType;

		using FieldPackage = typename TType::Reflection::MemberVariable;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			auto result = k_none_size;
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					result += bs_static_size<AsPure<decltype(Field::value_of(declare<That const &>()))>>();
				}
			);
			return result;
		}

		static auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					result += bs_size(Field::value_of(that));
				}
			);
			return result;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix.write(Field::value_of(that));
				}
			);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					thix.read(Field::value_of(that));
				}
			);
			return;
		}

	};

	#pragma endregion

	#pragma region miscellaneous

	template <>
	struct ByteStreamAdapter<FourCC> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = FourCC;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			auto result = k_none_size;
			result += bs_static_size<decltype(That::first)>();
			result += bs_static_size<decltype(That::second)>();
			result += bs_static_size<decltype(That::third)>();
			result += bs_static_size<decltype(That::fourth)>();
			return result;
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			result += bs_size(that.first);
			result += bs_size(that.second);
			result += bs_size(that.third);
			result += bs_size(that.fourth);
			return result;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.first);
			thix.write(that.second);
			thix.write(that.third);
			thix.write(that.fourth);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.first);
			thix.read(that.second);
			thix.read(that.third);
			thix.read(that.fourth);
			return;
		}

	};

	// ----------------

	template <auto t_value> requires
		AutoConstraint
	struct ByteStreamAdapter<ConstantBlock<t_value>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = ConstantBlock<t_value>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return bs_static_size<AsUnmakeConstant<decltype(t_value)>>();
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			return bs_size(t_value);
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write_constant(t_value);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read_constant(t_value);
			return;
		}

	};

	// ----------------

	template <typename TLength> requires
		AutoConstraint
	struct ByteStreamAdapter<StringBlock<TLength>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = StringBlock<TLength>;

		// ----------------

		static auto size (
			That const & that
		) -> Size {
			if constexpr (IsVoid<TLength>) {
				return bs_size(that.value) + bs_static_size<Character>();
			}
			else {
				return bs_static_size<TLength>() + bs_size(that.value);
			}
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			if constexpr (IsVoid<TLength>) {
				StringParser::write_string_until(self_cast<OCharacterStreamView>(thix), that.value.as_view(), CharacterType::k_null);
				self_cast<OCharacterStreamView>(thix).write_constant(CharacterType::k_null);
			}
			else {
				thix.write(cbw<TLength>(that.value.size()));
				thix.write(that.value);
			}
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			if constexpr (IsVoid<TLength>) {
				auto that_view = CStringView{};
				StringParser::read_string_until(self_cast<ICharacterStreamView>(thix), that_view, CharacterType::k_null);
				self_cast<ICharacterStreamView>(thix).read_constant(CharacterType::k_null);
				that.value = that_view;
			}
			else {
				auto length = thix.read_of<TLength>();
				thix.read(that.value, cbw<Size>(length));
			}
			return;
		}

	};

	#pragma endregion

}
