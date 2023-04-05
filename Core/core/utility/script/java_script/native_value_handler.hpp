#pragma once

#include "core/utility/container/wrapper/wrapper.hpp"
#include "core/utility/null.hpp"
#include "core/utility/miscellaneous/byte_series/container.hpp"
#include "core/utility/string/string.hpp"
#include "core/utility/data/json/value.hpp"

namespace TwinStar::Core::JavaScript {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
	class NativeValueHandler {

	public:

		using Value = TValue;

	protected:

		Variant<Null, Pointer<Value>, std::shared_ptr<Value>> m_value{};

	protected:

		#pragma region structor

		explicit NativeValueHandler (
			Pointer<Value> const & value,
			Boolean const &        is_holder
		) :
			NativeValueHandler{} {
			if (is_holder) {
				thiz.m_value.template set<std::shared_ptr<Value>>(value.value);
			} else {
				thiz.m_value.template set<Pointer<Value>>(value);
			}
		}

		#pragma endregion

	public:

		#pragma region structor

		~NativeValueHandler (
		) = default;

		// ----------------

		NativeValueHandler (
		) = default;

		NativeValueHandler (
			NativeValueHandler const & that
		) = default;

		NativeValueHandler (
			NativeValueHandler && that
		) = default;

		#pragma endregion

		#pragma region operator

		auto operator = (
			NativeValueHandler const & that
		) -> NativeValueHandler& = default;

		auto operator = (
			NativeValueHandler && that
		) -> NativeValueHandler& = default;

		#pragma endregion

		#pragma region value

		auto value (
		) -> Value& {
			assert_test(!thiz.m_value.template is<Null>());
			if (thiz.m_value.template is<Pointer<Value>>()) {
				return *thiz.m_value.template get<Pointer<Value>>();
			} else if (thiz.m_value.template is<std::shared_ptr<Value>>()) {
				return *thiz.m_value.template get<std::shared_ptr<Value>>();
			} else {
				throw ImpossibleException{};
			}
		}

		#pragma endregion

	public:

		#pragma region create

		static auto new_instance (
			Value & value
		) -> NativeValueHandler {
			return NativeValueHandler{make_pointer(&value), k_true};
		}

		static auto new_reference (
			Value & value
		) -> NativeValueHandler {
			return NativeValueHandler{make_pointer(&value), k_false};
		}

		template <typename ... Argument> requires
			CategoryConstraint<IsValid<Argument ...>>
			&& (IsConstructible<Value, Argument && ...>)
		static auto new_instance_allocate (
			Argument && ... argument
		) -> NativeValueHandler {
			return new_instance(*allocate_instance<Value>(as_forward<Argument>(argument) ...));
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsNativeValueHandler = IsTemplateInstanceOfT<It, NativeValueHandler>;

	#pragma endregion

}
