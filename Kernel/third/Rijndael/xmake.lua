-- Rijndael
-- ? ?
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
