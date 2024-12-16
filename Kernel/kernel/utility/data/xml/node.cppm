module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.data.xml.node;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.variant.enumerable_variant;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.container.map.map;
import twinning.kernel.utility.string.string;

export namespace Twinning::Kernel::XML {

	#pragma region declaration

	class Node;

	#pragma endregion

	#pragma region sub type

	struct Element {

		String name{};

		Map<String, String> attribute{};

		List<Node> child{};

		// ----------------

		inline friend auto operator == (
			Element const & thix,
			Element const & that
		) -> bool = default;

	};

	struct Text {

		String value{};

		Boolean cdata{};

		// ----------------

		inline friend auto operator == (
			Text const & thix,
			Text const & that
		) -> bool = default;

	};

	struct Comment {

		String value{};

		// ----------------

		inline friend auto operator == (
			Comment const & thix,
			Comment const & that
		) -> bool = default;

	};

	// ----------------

	M_enumeration(
		M_wrap(NodeType),
		M_wrap(
			element,
			text,
			comment,
		),
	);

	#pragma endregion

	#pragma region type

	class Node :
		public EnumerableVariant<NodeType, Element, Text, Comment> {

	public:

		#pragma region structor

		~Node (
		) = default;

		// ----------------

		Node (
		) = default;

		Node (
			Node const & that
		) = default;

		Node (
			Node && that
		) = default;

		// ----------------

		using EnumerableVariant::EnumerableVariant;

		#pragma endregion

		#pragma region operator

		auto operator = (
			Node const & that
		) -> Node & = default;

		auto operator = (
			Node && that
		) -> Node & = default;

		#pragma endregion

		#pragma region value

		auto is_element (
		) const -> Boolean {
			return thiz.is<Element>();
		}

		auto is_text (
		) const -> Boolean {
			return thiz.is<Text>();
		}

		auto is_comment (
		) const -> Boolean {
			return thiz.is<Comment>();
		}

		// ----------------

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_element (
			Argument && ... argument
		) -> Element & {
			return thiz.set<Element>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_text (
			Argument && ... argument
		) -> Text & {
			return thiz.set<Text>(as_forward<Argument>(argument) ...);
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
		auto set_comment (
			Argument && ... argument
		) -> Comment & {
			return thiz.set<Comment>(as_forward<Argument>(argument) ...);
		}

		// ----------------

		auto get_element (
		) -> Element & {
			return thiz.get<Element>();
		}

		auto get_text (
		) -> Text & {
			return thiz.get<Text>();
		}

		auto get_comment (
		) -> Comment & {
			return thiz.get<Comment>();
		}

		// ----------------

		auto get_element (
		) const -> Element const & {
			return thiz.get<Element>();
		}

		auto get_text (
		) const -> Text const & {
			return thiz.get<Text>();
		}

		auto get_comment (
		) const -> Comment const & {
			return thiz.get<Comment>();
		}

		#pragma endregion

	};

	#pragma endregion

}
