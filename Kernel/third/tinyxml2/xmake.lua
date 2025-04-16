-- tinyxml2
-- 11.0.0 9148bdf719e997d1f474be6bcc7943881046dba1
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
