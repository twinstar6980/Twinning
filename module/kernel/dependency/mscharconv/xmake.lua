target('dependency.mscharconv', function()
	set_group('source/dependency')
	set_kind('headeronly')
	add_headerfiles(
		'origin/include/msstl/converted/charconv.inl',
		'origin/include/msstl/converted/m_floating_type_traits.inl',
		'origin/include/msstl/converted/xbit_ops.h.inl',
		'origin/include/msstl/converted/xcharconv.h.inl',
		'origin/include/msstl/converted/xcharconv_ryu.h.inl',
		'origin/include/msstl/converted/xcharconv_ryu_tables.h.inl',
		'origin/include/msstl/charconv.hpp',
		'origin/include/msstl/charconv_fwd.hpp',
		'origin/include/msstl/charconv_impl.inl',
		'origin/include/msstl/util.hpp',
		{ install = false }
	)
	add_files(
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_defines(
		{ public = true }
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
		return
	end)
end)
