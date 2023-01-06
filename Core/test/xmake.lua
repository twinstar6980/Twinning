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
		m.root .. '/../Shell',
		{ private = true }
	)
	add_defines(
		'_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING',
		{ private = true }
	)
	add_deps(
		'core',
		{}
	)
	if m.system:is('windows') then
		add_files(
			m.root .. '/../Shell/shell/windows/application.manifest',
			{}
		)
		add_links(
			'Ole32',
			{ private = true }
		)
	end
	if m.system:is('linux') then
		add_links(
			'pthread',
			{ private = true }
		)
	end
	set_runargs(
		'_',
		'C:/Program Files/TwinStar/ToolKit/script/main.js',
		'true',
		{ private = true }
	)
	on_load(function(target)
		import('custom')
		custom.apply_condition_definition_basic(target)
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_regular(target)
		custom.apply_import_vld_if_can(target)
	end)
end)
