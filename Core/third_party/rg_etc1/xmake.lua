-- rg_etc1
-- ?
-- https://github.com/richgel999/rg-etc1

target('rg_etc1', function()
	set_group('source/third_party')
	set_kind('static')
	add_headerfiles(
		'./rg_etc1.h',
		{ install = false }
	)
	add_files(
		'./rg_etc1.cpp',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
