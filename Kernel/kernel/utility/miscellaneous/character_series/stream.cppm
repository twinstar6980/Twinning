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
		&& (IsSameV<t_mode, StreamMode>)
	class CharacterStreamView :
		public StreamView<Character, t_mode, BasicCharacterListView> {

	private:

		using StreamView = StreamView<Character, t_mode, BasicCharacterListView>;

		using IOStream = CharacterStreamView<StreamMode::Constant::io()>;

		using IStream = CharacterStreamView<StreamMode::Constant::i()>;

		using OStream = CharacterStreamView<StreamMode::Constant::o()>;

	public:

		using typename StreamView::Element;

		using StreamView::mode;

		using typename StreamView::ListView;

		using typename StreamView::QElement;

		using typename StreamView::QIterator;

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
			CharacterStreamAdapter<AsPure<That>>::write(thiz, as_forward<That>(that), as_forward<Option>(option) ...);
			return;
		}

		template <typename That, typename ... Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option ...>>
		auto read (
			That &&       that,
			Option && ... option
		) -> Void requires
			(mode == StreamMode::Constant::i() || mode == StreamMode::Constant::io()) {
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

		template <typename That = Character, typename ... Option> requires
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

	#pragma region alias

	using IOCharacterStreamView = CharacterStreamView<StreamMode::Constant::io()>;

	using ICharacterStreamView = CharacterStreamView<StreamMode::Constant::i()>;

	using OCharacterStreamView = CharacterStreamView<StreamMode::Constant::o()>;

	#pragma endregion

}
