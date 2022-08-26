#pragma once

#include "core/utility/misc/byte_series/stream.hpp"
#include "core/utility/misc/byte_series/stream_adapter_utility.hpp"

namespace TwinKleS::Core {

	#pragma region basic

	// TODO : should be member variable ?
	inline thread_local auto g_byte_stream_use_big_endian = Boolean{k_false};

	template <typename TType> requires
		CategoryConstraint<IsPureInstance<TType>>
		&& (IsBaseWrapper<TType>)
	#if defined M_compiler_clang // NOTE : avoid clang bug
		&& (IsDerivedFrom<TType, BaseWrapper<typename TType::Value>>)
	#endif
	struct ByteStreamAdapter<TType> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = TType;

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
				std::memcpy(thix.current_pointer().value, &that, k_type_size<TType>.value);
			} else [[unlikely]]
			{
				auto that_reversed = reverse_endian(that);
				std::memcpy(thix.current_pointer().value, &that_reversed, k_type_size<TType>.value);
			}
			thix.forward(k_type_size<TType>);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			std::memcpy(&that, thix.current_pointer().value, k_type_size<TType>.value);
			thix.forward(k_type_size<TType>);
			if (g_byte_stream_use_big_endian) {
				that = reverse_endian(that);
			}
			return;
		}

	};

	// ----------------

	template <typename TType> requires
		AutoConstraint
	struct ByteStreamAdapter<AsEnum<TType>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = AsEnum<TType>;

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
			thix.write(self_cast<BaseWrapper<AsEnumUnderlying<That>>>(that));
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			thix.read(self_cast<BaseWrapper<AsEnumUnderlying<That>>>(that));
			return;
		}

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
			return bs_static_size<TElement>() * mbw<Size>(t_size);
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

	#pragma region misc

	template <auto t_size> requires
		AutoConstraint
	struct ByteStreamAdapter<PaddingBlock<t_size>> {

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = PaddingBlock<t_size>;

		// ----------------

		static constexpr auto static_size (
		) -> Size {
			return mbw<Size>(t_size);
		}

		static constexpr auto size (
			That const & that
		) -> Size {
			return mbw<Size>(t_size);
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			for (auto & index : SizeRange{mbw<Size>(t_size)}) {
				thix.write(k_null_byte);
			}
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			for (auto & index : SizeRange{mbw<Size>(t_size)}) {
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
			return bs_static_size<TLength>() + bs_size(that.value);
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void {
			thix.write(cbw<TLength>(that.value.size()));
			thix.write(that.value);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void {
			auto length = thix.read_of<TLength>();
			thix.read(that.value, cbw<Size>(length));
			return;
		}

	};

	// ----------------

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Image::PixelRGB),
		M_wrap(red, green, blue),
	);

	M_byte_stream_adapter_for_aggregate_by_field_of_unique(
		M_wrap(Image::PixelRGBA),
		M_wrap(red, green, blue, alpha),
	);

	#pragma endregion

}
