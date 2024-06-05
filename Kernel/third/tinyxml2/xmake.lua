-- tinyxml2
-- 10.0.0 321ea883b7190d4e85cae5512a12e5eaa8f8731f
-- https://github.com/leethomason/tinyxml2

target('third.tinyxml2', function()
	set_group('source/third')
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
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
