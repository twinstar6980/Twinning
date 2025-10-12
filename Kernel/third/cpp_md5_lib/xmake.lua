-- cpp_md5_lib
-- ? b330948a8660ebd5066c966a0b4654983999e43b
-- https://github.com/tiankonguse/cpp-md5-lib

target('third.cpp_md5_lib', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./include/md5.h',
		{ install = false }
	)
	add_files(
		'./src/md5.cpp',
		{}
	)
	add_includedirs(
		'./include',
		{ public = true }
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
	end)
end)
