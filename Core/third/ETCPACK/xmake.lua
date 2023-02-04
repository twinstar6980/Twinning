-- ETCPACK
-- 2.74 14a64d9d19318fb9f81ce339b7103ffa0f1781d7
-- https://github.com/Ericsson/ETCPACK

target('third.ETCPACK', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./image.h',
		{ install = false }
	)
	add_files(
		'./image.cxx',
		'./etcdec.cxx',
		'./etcpack.cxx',
		{}
	)
	add_headerfiles(
		'./_etcpack.cxx', -- original code
		'./_etcpack.hpp', -- custom interface
		{ install = false }
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
