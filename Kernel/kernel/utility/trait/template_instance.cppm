module;

// NOTE: EXPLAIN: t = typename, v = auto, m = template, n = ...

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.template_instance;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename TIt, template <typename> typename TTemplate>
	concept IsTemplateInstanceOfTt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1>(TTemplate<T1> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, typename> typename TTemplate>
	concept IsTemplateInstanceOfTtt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1, typename T2>(TTemplate<T1, T2> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, typename, auto> typename TTemplate>
	concept IsTemplateInstanceOfTttv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1, typename T2, auto t_3>(TTemplate<T1, T2, t_3> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto> typename TTemplate>
	concept IsTemplateInstanceOfTv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <auto t_1>(TTemplate<t_1> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto, auto> typename TTemplate>
	concept IsTemplateInstanceOfTvv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <auto t_1, auto t_2>(TTemplate<t_1, t_2> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto, auto, auto, auto> typename TTemplate>
	concept IsTemplateInstanceOfTvvvv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <auto t_1, auto t_2, auto t_3, auto t_4>(TTemplate<t_1, t_2, t_3, t_4> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, auto> typename TTemplate>
	concept IsTemplateInstanceOfTtv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1, auto t_2>(TTemplate<T1, t_2> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, auto, template <typename, auto> typename> typename TTemplate>
	concept IsTemplateInstanceOfTtvmtvm =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1, auto t_2, template <typename, auto> typename TM3>(TTemplate<T1, t_2, TM3> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename ...> typename TTemplate>
	concept IsTemplateInstanceOfTnt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename ... T1>(TTemplate<T1 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto ...> typename TTemplate>
	concept IsTemplateInstanceOfTnv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <auto ... t_1>(TTemplate<t_1 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, typename ...> typename TTemplate>
	concept IsTemplateInstanceOfTtnt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1, typename ... T2>(TTemplate<T1, T2 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, auto, typename, typename ...> typename TTemplate>
	concept IsTemplateInstanceOfTtvtnt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[] <typename T1, auto t_2, typename T3, typename ... T4>(TTemplate<T1, t_2, T3, T4 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	#pragma endregion

}
