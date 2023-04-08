-- tinyfiledialogs
-- 3.10 35862503e0fc3ac96f8c65086c854a250768398f
-- https://sourceforge.net/projects/tinyfiledialogs

target('third.tinyfiledialogs', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./tinyfiledialogs.h',
		{ install = false }
	)
	add_files(
		'./tinyfiledialogs.c',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
