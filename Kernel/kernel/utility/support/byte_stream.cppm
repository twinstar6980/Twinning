module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.support.byte_stream;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.null;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.array.array;
import twinning.kernel.utility.container.static_array.static_array;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.byte_series.stream;
import twinning.kernel.utility.miscellaneous.byte_series.stream_adapter;
import twinning.kernel.utility.miscellaneous.byte_series.utility;
import twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.miscellaneous.character_series.stream;
import twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.miscellaneous.record;
import twinning.kernel.utility.miscellaneous.fourcc;
import twinning.kernel.utility.miscellaneous.constant_block;
import twinning.kernel.utility.miscellaneous.string_block;
import twinning.kernel.utility.string.basic_string;
import twinning.kernel.utility.string.basic_string_view;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.string.parser;

export namespace Twinning::Kernel {

	#pragma region basic

	// TODO : should be member variable ?
	inline thread_local auto g_byte_stream_use_big_endian = Boolean{k_false};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<BaseBox<TValue>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = BaseBox<TValue>;

		// ----------------

		inline static constexpr auto static_size (
		) -> Size {
			return k_type_size<That>;
		}

		inline static constexpr auto size (
			That const & that
		) -> Size {
			return k_type_size<That>;
		}

		inline static auto write (
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

		inline static auto read (
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
	struct ByteStreamAdapter<BooleanBox<TValue>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = BooleanBox<TValue>;

		// ----------------

		inline static constexpr auto static_size (
		) -> Size {
			return bs_static_size<BaseBox<TValue>>();
		}

		inline static constexpr auto size (
			That const & that
		) -> Size {
			return bs_size<BaseBox<TValue>>(that);
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			using RawValueAlternative = AsSwitch<IsSame<TValue, ZBoolean>, ZIntegerU8, TValue>;
			using RawBox = IntegerBox<std::make_unsigned_t<RawValueAlternative>>;
			thix.write(self_cast<RawBox>(that));
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			using RawValueAlternative = AsSwitch<IsSame<TValue, ZBoolean>, ZIntegerU8, TValue>;
			using RawBox = IntegerBox<std::make_unsigned_t<RawValueAlternative>>;
			thix.read(self_cast<RawBox>(that));
			assert_test((self_cast<RawBox>(that) & ~mbox<RawBox>(0b1)) == mbox<RawBox>(0b0));
			return;
		}

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<NumberBox<TValue>> :
		ByteStreamAdapter<BaseBox<TValue>> {

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<IntegerBox<TValue>> :
		ByteStreamAdapter<NumberBox<TValue>> {

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<FloaterBox<TValue>> :
		ByteStreamAdapter<NumberBox<TValue>> {

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<CharacterBox<TValue>> :
		ByteStreamAdapter<IntegerBox<TValue>> {

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<ByteBox<TValue>> :
		ByteStreamAdapter<IntegerBox<TValue>> {

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<EnumeratedBox<TValue>> :
		ByteStreamAdapter<IntegerBox<TValue>> {

	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<EnumerationBox<TValue>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = EnumerationBox<TValue>;

		// ----------------

		inline static constexpr auto static_size (
		) -> Size {
			return bs_static_size<typename That::Underlying>();
		}

		inline static constexpr auto size (
			That const & that
		) -> Size {
			return bs_size<typename That::Underlying>(that.underlying());
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.as_underlying());
			return;
		}

		inline static auto read (
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

		inline static constexpr auto static_size (
			Size const & size
		) -> Size {
			return bs_static_size<TElement>() * size;
		}

		inline static auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			for (auto & element : that) {
				result += bs_size(element);
			}
			return result;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			for (auto & element : that) {
				thix.write(element);
			}
			return;
		}

		inline static auto read (
			ThisI &      thix,
			That const & that
		) -> Void requires
			(!t_constant.value) {
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

		inline static constexpr auto static_size (
			Size const & size
		) -> Size {
			return bs_static_size<TElement>() * size;
		}

		inline static auto size (
			That const & that
		) -> Size {
			return bs_size(that.as_view());
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.as_view());
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.as_view());
			return;
		}

		inline static auto read (
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

		inline static constexpr auto static_size (
			Size const & size
		) -> Size {
			return bs_static_size<TElement>() * size;
		}

		inline static auto size (
			That const & that
		) -> Size {
			return bs_size(that.as_view());
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.as_view());
			return;
		}

		inline static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.as_view());
			return;
		}

		inline static auto read (
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

		inline static constexpr auto static_size (
		) -> Size {
			return bs_static_size<TElement>() * t_size;
		}

		inline static auto size (
			That const & that
		) -> Size {
			return bs_size(that.view());
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.view());
			return;
		}

		inline static auto read (
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

		inline static constexpr auto static_size (
		) -> Size {
			auto result = k_none_size;
			Generalization::each<FieldPackage>(
				[&] <auto index, typename Field> (ValuePackage<index>, TypePackage<Field>) {
					result += bs_static_size<AsPure<decltype(Field::value_of(declare<That const &>()))>>();
				}
			);
			return result;
		}

		inline static auto size (
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

		inline static auto write (
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

		inline static auto read (
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

		inline static constexpr auto static_size (
		) -> Size {
			auto result = k_none_size;
			result += bs_static_size<decltype(That::first)>();
			result += bs_static_size<decltype(That::second)>();
			result += bs_static_size<decltype(That::third)>();
			result += bs_static_size<decltype(That::fourth)>();
			return result;
		}

		inline static constexpr auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			result += bs_size(that.first);
			result += bs_size(that.second);
			result += bs_size(that.third);
			result += bs_size(that.fourth);
			return result;
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.first);
			thix.write(that.second);
			thix.write(that.third);
			thix.write(that.fourth);
			return;
		}

		inline static auto read (
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

		inline static constexpr auto static_size (
		) -> Size {
			return bs_static_size<AsUnmakeConstant<decltype(t_value)>>();
		}

		inline static constexpr auto size (
			That const & that
		) -> Size {
			return bs_size(t_value);
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write_constant(t_value);
			return;
		}

		inline static auto read (
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

		inline static auto size (
			That const & that
		) -> Size {
			if constexpr (IsVoid<TLength>) {
				return bs_size(that.value) + bs_static_size<Character>();
			}
			else {
				return bs_static_size<TLength>() + bs_size(that.value);
			}
		}

		inline static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			if constexpr (IsVoid<TLength>) {
				StringParser::write_string_until(self_cast<OCharacterStreamView>(thix), that.value.as_view(), CharacterType::k_null);
				self_cast<OCharacterStreamView>(thix).write_constant(CharacterType::k_null);
			}
			else {
				thix.write(cbox<TLength>(that.value.size()));
				thix.write(that.value);
			}
			return;
		}

		inline static auto read (
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
				thix.read(that.value, cbox<Size>(length));
			}
			return;
		}

	};

	#pragma endregion

}
