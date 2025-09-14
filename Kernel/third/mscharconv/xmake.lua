-- mscharconv
-- 1.2.3 5d2e3a6e9663a41207561c9510ad1e3d557b5c5e
-- https://github.com/iboB/mscharconv

target('third.mscharconv', function()
	set_group('source/third')
	set_kind('headeronly')
	add_headerfiles(
		'./include/msstl/converted/charconv.inl',
		'./include/msstl/converted/m_floating_type_traits.inl',
		'./include/msstl/converted/xbit_ops.h.inl',
		'./include/msstl/converted/xcharconv.h.inl',
		'./include/msstl/converted/xcharconv_ryu.h.inl',
		'./include/msstl/converted/xcharconv_ryu_tables.h.inl',
		'./include/msstl/charconv.hpp',
		'./include/msstl/charconv_fwd.hpp',
		'./include/msstl/charconv_impl.inl',
		'./include/msstl/util.hpp',
		{ install = false }
	)
	add_files(
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
