-- shell

target('shell', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'./**.hpp',
		{ install = false }
	)
	add_files(
		'./**.cppm',
		'./**.cpp',
		m.root .. '/../common/cpp/std.cppm',
		{}
	)
	add_includedirs(
		m.root .. '',
		{}
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('helper')
		helper.apply_condition_definition_basic(target)
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_regular(target)
	end)
	set_runargs(
		m.root .. '/../.local/test/kernel',
		m.root .. '/../.local/test/script/main.js',
		m.root .. '/../.local/test',
		{ private = true }
	)
end)
