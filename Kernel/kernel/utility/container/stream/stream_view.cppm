module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.stream.stream_mode;

export namespace Twinning::Kernel {

	#pragma region type

	template <typename TElement, auto t_mode, template <typename, auto> typename TMListView = ListView> requires
		CategoryConstraint<IsPureInstance<TElement>>
		&& (IsSameV<t_mode, StreamMode>)
	class StreamView {

	private:

		using IOStream = StreamView<TElement, StreamMode::Constant::io(), TMListView>;

		using IStream = StreamView<TElement, StreamMode::Constant::i(), TMListView>;

		using OStream = StreamView<TElement, StreamMode::Constant::o(), TMListView>;

	public:

		using Element = TElement;

		inline static constexpr auto mode = StreamMode{t_mode};

		using ListView = TMListView<Element, mbox<Boolean>(t_mode == StreamMode::Constant::i())>;

		using QElement = typename ListView::QElement;

		using QIterator = typename ListView::QIterator;

	protected:

		ListView m_view;

		Size m_position;

	public:

		#pragma region structor

		~StreamView (
		) = default;

		// ----------------

		StreamView (
		) :
			m_view{},
			m_position{} {
		}

		StreamView (
			StreamView const & that
		) = default;

		StreamView (
			StreamView && that
		) = default;

		// ----------------

		explicit StreamView (
			ListView const & view,
			Size const &     position = k_begin_index
		) :
			m_view{view},
			m_position{position} {
			assert_test(position <= view.size());
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			StreamView const & that
		) -> StreamView & = default;

		auto operator = (
			StreamView && that
		) -> StreamView & = default;

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

		#pragma region set

		auto reset (
		) -> Void {
			thiz.m_view.reset();
			thiz.m_position = k_begin_index;
			return;
		}

		auto set (
			ListView const & view,
			Size const &     position = k_begin_index
		) -> Void {
			assert_test(position <= view.size());
			thiz.m_view = view;
			thiz.m_position = position;
			return;
		}

		#pragma endregion

		#pragma region data

		auto data (
		) const -> QIterator {
			return thiz.m_view.begin();
		}

		#pragma endregion

		#pragma region size

		auto size (
		) const -> Size {
			return thiz.m_view.size();
		}

		#pragma endregion

		#pragma region position

		auto position (
		) const -> Size {
			return thiz.m_position;
		}

		// ----------------

		auto full (
		) const -> Boolean {
			return thiz.position() >= thiz.size();
		}

		auto reserve (
		) const -> Size {
			return thiz.size() - thiz.position();
		}

		#pragma endregion

		#pragma region set position

		auto set_position (
			Size const & position
		) -> Void {
			assert_test(position <= thiz.size());
			thiz.m_position = position;
			return;
		}

		// ----------------

		auto backward (
			Size const & size = 1_sz
		) -> Void {
			assert_test(size <= thiz.position());
			return thiz.set_position(thiz.position() - size);
		}

		auto forward (
			Size const & size = 1_sz
		) -> Void {
			assert_test(size <= thiz.reserve());
			return thiz.set_position(thiz.position() + size);
		}

		// ----------------

		auto backward_to_begin (
		) -> Void {
			return thiz.set_position(k_begin_index);
		}

		auto forward_to_end (
		) -> Void {
			return thiz.set_position(thiz.size());
		}

		#pragma endregion

		#pragma region view

		auto view (
		) -> ListView {
			return ListView{thiz.data(), thiz.size()};
		}

		auto sub_view (
			Size const & begin,
			Size const & count
		) -> ListView {
			assert_test(begin + count <= thiz.size());
			return ListView{thiz.data() + begin, count};
		}

		// ----------------

		auto previous_view (
			Size const & size
		) -> ListView {
			assert_test(size <= thiz.position());
			return thiz.sub_view(thiz.position() - size, size);
		}

		auto next_view (
			Size const & size
		) -> ListView {
			assert_test(size <= thiz.reserve());
			return thiz.sub_view(thiz.position(), size);
		}

		// ----------------

		auto stream_view (
		) -> ListView {
			return thiz.sub_view(k_begin_index, thiz.position());
		}

		auto reserve_view (
		) -> ListView {
			return thiz.sub_view(thiz.position(), thiz.size() - thiz.position());
		}

		#pragma endregion

		#pragma region query

		auto current_pointer (
		) -> QIterator {
			assert_test(thiz.position() <= thiz.size());
			return thiz.data() + thiz.position();
		}

		// ----------------

		auto current (
		) -> QElement & {
			assert_test(!thiz.full());
			return thiz.m_view[thiz.m_position];
		}

		auto next (
		) -> QElement & {
			auto & current = thiz.current();
			thiz.forward();
			return current;
		}

		// ----------------

		auto forward_view (
			Size const & size
		) -> ListView {
			auto view = thiz.next_view(size);
			thiz.forward(size);
			return view;
		}

		#pragma endregion

		#pragma region write & read

		auto write (
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::o() || mode == StreamMode::Constant::io()) {
			assert_test(!thiz.full());
			thiz.next() = value;
			return;
		}

		auto read (
			Element & value
		) -> Void requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
			assert_test(!thiz.full());
			value = thiz.next();
			return;
		}

		// ----------------

		auto write_constant (
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::o() || mode == StreamMode::Constant::io()) {
			thiz.write(value);
			return;
		}

		auto read_constant (
			Element const & value
		) -> Void requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
			auto temporary_value = Element{};
			thiz.read(temporary_value);
			assert_test(temporary_value == value);
			return;
		}

		// ----------------

		auto read_of (
		) -> Element requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
			auto value = Element{};
			thiz.read(value);
			return value;
		}

		#pragma endregion

		#pragma region write & read space

		auto write_space (
			Element const & value,
			Size const &    size
		) -> Void requires
			(mode == StreamMode::Constant::o() || mode == StreamMode::Constant::io()) {
			assert_test(size <= thiz.reserve());
			auto space = thiz.forward_view(size);
			for (auto & element : space) {
				element = value;
			}
			return;
		}

		auto read_space (
			Element const & value,
			Size const &    size
		) -> Void requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
			assert_test(size <= thiz.reserve());
			auto space = thiz.forward_view(size);
			for (auto & element : space) {
				assert_test(element == value);
			}
			return;
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

	#pragma region alias

	template <typename Element, template <typename, auto> typename ListView = ListView> requires
		AutoConstraint
	using IOStreamView = StreamView<Element, StreamMode::Constant::io(), ListView>;

	template <typename Element, template <typename, auto> typename ListView = ListView> requires
		AutoConstraint
	using IStreamView = StreamView<Element, StreamMode::Constant::i(), ListView>;

	template <typename Element, template <typename, auto> typename ListView = ListView> requires
		AutoConstraint
	using OStreamView = StreamView<Element, StreamMode::Constant::o(), ListView>;

	#pragma endregion

}
