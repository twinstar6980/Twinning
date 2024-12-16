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
	if m.system:is('windows', 'linux', 'macintosh') then
		add_deps(
			'third.tinyfiledialogs',
			{}
		)
	end
	if m.system:is('windows') then
		add_files(
			m.root .. '/../Shell/shell/resource/windows/application.manifest',
			{}
		)
		add_links(
			'Ole32',
			'User32',
			{ private = true }
		)
	end
	on_load(function(target)
		import('helper')
		helper.apply_condition_definition_basic(target)
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_regular(target)
		helper.import_vld_if_needed(target)
	end)
	set_runargs(
		'/Twinning/kernel',
		'/Twinning/script/main.js',
		'/Twinning',
		{ private = true }
	)
end)
