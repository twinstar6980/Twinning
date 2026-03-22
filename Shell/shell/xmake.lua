-- shell

target('shell', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'**.hpp',
		{ install = false }
	)
	add_files(
		'**.cppm',
		'**.cpp',
		manifest.root .. '/../common/cpp/std.cppm',
		{}
	)
	add_includedirs(
		manifest.root .. '',
		{}
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_condition_definition_basic(target)
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_regular(target)
		return
	end)
	set_runargs(
		manifest.root .. '/../.local/test/kernel',
		manifest.root .. '/../.local/test/script/main.js',
		manifest.root .. '/../.local/test',
		{ private = true }
	)
end)
