module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.miscellaneous.character_series.stream;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.stream.stream_mode;
import twinning.kernel.utility.container.stream.stream_view;
import twinning.kernel.utility.miscellaneous.character_series.container;
import twinning.kernel.utility.miscellaneous.character_series.stream_adapter;

export namespace Twinning::Kernel {

	#pragma region type

	template <auto t_mode> requires
		CategoryConstraint<>
		&& (IsSameOf<t_mode, StreamMode>)
	class CharacterStreamView :
		public StreamView<Character, t_mode, BasicCharacterListView> {

	private:

		using StreamView = StreamView<Character, t_mode, BasicCharacterListView>;

		using AccessStream = CharacterStreamView<StreamMode::Constant::access()>;

		using InputStream = CharacterStreamView<StreamMode::Constant::input()>;

		using OutputStream = CharacterStreamView<StreamMode::Constant::output()>;

	public:

		using typename StreamView::Element;

		using StreamView::mode;

		using typename StreamView::ListView;

		using typename StreamView::QualifyElement;

		using typename StreamView::QualifyIterator;

	public:

		#pragma region structor

		~CharacterStreamView (
		) = default;

		// ----------------

		CharacterStreamView (
		) = default;

		CharacterStreamView (
			CharacterStreamView const & that
		) = default;

		CharacterStreamView (
			CharacterStreamView && that
		) = default;

		// ----------------

		using StreamView::StreamView;

		#pragma endregion

		#pragma region operator

		auto operator = (
			CharacterStreamView const & that
		) -> CharacterStreamView & = default;

		auto operator = (
			CharacterStreamView && that
		) -> CharacterStreamView & = default;

		// ----------------

		implicit operator InputStream & () requires
			(mode == StreamMode::Constant::access()) {
			return thiz.as_input_stream();
		}

		implicit operator OutputStream & () requires
			(mode == StreamMode::Constant::access()) {
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
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			CharacterStreamAdapter<AsPure<That>>::write(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read (
			That &&       that,
			Option && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			CharacterStreamAdapter<AsPure<That>>::read(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		// ----------------

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto write_constant (
			That const &  that,
			Option && ... option
		) -> Void requires
			(mode == StreamMode::Constant::output() || mode == StreamMode::Constant::access()) {
			thiz.write(that, as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read_constant (
			That const &  that,
			Option && ... option
		) -> Void requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto temporary_that = That{};
			thiz.read(temporary_that, as_forward<Option>(option) ...);
			assert_test(temporary_that == that);
			return;
		}

		// ----------------

		template <typename That = Character, typename ... Option> requires
			CategoryConstraint<IsPureInstance<That> && IsValid<Option ...>>
		auto read_of (
			Option && ... option
		) -> That requires
			(mode == StreamMode::Constant::input() || mode == StreamMode::Constant::access()) {
			auto that = That{};
			thiz.read(that, as_forward<Option>(option) ...);
			return that;
		}

		#pragma endregion

		#pragma region mode cast

		auto as_input_stream (
		) -> InputStream & requires
			(mode == StreamMode::Constant::access()) {
			return self_cast<InputStream>(thiz);
		}

		auto as_output_stream (
		) -> OutputStream & requires
			(mode == StreamMode::Constant::access()) {
			return self_cast<OutputStream>(thiz);
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	using AccessCharacterStreamView = CharacterStreamView<StreamMode::Constant::access()>;

	using InputCharacterStreamView = CharacterStreamView<StreamMode::Constant::input()>;

	using OutputCharacterStreamView = CharacterStreamView<StreamMode::Constant::output()>;

	#pragma endregion

}
