#pragma once

#include "core/utility/container/stream/stream_view.hpp"
#include "core/utility/misc/byte_series/container.hpp"
#include "core/utility/misc/byte_series/utility.hpp"
#include "core/utility/misc/byte_series/stream_adapter.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <auto t_method> requires
		CategoryConstraint<>
		&& (IsSameV<t_method, StreamMethod>)
	class ByteStreamView :
		public StreamView<Byte, t_method, BasicByteListView> {

	private: //

		using StreamView = StreamView<Byte, t_method, BasicByteListView>;

		using IOStream = ByteStreamView<StreamMethod::io>;

		using IStream = ByteStreamView<StreamMethod::i>;

		using OStream = ByteStreamView<StreamMethod::o>;

	public: //

		using typename StreamView::Element;

		using StreamView::method;

		using typename StreamView::ListView;

		using typename StreamView::QElement;

		using typename StreamView::QIterator;

	public: //

		#pragma region structor

		~ByteStreamView (
		) = default;

		// ----------------

		ByteStreamView (
		) = default;

		ByteStreamView (
			ByteStreamView const & that
		) = default;

		ByteStreamView (
			ByteStreamView && that
		) = default;

		// ----------------

		using StreamView::StreamView;

		#pragma endregion

		#pragma region operator

		auto operator = (
			ByteStreamView const & that
		) -> ByteStreamView& = default;

		auto operator = (
			ByteStreamView && that
		) -> ByteStreamView& = default;

		// ----------------

		implicit operator IStream & () requires
			(method == StreamMethod::io) {
			return thiz.as_input_stream();
		}

		implicit operator OStream & () requires
			(method == StreamMethod::io) {
			return thiz.as_output_stream();
		}

		#pragma endregion

		#pragma region write & read by adapter

		template <typename That, typename ...Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option...>>
		auto write (
			That &&      that,
			Option && ...option
		) -> Void requires
			(method == StreamMethod::o || method == StreamMethod::io) {
			ByteStreamAdapter<AsPure<That>>::write(thiz, as_forward<That>(that), as_forward<Option>(option)...);
			return;
		}

		template <typename That, typename ...Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option...>>
		auto read (
			That &&      that,
			Option && ...option
		) -> Void requires
			(method == StreamMethod::i || method == StreamMethod::io) {
			ByteStreamAdapter<AsPure<That>>::read(thiz, as_forward<That>(that), as_forward<Option>(option)...);
			return;
		}

		// ----------------

		template <typename That = Byte, typename ...Option> requires
			CategoryConstraint<IsPureInstance<That> && IsValid<Option...>>
		auto read_of (
			Option && ...option
		) -> That requires
			(method == StreamMethod::i || method == StreamMethod::io) {
			auto that = That{};
			thiz.read(that, as_forward<Option>(option)...);
			return that;
		}

		#pragma endregion

		#pragma region pad

		auto pad (
			Size const & size
		) -> Void requires
			(method == StreamMethod::o || method == StreamMethod::io) {
			Range::assign(thiz.next_view(size), k_null_byte);
			thiz.forward(size);
			return;
		}

		auto pad_auto (
			Size const & unit
		) -> Void requires
			(method == StreamMethod::o || method == StreamMethod::io) {
			return thiz.pad(compute_padding_size(thiz.position(), unit));
		}

		#pragma endregion

		#pragma region method cast

		auto as_input_stream (
		) -> IStream& requires
			(method == StreamMethod::io) {
			return self_cast<IStream>(thiz);
		}

		auto as_output_stream (
		) -> OStream& requires
			(method == StreamMethod::io) {
			return self_cast<OStream>(thiz);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	using IOByteStreamView = ByteStreamView<StreamMethod::io>;

	using IByteStreamView = ByteStreamView<StreamMethod::i>;

	using OByteStreamView = ByteStreamView<StreamMethod::o>;

	#pragma endregion

	#pragma region utility

	template <typename That, typename ...Option> requires
		CategoryConstraint<IsPureInstance<That> && IsValid<Option...>>
	inline constexpr auto bs_static_size (
		Option && ...option
	) -> Size {
		return ByteStreamAdapter<That>::static_size(as_forward<Option>(option)...);
	}

	template <typename That, typename ...Option> requires
		CategoryConstraint<IsPureInstance<That> && IsValid<Option...>>
	inline constexpr auto bs_size (
		That const & that,
		Option && ...option
	) -> Size {
		return ByteStreamAdapter<That>::size(that, as_forward<Option>(option)...);
	}

	#pragma endregion

}
