-- avir
-- 3.1 1c8433cba92695ed69903175eae0fa527d1e8bc6
-- https://github.com/avaneev/avir

target('third.avir', function()
	set_group('source/third')
	set_kind('headeronly')
	add_headerfiles(
		'./avir.h',
		'./avir_dil.h',
		'./avir_float4_sse.h',
		'./avir_float8_avx.h',
		'./lancir.h',
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
