-- tinyfiledialogs
-- 3.13.2 b7221857534b14954a2835c7ef0ce007e68ba0e7
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
	if m.system:is('windows') then
		add_links(
			'ComDlg32',
			'Ole32',
			'User32',
			'shell32',
			{ private = true }
		)
	end
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
