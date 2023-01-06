#pragma once

#include "core/utility/miscellaneous/byte_series/stream.hpp"

namespace TwinStar::Core {

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
			if (!g_byte_stream_use_big_endian) [[likely]]
			{
				std::memcpy(thix.current_pointer().value, &that, k_type_size<TValue>.value);
			} else [[unlikely]]
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
			thix.forward(k_type_size<TValue>);
			if (g_byte_stream_use_big_endian) {
				that = reverse_endian(that);
			}
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
			thix.write(up_cast<BaseWrapper<TValue>>(that));
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(up_cast<BaseWrapper<TValue>>(that));
			// todo
			assert_condition((self_cast<IntegerWrapper<ZIntegerU8>>(that) & ~mbw<IntegerWrapper<ZIntegerU8>>(0b1)) == mbw<IntegerWrapper<ZIntegerU8>>(0));
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
	struct ByteStreamAdapter<FloatingWrapper<TValue>> :
		ByteStreamAdapter<NumberWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<ByteWrapper<TValue>> :
		ByteStreamAdapter<IntegerWrapper<TValue>> {
	};

	template <typename TValue> requires
		AutoConstraint
	struct ByteStreamAdapter<CharacterWrapper<TValue>> :
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
	struct ByteStreamAdapter<Byte> :
		ByteStreamAdapter<Byte8> {
	};

	template <>
	struct ByteStreamAdapter<Character> :
		ByteStreamAdapter<CharacterLocale> {
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
	struct ByteStreamAdapter<FourCC> :
		ByteStreamAdapter<IntegerU32> {
	};

	// ----------------

	template <auto t_size> requires
		AutoConstraint
	struct ByteStreamAdapter<PaddingBlock<t_size>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = PaddingBlock<t_size>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return t_size;
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			return t_size;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			for (auto & index : SizeRange{t_size}) {
				thix.write(k_null_byte);
			}
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			for (auto & index : SizeRange{t_size}) {
				assert_condition(thix.read_of() == k_null_byte);
			}
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
			} else {
				return bs_static_size<TLength>() + bs_size(that.value);
			}
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			if constexpr (IsVoid<TLength>) {
				StringParser::write_string(self_cast<OCharacterStreamView>(thix), that.value);
				self_cast<OCharacterStreamView>(thix).write(CharacterType::k_null);
			} else {
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
				StringParser::read_string(self_cast<ICharacterStreamView>(thix), that_view);
				that.value = that_view;
				self_cast<ICharacterStreamView>(thix).forward(1_sz);
			} else {
				auto length = thix.read_of<TLength>();
				thix.read(that.value, cbw<Size>(length));
			}
			return;
		}

	};

	// ----------------

	template <>
	struct ByteStreamAdapter<Image::PixelRGB> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Image::PixelRGB;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			auto result = k_none_size;
			result += bs_static_size<AsPure<decltype(That::red)>>();
			result += bs_static_size<AsPure<decltype(That::green)>>();
			result += bs_static_size<AsPure<decltype(That::blue)>>();
			return result;
		}

		static auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			result += bs_size(that.red);
			result += bs_size(that.green);
			result += bs_size(that.blue);
			return result;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.red);
			thix.write(that.green);
			thix.write(that.blue);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.red);
			thix.read(that.green);
			thix.read(that.blue);
			return;
		}

	};

	template <>
	struct ByteStreamAdapter<Image::PixelRGBA> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = Image::PixelRGBA;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			auto result = k_none_size;
			result += bs_static_size<AsPure<decltype(That::red)>>();
			result += bs_static_size<AsPure<decltype(That::green)>>();
			result += bs_static_size<AsPure<decltype(That::blue)>>();
			result += bs_static_size<AsPure<decltype(That::alpha)>>();
			return result;
		}

		static auto size (
			That const & that
		) -> Size {
			auto result = k_none_size;
			result += bs_size(that.red);
			result += bs_size(that.green);
			result += bs_size(that.blue);
			result += bs_size(that.alpha);
			return result;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(that.red);
			thix.write(that.green);
			thix.write(that.blue);
			thix.write(that.alpha);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(that.red);
			thix.read(that.green);
			thix.read(that.blue);
			thix.read(that.alpha);
			return;
		}

	};

	#pragma endregion

}
