module;

// NOTE : EXPLAIN - T = typename, V = auto, M = template, N = ...

#include "kernel/common.hpp"

export module twinning.kernel.utility.trait.template_instance;
import twinning.kernel.utility.trait.base;
import twinning.kernel.utility.trait.category;

export namespace Twinning::Kernel::Trait {

	#pragma region concept

	template <typename It, template <typename> typename Template>
	concept IsTemplateInstanceOfT =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1> (Template<T1> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename, typename> typename Template>
	concept IsTemplateInstanceOfTT =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1, typename T2> (Template<T1, T2> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename, typename, auto> typename Template>
	concept IsTemplateInstanceOfTTV =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1, typename T2, auto t_3> (Template<T1, T2, t_3> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <auto> typename Template>
	concept IsTemplateInstanceOfV =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <auto t_1> (Template<t_1> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <auto, auto> typename Template>
	concept IsTemplateInstanceOfVV =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <auto t_1, auto t_2> (Template<t_1, t_2> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <auto, auto, auto, auto> typename Template>
	concept IsTemplateInstanceOfVVVV =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <auto t_1, auto t_2, auto t_3, auto t_4> (Template<t_1, t_2, t_3, t_4> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename, auto> typename Template>
	concept IsTemplateInstanceOfTV =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1, auto t_2> (Template<T1, t_2> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename, auto, template <typename, auto> typename> typename Template>
	concept IsTemplateInstanceOfTVMTVM =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1, auto t_2, template <typename, auto> typename TM3> (Template<T1, t_2, TM3> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename ...> typename Template>
	concept IsTemplateInstanceOfNT =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename ... T1> (Template<T1 ...> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <auto ...> typename Template>
	concept IsTemplateInstanceOfNV =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <auto ... t_1> (Template<t_1 ...> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename, typename ...> typename Template>
	concept IsTemplateInstanceOfTNT =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1, typename ... T2> (Template<T1, T2 ...> & it) {
				}(declare<It &>())
			};
		})
		;

	template <typename It, template <typename, auto, typename, typename ...> typename Template>
	concept IsTemplateInstanceOfTVTNT =
		CategoryConstraint<IsPureInstance<It>>
		&& (requires {
			{
				[] <typename T1, auto t_2, typename T3, typename ... T4> (Template<T1, t_2, T3, T4 ...> & it) {
				}(declare<It &>())
			};
		})
		;

	#pragma endregion

}
