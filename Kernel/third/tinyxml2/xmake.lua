-- tinyxml2
-- 9.0.0 1dee28e51f9175a31955b9791c74c430fe13dc82
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
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
