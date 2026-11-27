module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.container.stream.stream_flag;
import twinning.kernel.utility.range.integer_range;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_mode, template <typename, auto> typename TMArrayView = ArrayView, typename TFlag = StreamFlag> requires
		CategoryConstraint<IsPureInstance<TElement> && IsPureInstance<TFlag>>
		&& (IsSameOf<t_mode, StreamMode>)
	class StreamView {

	private:

		using AccessStream = StreamView<TElement, StreamMode::Constant::access(), TMArrayView, TFlag>;

		using InputStream = StreamView<TElement, StreamMode::Constant::input(), TMArrayView, TFlag>;

		using OutputStream = StreamView<TElement, StreamMode::Constant::output(), TMArrayView, TFlag>;

	public:

		using Element = TElement;

		inline static constexpr auto mode = StreamMode{t_mode};

		using ArrayView = TMArrayView<Element, make_box<Boolean>(t_mode == StreamMode::Constant::input())>;

		using Flag = TFlag;

		using QualifyElement = ArrayView::QualifyElement;

		using QualifyIterator = ArrayView::QualifyIterator;

	protected:

		ArrayView m_view;

		Size m_position;

		Flag m_flag;

	public:

		#pragma region constructor

		constexpr ~StreamView(
		) = default;

		// ----------------

		constexpr StreamView(
		) :
			m_view{},
			m_position{},
			m_flag{} {
			return;
		}

		constexpr StreamView(
			StreamView const & that
		) = default;

		constexpr StreamView(
			StreamView && that
		) = default;

		// ----------------

		explicit constexpr StreamView(
			ArrayView const & view,
			Size const &      position = 0_sz,
			Flag const &      flag = Flag{}
		) :
			StreamView{} {
			thiz.set(view, position, flag);
			return;
		}

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			StreamView const & that
		) -> StreamView & = default;

		constexpr auto operator =(
			StreamView && that
		) -> StreamView & = default;

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

		#pragma region set

		constexpr auto reset(
		) -> Void {
			thiz.m_view.reset();
			thiz.m_position = 0_sz;
			thiz.m_flag = Flag{};
			return;
		}

		constexpr auto set(
			ArrayView const & view,
			Size const &      position,
			Flag const &      flag
		) -> Void {
			assert_test(position <= view.size());
			thiz.m_view = view;
			thiz.m_position = position;
			thiz.m_flag = flag;
			return;
		}

		#pragma endregion

		#pragma region view

		constexpr auto view(
		) const -> ArrayView {
			return thiz.m_view;
		}

		#pragma endregion

		#pragma region flag

		constexpr auto flag(
		) const -> Flag {
			return thiz.m_flag;
		}

		#pragma endregion

		#pragma region position

		constexpr auto position(
		) const -> Size {
			return thiz.m_position;
		}

		constexpr auto size(
		) const -> Size {
			return thiz.m_view.size();
		}

		// ----------------

		constexpr auto full(
		) const -> Boolean {
			return thiz.position() >= thiz.size();
		}

		constexpr auto reserve(
		) const -> Size {
			return thiz.size() - thiz.position();
		}

		#pragma endregion

		#pragma region set position

		constexpr auto set_position(
			Size const & position
		) -> Void {
			assert_test(position <= thiz.size());
			thiz.m_position = position;
			return;
		}

		// ----------------

		constexpr auto backward(
			Size const & size = 1_sz
		) -> Void {
			assert_test(size <= thiz.position());
			return thiz.set_position(thiz.position() - size);
		}

		constexpr auto forward(
			Size const & size = 1_sz
		) -> Void {
			assert_test(size <= thiz.reserve());
			return thiz.set_position(thiz.position() + size);
		}

		// ----------------

		constexpr auto backward_to_begin(
		) -> Void {
			return thiz.set_position(0_sz);
		}

		constexpr auto forward_to_end(
		) -> Void {
			return thiz.set_position(thiz.size());
		}

		#pragma endregion

		#pragma region query

		constexpr auto current(
		) const -> QualifyIterator {
			return thiz.m_view.iterator(thiz.position());
		}

		#pragma endregion

		#pragma region split

		constexpr auto split_copy(
		) -> StreamView {
			return StreamView{thiz.m_view, thiz.position(), thiz.flag()};
		}

		constexpr auto split_sub(
			Size const & begin,
			Size const & count
		) -> StreamView {
			assert_test(begin + count <= thiz.size());
			return StreamView{thiz.m_view.sub(begin, count), 0_sz, thiz.flag()};
		}

		constexpr auto split_all(
		) -> StreamView {
			return thiz.split_sub(0_sz, thiz.size());
		}

		// ----------------

		constexpr auto split_next(
			Size const & count
		) -> StreamView {
			assert_test(count <= thiz.reserve());
			return thiz.split_sub(thiz.position(), count);
		}

		// ----------------

		constexpr auto split_stream(
		) -> StreamView {
			return thiz.split_sub(0_sz, thiz.position());
		}

		constexpr auto split_reserve(
		) -> StreamView {
			return thiz.split_sub(thiz.position(), thiz.size() - thiz.position());
		}

		// ----------------

		constexpr auto split_forward(
			Size const & count
		) -> StreamView {
			auto view = thiz.split_next(count);
			thiz.forward(count);
			return view;
		}

		#pragma endregion

		#pragma region exchange

		constexpr auto write(
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			assert_test(!thiz.full());
			thiz.m_view[thiz.m_position] = value;
			thiz.forward();
			return;
		}

		constexpr auto read(
			Element & value
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			assert_test(!thiz.full());
			value = thiz.m_view[thiz.m_position];
			thiz.forward();
			return;
		}

		// ----------------

		constexpr auto read_of(
		) -> Element requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto value = Element{};
			thiz.read(value);
			return value;
		}

		// ----------------

		constexpr auto write_constant(
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			thiz.write(value);
			return;
		}

		constexpr auto read_constant(
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto temporary_value = Element{};
			thiz.read(temporary_value);
			assert_test(temporary_value == value);
			return;
		}

		// ----------------

		constexpr auto write_space(
			Size const &    size,
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			assert_test(size <= thiz.reserve());
			for (auto & index : SizeRange{size}) {
				thiz.write_constant(value);
			}
			return;
		}

		constexpr auto read_space(
			Size const &    size,
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			assert_test(size <= thiz.reserve());
			for (auto & index : SizeRange{size}) {
				thiz.read_constant(value);
			}
			return;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename TElement, template <typename, auto> typename TMArrayView = ArrayView, typename TFlag = StreamFlag> requires
		AutomaticConstraint
	using AccessStreamView = StreamView<TElement, StreamMode::Constant::access(), TMArrayView, TFlag>;

	template <typename TElement, template <typename, auto> typename TMArrayView = ArrayView, typename TFlag = StreamFlag> requires
		AutomaticConstraint
	using InputStreamView = StreamView<TElement, StreamMode::Constant::input(), TMArrayView, TFlag>;

	template <typename TElement, template <typename, auto> typename TMArrayView = ArrayView, typename TFlag = StreamFlag> requires
		AutomaticConstraint
	using OutputStreamView = StreamView<TElement, StreamMode::Constant::output(), TMArrayView, TFlag>;

	#pragma endregion

}
