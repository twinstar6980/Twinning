module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.byte_series.stream;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.miscellaneous.byte_series.container;
import twinning.kernel.utility.miscellaneous.byte_series.stream_adapter;

export namespace Twinning::Kernel {

	#pragma region type

	template <auto t_mode> requires
		CategoryConstraint<>
		&& (IsSameV<t_mode, StreamMode>)
	class ByteStreamView :
		public StreamView<Byte, t_mode, BasicByteListView> {

	private:

		using StreamView = StreamView<Byte, t_mode, BasicByteListView>;

		using IOStream = ByteStreamView<StreamMode::Constant::io()>;

		using IStream = ByteStreamView<StreamMode::Constant::i()>;

		using OStream = ByteStreamView<StreamMode::Constant::o()>;

	public:

		using typename StreamView::Element;

		using StreamView::mode;

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
		) -> ByteStreamView & = default;

		auto operator = (
			ByteStreamView && that
		) -> ByteStreamView & = default;

		// ----------------

		implicit operator IStream & () requires
			(mode == StreamMode::Constant::io()) {
			return thiz.as_input_stream();
		}

		implicit operator OStream & () requires
			(mode == StreamMode::Constant::io()) {
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
			(mode == StreamMode::Constant::o() || mode == StreamMode::Constant::io()) {
			ByteStreamAdapter<AsPure<That>>::write(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read (
			That &&       that,
			Option && ... option
		) -> Void requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
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
			(mode == StreamMode::Constant::o() || mode == StreamMode::Constant::io()) {
			thiz.write(that, as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read_constant (
			That const &  that,
			Option && ... option
		) -> Void requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
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
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
			auto that = That{};
			thiz.read(that, as_forward<Option>(option) ...);
			return that;
		}

		#pragma endregion

		#pragma region method cast

		auto as_input_stream (
		) -> IStream & requires
			(mode == StreamMode::Constant::io()) {
			return self_cast<IStream>(thiz);
		}

		auto as_output_stream (
		) -> OStream & requires
			(mode == StreamMode::Constant::io()) {
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

	using IOByteStreamView = ByteStreamView<StreamMode::Constant::io()>;

	using IByteStreamView = ByteStreamView<StreamMode::Constant::i()>;

	using OByteStreamView = ByteStreamView<StreamMode::Constant::o()>;

	#pragma endregion

}
