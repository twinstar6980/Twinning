-- Rijndael
-- ?
-- ?

target('third.Rijndael', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./Rijndael.h',
		{ install = false }
	)
	add_files(
		'./Rijndael.cpp',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
