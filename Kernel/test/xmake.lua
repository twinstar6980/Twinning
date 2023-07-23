-- test

target('test', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		{ install = false }
	)
	add_files(
		'./main.cpp',
		{}
	)
	add_includedirs(
		m.root .. '',
		m.root .. '/../Shell',
		{ private = true }
	)
	add_defines(
		'_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING',
		{ private = true }
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
			{ private = true }
		)
	end
	on_load(function(target)
		import('custom')
		custom.apply_condition_definition_basic(target)
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_regular(target)
		custom.apply_import_vld_if_can(target)
	end)
	set_runargs(
		'/TwinStar.ToolKit/kernel',
		'/TwinStar.ToolKit/script/main.js',
		'/TwinStar.ToolKit',
		{ private = true }
	)
end)
