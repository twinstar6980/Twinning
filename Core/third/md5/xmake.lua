-- md5
-- ?
-- https://github.com/JieweiWei/md5

target('third.md5', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./md5.h',
		{ install = false }
	)
	add_files(
		'./md5.cpp',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
