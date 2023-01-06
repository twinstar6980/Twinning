#pragma once

#include "core/utility/base_wrapper/base.hpp"

namespace TwinStar::Core {

	#pragma region type

	template <typename TValue> requires
		CategoryConstraint<IsPureInstance<TValue>>
		&& (IsBaseWrapperValue<TValue>)
	class EnumerationWrapper :
		public BaseWrapper<TValue> {

	private:

		using BaseWrapper = BaseWrapper<TValue>;

	public:

		using typename BaseWrapper::Value;

		using Underlying = IntegerWrapper<std::underlying_type_t<Value>>;

	public:

		#pragma region structor

		constexpr ~EnumerationWrapper (
		) = default;

		// ----------------

		constexpr EnumerationWrapper (
		) = default;

		constexpr EnumerationWrapper (
			EnumerationWrapper const & that
		) = default;

		constexpr EnumerationWrapper (
			EnumerationWrapper && that
		) = default;

		// ----------------

		using BaseWrapper::BaseWrapper;

		#pragma endregion

		#pragma region operator

		constexpr auto operator = (
			EnumerationWrapper const & that
		) -> EnumerationWrapper& = default;

		constexpr auto operator = (
			EnumerationWrapper && that
		) -> EnumerationWrapper& = default;

		#pragma endregion

		#pragma region underlying

		auto as_underlying (
		) -> Underlying& {
			return self_cast<Underlying>(thiz);
		}

		auto as_underlying (
		) const -> Underlying const& {
			return self_cast<Underlying>(thiz);
		}

		// ----------------

		constexpr auto underlying (
		) const -> Underlying {
			return cbw<Underlying>(thiz);
		}

		#pragma endregion

	};

	// ----------------

	template <typename It>
	concept IsEnumerationWrapper = IsTemplateInstanceOfT<It, EnumerationWrapper>;

	#pragma endregion

	#pragma region operator

	template <typename It> requires
		CategoryConstraint<IsPureInstance<It>>
		&& (IsEnumerationWrapper<It>)
	inline constexpr auto operator == (
		It const & thix,
		It const & that
	) -> bool {
		return thix.value == that.value;
	}

	#pragma endregion

	#pragma region alias

	template <typename Value> requires
		AutoConstraint
	using Enumeration = EnumerationWrapper<Value>;

	#pragma endregion

}

#define M_enumeration_item(_name)\
	_name,

#define M_enumeration_item_reflection(_name)\
	Trait::Reflection::EnumerationValueField<Trait::Reflection::make_string(#_name), Type::_name>,

#define M_enumeration_item_getter(_name)\
	inline static constexpr auto _name (\
	) -> Type {\
		return Type{Type::Value::_name};\
	}

// ----------------

#define M_enumeration(_type, _name, _)\
	struct _type :\
		Enumeration<decltype([]{\
			enum class Type : ZIntegerU8 {\
				M_map(M_enumeration_item, _name)\
			};\
			return declare<Type>();\
		}())> {\
		using EnumerationWrapper<typename _type::Value>::EnumerationWrapper;\
		struct Reflection {\
			using Type = typename EnumerationWrapper<typename _type::Value>::Value;\
			using EnumerationValue = AsTypePackageRemoveTail<TypePackage<\
				M_map(M_enumeration_item_reflection, _name)\
				None\
			>, 1_szz>;\
		};\
		struct Constant {\
			using Type = _type;\
			M_map(M_enumeration_item_getter, _name)\
		};\
	}
