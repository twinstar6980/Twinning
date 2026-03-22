-- test

target('test', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'**.hpp',
		{ install = false }
	)
	add_files(
		'**.cpp',
		manifest.root .. '/../Shell/shell/**.cppm',
		manifest.root .. '/../common/cpp/std.cppm',
		{}
	)
	add_includedirs(
		manifest.root .. '',
		manifest.root .. '/../Shell',
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
		return
	end)
	set_runargs(
		manifest.root .. '/../.local/test/kernel',
		manifest.root .. '/../.local/test/script/main.js',
		manifest.root .. '/../.local/test',
		{ private = true }
	)
end)
