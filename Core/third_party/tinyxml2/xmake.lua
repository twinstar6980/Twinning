-- tinyxml2
-- 9.0.0
-- https://github.com/leethomason/tinyxml2

target('tinyxml2', function()
	set_group('source/third_party')
	set_kind('static')
	add_headerfiles(
		'./tinyxml2.h',
		{ install = false }
	)
	add_files(
		'./tinyxml2.cpp',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
