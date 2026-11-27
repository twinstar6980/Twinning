target('dependency.tinyxml2', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/tinyxml2.h',
		{ install = false }
	)
	add_files(
		'origin/tinyxml2.cpp',
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_defines(
		{ public = true }
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
		return
	end)
end)
