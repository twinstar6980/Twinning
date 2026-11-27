module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.binary.basic_binary_stream_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.binary.basic_binary_view;
import twinning.kernel.utility.binary.basic_binary_stream_flag;
import twinning.kernel.utility.binary.basic_binary_stream_adapter;
import twinning.kernel.utility.range.integer_range;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_mode> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsByteBox<TElement>)
		&& (IsSameOf<t_mode, StreamMode>)
	class BasicBinaryStreamView :
		public StreamView<TElement, t_mode, BasicBinaryView, BasicBinaryStreamFlag> {

	private:

		using StreamView = StreamView<TElement, t_mode, BasicBinaryView, BasicBinaryStreamFlag>;

		using AccessStream = BasicBinaryStreamView<TElement, StreamMode::Constant::access()>;

		using InputStream = BasicBinaryStreamView<TElement, StreamMode::Constant::input()>;

		using OutputStream = BasicBinaryStreamView<TElement, StreamMode::Constant::output()>;

	public:

		using typename StreamView::Element;

		using StreamView::mode;

		using typename StreamView::ArrayView;

		using typename StreamView::Flag;

		using typename StreamView::QualifyElement;

		using typename StreamView::QualifyIterator;

	public:

		#pragma region constructor

		constexpr ~BasicBinaryStreamView(
		) = default;

		// ----------------

		constexpr BasicBinaryStreamView(
		) :
			StreamView{} {
			return;
		}

		constexpr BasicBinaryStreamView(
			BasicBinaryStreamView const & that
		) = default;

		constexpr BasicBinaryStreamView(
			BasicBinaryStreamView && that
		) = default;

		// ----------------

		explicit constexpr BasicBinaryStreamView(
			ArrayView const & view,
			Size const &      position = 0_sz,
			Flag const &      flag = Flag{}
		) :
			StreamView{view, position, flag} {
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BasicBinaryStreamView const & that
		) -> BasicBinaryStreamView & = default;

		constexpr auto operator =(
			BasicBinaryStreamView && that
		) -> BasicBinaryStreamView & = default;

		// ----------------

		implicit operator InputStream &() requires
			(mode == StreamMode::Constant::access()) {
			return unsafe_cast<InputStream>(thiz);
		}

		implicit operator OutputStream &() requires
			(mode == StreamMode::Constant::access()) {
			return unsafe_cast<OutputStream>(thiz);
		}

		#pragma endregion

		#pragma region split

		constexpr auto split_copy(
		) -> BasicBinaryStreamView {
			return BasicBinaryStreamView{thiz.m_view, thiz.position(), thiz.flag()};
		}

		constexpr auto split_sub(
			Size const & begin,
			Size const & count
		) -> BasicBinaryStreamView {
			assert_test(begin + count <= thiz.size());
			return BasicBinaryStreamView{thiz.m_view.sub(begin, count), 0_sz, thiz.flag()};
		}

		constexpr auto split_all(
		) -> BasicBinaryStreamView {
			return thiz.split_sub(0_sz, thiz.size());
		}

		// ----------------

		constexpr auto split_next(
			Size const & count
		) -> BasicBinaryStreamView {
			assert_test(count <= thiz.reserve());
			return thiz.split_sub(thiz.position(), count);
		}

		// ----------------

		constexpr auto split_stream(
		) -> BasicBinaryStreamView {
			return thiz.split_sub(0_sz, thiz.position());
		}

		constexpr auto split_reserve(
		) -> BasicBinaryStreamView {
			return thiz.split_sub(thiz.position(), thiz.size() - thiz.position());
		}

		// ----------------

		constexpr auto split_forward(
			Size const & count
		) -> BasicBinaryStreamView {
			auto view = thiz.split_next(count);
			thiz.forward(count);
			return view;
		}

		#pragma endregion

		#pragma region exchange

		template <typename TValue, typename ... TOption> requires
			CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto write(
			TValue &&      value,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			BasicBinaryStreamAdapter<Element, AsPure<TValue>>::write(thiz, as_forward<TValue>(value), as_forward<TOption>(option) ...);
			return;
		}

		template <typename TValue, typename ... TOption> requires
			CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto read(
			TValue &&      value,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			BasicBinaryStreamAdapter<Element, AsPure<TValue>>::read(thiz, as_forward<TValue>(value), as_forward<TOption>(option) ...);
			return;
		}

		// ----------------

		template <typename TValue = Element, typename ... TOption> requires
			CategoryConstraint<IsPureInstance<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto read_of(
			TOption && ... option
		) -> TValue requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto value = TValue{};
			thiz.read(value, as_forward<TOption>(option) ...);
			return value;
		}

		// ----------------

		template <typename TValue, typename ... TOption> requires
			CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto write_constant(
			TValue const & value,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			thiz.write(value, as_forward<TOption>(option) ...);
			return;
		}

		template <typename TValue, typename ... TOption> requires
			CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto read_constant(
			TValue const & value,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto temporary_value = TValue{};
			thiz.read(temporary_value, as_forward<TOption>(option) ...);
			assert_test(temporary_value == value);
			return;
		}

		// ----------------

		template <typename TValue, typename ... TOption> requires
			CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto write_space(
			Size const &   size,
			TValue const & value,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			for (auto & index : SizeRange{size}) {
				thiz.write_constant(value, as_forward<TOption>(option) ...);
			}
			return;
		}

		template <typename TValue, typename ... TOption> requires
			CategoryConstraint<IsValid<TValue> && IsValid<TOption ...>>
			&& (IsHasBasicBinaryStreamAdapter<Element, AsPure<TValue>>)
		constexpr auto read_space(
			Size const &   size,
			TValue const & value,
			TOption && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			for (auto & index : SizeRange{size}) {
				thiz.read_constant(value, as_forward<TOption>(option) ...);
			}
			return;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename TElement> requires
		AutomaticConstraint
	using AccessBasicBinaryStreamView = BasicBinaryStreamView<TElement, StreamMode::Constant::access()>;

	template <typename TElement> requires
		AutomaticConstraint
	using InputBasicBinaryStreamView = BasicBinaryStreamView<TElement, StreamMode::Constant::input()>;

	template <typename TElement> requires
		AutomaticConstraint
	using OutputBasicBinaryStreamView = BasicBinaryStreamView<TElement, StreamMode::Constant::output()>;

	#pragma endregion

}
