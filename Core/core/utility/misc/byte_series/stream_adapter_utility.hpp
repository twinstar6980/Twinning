#pragma once

#include "core/utility/misc/byte_series/stream.hpp"

namespace TwinKleS::Core {

	#pragma region check

	template <typename It>
	concept IsHasByteStreamAdapterWithRegularStaticSize =
	CategoryConstraint<IsPureInstance<It>>
	&& (requires { { ByteStreamAdapter<It>::static_size() } -> IsSame<Size>; })
	;

	template <typename It>
	concept IsHasByteStreamAdapterWithRegularSize =
	CategoryConstraint<IsPureInstance<It>>
	&& (requires { { ByteStreamAdapter<It>::size(declare<It &>()) } -> IsSame<Size>; })
	;

	template <typename It>
	concept IsHasByteStreamAdapterWithRegularWrite =
	CategoryConstraint<IsPureInstance<It>>
	&& (requires { { ByteStreamAdapter<It>::write(declare<OByteStreamView &>(), declare<It const &>()) } -> IsSame<Void>; })
	;

	template <typename It>
	concept IsHasByteStreamAdapterWithRegularRead =
	CategoryConstraint<IsPureInstance<It>>
	&& (requires { { ByteStreamAdapter<It>::read(declare<IByteStreamView &>(), declare<It &>()) } -> IsSame<Void>; })
	;

	#pragma endregion

	#pragma region adapter for aggregate by reflection

	template <typename TType, typename TFieldPackage> requires
		CategoryConstraint<IsPureInstance<TType> && IsPureInstance<TFieldPackage>>
		&& (IsTypePackage<TFieldPackage>)
	struct ByteStreamAdapterForAggregateByField {

		using Type = TType;

		using FieldPackage = TFieldPackage;

		// ----------------

		static constexpr auto has_static_size (
		) -> Boolean {
			auto result = k_true;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					if constexpr (!IsHasByteStreamAdapterWithRegularStaticSize<AsPure<decltype(Field::value_of(declare<Type &>()))>>) {
						result = k_false;
					}
					return;
				}
			);
			return result;
		}

		static constexpr auto has_size (
		) -> Boolean {
			auto result = k_true;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					if constexpr (!IsHasByteStreamAdapterWithRegularSize<AsPure<decltype(Field::value_of(declare<Type &>()))>>) {
						result = k_false;
					}
					return;
				}
			);
			return result;
		}

		static constexpr auto has_write (
		) -> Boolean {
			auto result = k_true;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					if constexpr (!IsHasByteStreamAdapterWithRegularWrite<AsPure<decltype(Field::value_of(declare<Type &>()))>>) {
						result = k_false;
					}
					return;
				}
			);
			return result;
		}

		static constexpr auto has_read (
		) -> Boolean {
			auto result = k_true;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					if constexpr (!IsHasByteStreamAdapterWithRegularRead<AsPure<decltype(Field::value_of(declare<Type &>()))>>) {
						result = k_false;
					}
					return;
				}
			);
			return result;
		}

		// ----------------

		using ThisI = IByteStreamView;

		using ThisO = OByteStreamView;

		using That = TType;

		// ----------------

		static constexpr auto static_size (
		) -> Size requires
			(has_static_size().value) {
			auto result = k_none_size;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					result += bs_static_size<AsPure<decltype(Field::value_of(declare<That const &>()))>>();
					return;
				}
			);
			return result;
		}

		static auto size (
			That const & that
		) -> Size requires
			(has_size().value) {
			auto result = k_none_size;
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					result += bs_size(Field::value_of(that));
					return;
				}
			);
			return result;
		}

		static auto write (
			ThisO &      thix,
			That const & that
		) -> Void requires
			(has_write().value) {
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					thix.write(Field::value_of(that));
					return;
				}
			);
			return;
		}

		static auto read (
			ThisI & thix,
			That &  that
		) -> Void requires
			(has_read().value) {
			Reflection::iterate<FieldPackage>(
				[&] <typename Field> (Field) -> auto {
					thix.read(Field::value_of(that));
					return;
				}
			);
			return;
		}

	};

	// ----------------

	template <typename TType> requires
		CategoryConstraint<IsPureInstance<TType>>
	struct ByteStreamAdapterForAggregateByInformation :
		ByteStreamAdapterForAggregateByField<TType, typename Reflection::Information<TType>::MemberVariable> {
	};

	#pragma endregion

}

#define M_byte_stream_adapter_for_aggregate_by_field_of(_type, _member_variable, _)\
	struct ByteStreamAdapter<_type>  {\
		\
		using ThisI = IByteStreamView;\
		\
		using ThisO = OByteStreamView;\
		\
		using That = _type;\
		\
		using Type = _type;\
		\
		using FieldPackage = M_reflection_field_member_variable_of_n(M_wrap(_member_variable));\
		\
		static constexpr auto static_size (\
		) -> Size requires\
			(ByteStreamAdapterForAggregateByField<Type, FieldPackage>::has_static_size().value) {\
			return ByteStreamAdapterForAggregateByField<Type, FieldPackage>::static_size();\
		}\
		\
		static auto size (\
			That const & that\
		) -> Size requires\
			(ByteStreamAdapterForAggregateByField<Type, FieldPackage>::has_size().value) {\
			return ByteStreamAdapterForAggregateByField<Type, FieldPackage>::size(that);\
		}\
		\
		static auto write (\
			ThisO &       thix,\
			That const &  that\
		) -> Void requires\
			(ByteStreamAdapterForAggregateByField<Type, FieldPackage>::has_write().value) {\
			return ByteStreamAdapterForAggregateByField<Type, FieldPackage>::write(thix, that);\
		}\
		\
		static auto read (\
			ThisI & thix,\
			That &  that\
		) -> Void requires\
			(ByteStreamAdapterForAggregateByField<Type, FieldPackage>::has_read().value) {\
			return ByteStreamAdapterForAggregateByField<Type, FieldPackage>::read(thix, that);\
		}\
		\
	}

#define M_byte_stream_adapter_for_aggregate_by_field_of_unique(_type, _member_variable, _)\
	template <typename TType> requires\
		AutoConstraint\
		&& (IsSame<TType, _type>)\
	M_byte_stream_adapter_for_aggregate_by_field_of(TType, M_wrap(_member_variable), _)

// ----------------

#define M_byte_stream_adapter_for_aggregate_by_information_of(...)\
	struct ByteStreamAdapter<__VA_ARGS__> :\
		ByteStreamAdapterForAggregateByInformation<__VA_ARGS__> {\
	}

#define M_byte_stream_adapter_for_aggregate_by_information_of_unique(...)\
	template <>\
	struct ByteStreamAdapter<__VA_ARGS__> :\
		ByteStreamAdapterForAggregateByInformation<__VA_ARGS__> {\
	}
