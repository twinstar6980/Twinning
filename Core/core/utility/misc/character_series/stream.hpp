#pragma once

#include "core/utility/container/stream/stream_view.hpp"
#include "core/utility/misc/character_series/container.hpp"
#include "core/utility/misc/character_series/type.hpp"
#include "core/utility/misc/character_series/stream_adapter.hpp"

namespace TwinKleS::Core {

	#pragma region type

	template <auto t_method> requires
		CategoryConstraint<>
		&& (IsSameV<t_method, StreamMethod>)
	class CharacterStreamView :
		public StreamView<Character, t_method, BasicCharacterListView> {

	private: //

		using StreamView = StreamView<Character, t_method, BasicCharacterListView>;

		using IOStream = CharacterStreamView<StreamMethod::io>;

		using IStream = CharacterStreamView<StreamMethod::i>;

		using OStream = CharacterStreamView<StreamMethod::o>;

	public: //

		using typename StreamView::Element;

		using StreamView::method;

		using typename StreamView::ListView;

		using typename StreamView::QElement;

		using typename StreamView::QIterator;

	public: //

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
		) -> CharacterStreamView& = default;

		auto operator = (
			CharacterStreamView && that
		) -> CharacterStreamView& = default;

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
			CharacterStreamAdapter<AsPure<That>>::write(thiz, as_forward<That>(that), as_forward<Option>(option)...);
			return;
		}

		template <typename That, typename ...Option> requires
			CategoryConstraint<IsValid<That> && IsValid<Option...>>
		auto read (
			That &&      that,
			Option && ...option
		) -> Void requires
			(method == StreamMethod::i || method == StreamMethod::io) {
			CharacterStreamAdapter<AsPure<That>>::read(thiz, as_forward<That>(that), as_forward<Option>(option)...);
			return;
		}

		// ----------------

		template <typename That = Character, typename ...Option> requires
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

	using IOCharacterStreamView = CharacterStreamView<StreamMethod::io>;

	using ICharacterStreamView = CharacterStreamView<StreamMethod::i>;

	using OCharacterStreamView = CharacterStreamView<StreamMethod::o>;

	#pragma endregion

}
