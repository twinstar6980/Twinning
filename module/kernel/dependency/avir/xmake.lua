target('dependency.avir', function()
	set_group('source/dependency')
	set_kind('headeronly')
	add_headerfiles(
		'origin/avir.h',
		'origin/avir_dil.h',
		'origin/avir_float4_sse.h',
		'origin/avir_float8_avx.h',
		'origin/lancir.h',
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
