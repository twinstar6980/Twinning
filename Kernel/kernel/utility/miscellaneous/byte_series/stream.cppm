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
		&& (IsSameOf<t_mode, StreamMode>)
	class ByteStreamView :
		public StreamView<Byte, t_mode, BasicByteListView> {

	private:

		using StreamView = StreamView<Byte, t_mode, BasicByteListView>;

		using AccessStream = ByteStreamView<StreamMode::Constant::access()>;

		using InputStream = ByteStreamView<StreamMode::Constant::input()>;

		using OutputStream = ByteStreamView<StreamMode::Constant::output()>;

	public:

		using typename StreamView::Element;

		using StreamView::mode;

		using typename StreamView::ListView;

		using typename StreamView::QualifyElement;

		using typename StreamView::QualifyIterator;

	public:

		#pragma region constructor

		~ByteStreamView(
		) = default;

		// ----------------

		ByteStreamView(
		) = default;

		ByteStreamView(
			ByteStreamView const & that
		) = default;

		ByteStreamView(
			ByteStreamView && that
		) = default;

		// ----------------

		using StreamView::StreamView;

		#pragma endregion

		#pragma region operator

		auto operator =(
			ByteStreamView const & that
		) -> ByteStreamView & = default;

		auto operator =(
			ByteStreamView && that
		) -> ByteStreamView & = default;

		// ----------------

		implicit operator InputStream &() requires
			(mode == StreamMode::Constant::access()) {
			return thiz.as_input_stream();
		}

		implicit operator OutputStream &() requires
			(mode == StreamMode::Constant::access()) {
			return thiz.as_output_stream();
		}

		#pragma endregion

		#pragma region write & read by adapter

		template <typename TThat, typename ... TOption> requires
			CategoryConstraint<IsValid<TThat> && IsValid<TOption ...>>
		auto write(
			TThat &&       that,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			ByteStreamAdapter<AsPure<TThat>>::write(thiz, as_forward<TThat>(that), as_forward<TOption>(option) ...);
			return;
		}

		template <typename TThat, typename ... TOption> requires
			CategoryConstraint<IsValid<TThat> && IsValid<TOption ...>>
		auto read(
			TThat &&       that,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			ByteStreamAdapter<AsPure<TThat>>::read(thiz, as_forward<TThat>(that), as_forward<TOption>(option) ...);
			return;
		}

		// ----------------

		template <typename TThat, typename ... TOption> requires
			CategoryConstraint<IsValid<TThat> && IsValid<TOption ...>>
		auto write_constant(
			TThat const &  that,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			thiz.write(that, as_forward<TOption>(option) ...);
			return;
		}

		template <typename TThat, typename ... TOption> requires
			CategoryConstraint<IsValid<TThat> && IsValid<TOption ...>>
		auto read_constant(
			TThat const &  that,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto temporary_that = TThat{};
			thiz.read(temporary_that, as_forward<TOption>(option) ...);
			assert_test(temporary_that == that);
			return;
		}

		// ----------------

		template <typename TThat = Byte, typename ... TOption> requires
			CategoryConstraint<IsPureInstance<TThat> && IsValid<TOption ...>>
		auto read_of(
			TOption && ... option
		) -> TThat requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto that = TThat{};
			thiz.read(that, as_forward<TOption>(option) ...);
			return that;
		}

		#pragma endregion

		#pragma region mode cast

		auto as_input_stream(
		) -> InputStream & requires
			(mode == StreamMode::Constant::access()) {
			return self_cast<InputStream>(thiz);
		}

		auto as_output_stream(
		) -> OutputStream & requires
			(mode == StreamMode::Constant::access()) {
			return self_cast<OutputStream>(thiz);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region utility

	template <typename TThat, typename ... TOption> requires
		CategoryConstraint<IsPureInstance<TThat> && IsValid<TOption ...>>
	inline constexpr auto bs_static_size(
		TOption && ... option
	) -> Size {
		return ByteStreamAdapter<TThat>::static_size(as_forward<TOption>(option) ...);
	}

	template <typename TThat, typename ... TOption> requires
		CategoryConstraint<IsPureInstance<TThat> && IsValid<TOption ...>>
	inline constexpr auto bs_size(
		TThat const &  that,
		TOption && ... option
	) -> Size {
		return ByteStreamAdapter<TThat>::size(that, as_forward<TOption>(option) ...);
	}

	#pragma endregion

	#pragma region alias

	using AccessByteStreamView = ByteStreamView<StreamMode::Constant::access()>;

	using InputByteStreamView = ByteStreamView<StreamMode::Constant::input()>;

	using OutputByteStreamView = ByteStreamView<StreamMode::Constant::output()>;

	#pragma endregion

}
