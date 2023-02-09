#pragma once

#include "core/utility/container/stream/stream_view.hpp"
#include "core/utility/miscellaneous/byte_series/container.hpp"
#include "core/utility/miscellaneous/byte_series/utility.hpp"
#include "core/utility/miscellaneous/byte_series/stream_adapter.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <auto t_method> requires
		CategoryConstraint<>
		&& (IsSameV<t_method, StreamMethod>)
	class ByteStreamView :
		public StreamView<Byte, t_method, BasicByteListView> {

	private:

		using StreamView = StreamView<Byte, t_method, BasicByteListView>;

		using IOStream = ByteStreamView<StreamMethod::Constant::io()>;

		using IStream = ByteStreamView<StreamMethod::Constant::i()>;

		using OStream = ByteStreamView<StreamMethod::Constant::o()>;

	public:

		using typename StreamView::Element;

		using StreamView::method;

		using typename StreamView::ListView;

		using typename StreamView::QElement;

		using typename StreamView::QIterator;

	public:

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
			(method == StreamMethod::Constant::io()) {
			return thiz.as_input_stream();
		}

		implicit operator OStream & () requires
			(method == StreamMethod::Constant::io()) {
			return thiz.as_output_stream();
		}

		#pragma endregion

		#pragma region write & read by adapter

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto write (
			That &&       that,
			Option && ... option
		) -> Void requires
			(method == StreamMethod::Constant::o() || method == StreamMethod::Constant::io()) {
			ByteStreamAdapter<AsPure<That>>::write(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read (
			That &&       that,
			Option && ... option
		) -> Void requires
			(method == StreamMethod::Constant::i() || method == StreamMethod::Constant::io()) {
			ByteStreamAdapter<AsPure<That>>::read(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		// ----------------

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto write_constant (
			That const &  that,
			Option && ... option
		) -> Void requires
			(method == StreamMethod::Constant::o() || method == StreamMethod::Constant::io()) {
			thiz.write(that, as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read_constant (
			That const &  that,
			Option && ... option
		) -> Void requires
			(method == StreamMethod::Constant::i() || method == StreamMethod::Constant::io()) {
			auto temporary_that = That{};
			thiz.read(temporary_that, as_forward<Option>(option) ...);
			assert_test(temporary_that == that);
			return;
		}

		// ----------------

		template <typename That = Byte, typename ... Option> requires
			CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
		auto read_of (
			Option && ... option
		) -> That requires
			(method == StreamMethod::Constant::i() || method == StreamMethod::Constant::io()) {
			auto that = That{};
			thiz.read(that, as_forward<Option>(option) ...);
			return that;
		}

		#pragma endregion

		#pragma region method cast

		auto as_input_stream (
		) -> IStream& requires
			(method == StreamMethod::Constant::io()) {
			return self_cast<IStream>(thiz);
		}

		auto as_output_stream (
		) -> OStream& requires
			(method == StreamMethod::Constant::io()) {
			return self_cast<OStream>(thiz);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename That, typename ... Option> requires
		CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
	inline constexpr auto bs_static_size (
		Option && ... option
	) -> Size {
		return ByteStreamAdapter<That>::static_size(as_forward<Option>(option) ...);
	}

	template <typename That, typename ... Option> requires
		CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
	inline constexpr auto bs_size (
		That const &  that,
		Option && ... option
	) -> Size {
		return ByteStreamAdapter<That>::size(that, as_forward<Option>(option) ...);
	}

	#pragma endregion

	#pragma region alias

	using IOByteStreamView = ByteStreamView<StreamMethod::Constant::io()>;

	using IByteStreamView = ByteStreamView<StreamMethod::Constant::i()>;

	using OByteStreamView = ByteStreamView<StreamMethod::Constant::o()>;

	#pragma endregion

}
