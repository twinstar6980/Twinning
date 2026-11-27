module;

// NOTE: EXPLAIN: t = typename, v = auto, m = template, n = ...

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.template_instance;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename TIt, template <typename> typename TMTemplate>
	concept IsTemplateInstanceOfTt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1>(TMTemplate<T1> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, typename> typename TMTemplate>
	concept IsTemplateInstanceOfTtt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1, typename T2>(TMTemplate<T1, T2> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, typename, auto> typename TMTemplate>
	concept IsTemplateInstanceOfTttv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1, typename T2, auto t_3>(TMTemplate<T1, T2, t_3> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto> typename TMTemplate>
	concept IsTemplateInstanceOfTv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<auto t_1>(TMTemplate<t_1> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto, auto> typename TMTemplate>
	concept IsTemplateInstanceOfTvv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<auto t_1, auto t_2>(TMTemplate<t_1, t_2> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto, auto, auto, auto> typename TMTemplate>
	concept IsTemplateInstanceOfTvvvv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<auto t_1, auto t_2, auto t_3, auto t_4>(TMTemplate<t_1, t_2, t_3, t_4> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, auto> typename TMTemplate>
	concept IsTemplateInstanceOfTtv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1, auto t_2>(TMTemplate<T1, t_2> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, auto, template <typename, auto> typename> typename TMTemplate>
	concept IsTemplateInstanceOfTtvmtvm =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1, auto t_2, template <typename, auto> typename TM3>(TMTemplate<T1, t_2, TM3> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename ...> typename TMTemplate>
	concept IsTemplateInstanceOfTnt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename ... T1>(TMTemplate<T1 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <auto ...> typename TMTemplate>
	concept IsTemplateInstanceOfTnv =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<auto ... t_1>(TMTemplate<t_1 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, typename ...> typename TMTemplate>
	concept IsTemplateInstanceOfTtnt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1, typename ... T2>(TMTemplate<T1, T2 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	template <typename TIt, template <typename, auto, typename, typename ...> typename TMTemplate>
	concept IsTemplateInstanceOfTtvtnt =
		CategoryConstraint<IsPureInstance<TIt>>
		&& (requires {
			{
				[]<typename T1, auto t_2, typename T3, typename ... T4>(TMTemplate<T1, t_2, T3, T4 ...> & it) {
				}(declare<TIt &>())
			};
		})
		;

	#pragma endregion

}
