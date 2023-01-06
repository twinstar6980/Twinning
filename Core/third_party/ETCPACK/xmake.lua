-- ETCPACK
-- 2.74
-- https://github.com/Ericsson/ETCPACK

target('ETCPACK', function()
	set_group('source/third_party')
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
