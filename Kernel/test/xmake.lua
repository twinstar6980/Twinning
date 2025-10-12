-- test

target('test', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'./**.hpp',
		{ install = false }
	)
	add_files(
		'./**.cpp',
		m.root .. '/../Shell/shell/**.cppm',
		m.root .. '/../common/cpp/std.cppm',
		{}
	)
	add_includedirs(
		m.root .. '',
		m.root .. '/../Shell',
		{}
	)
	add_deps(
		'kernel',
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_condition_definition_basic(target)
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_regular(target)
	end)
	set_runargs(
		m.root .. '/../.local/test/kernel',
		m.root .. '/../.local/test/script/main.js',
		m.root .. '/../.local/test',
		{ private = true }
	)
end)
