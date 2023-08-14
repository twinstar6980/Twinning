-- implement

target('implement', function()
	set_group('source')
	set_kind('shared')
	add_headerfiles(
		'./common.hpp',
		'./language.hpp',
		'./base_explorer_command.hpp',
		'./forward_explorer_command.hpp',
		'./forward_explorer_command_exposed.hpp',
		{ install = false }
	)
	add_files(
		'./dllmain.cpp',
		'./resource/library.def',
		'./resource/resource.rc',
		{}
	)
	add_includedirs(
		m.root .. '',
		{ private = true }
	)
	add_defines(
		'_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING',
		{ private = true }
	)
	add_links(
		'WindowsApp',
		'User32',
		{ private = true }
	)
	add_packages(
		'vcpkg::wil',
		{ private = true }
	)
	on_load(function(target)
		import('custom')
		custom.apply_condition_definition_basic(target)
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_regular(target)
	end)
	after_build(function (target)
		import('custom')
		local m = custom.load_m(target)
		os.cp(target:targetfile(), m.root .. '/package/Asset/Library/implement.dll')
	end)
end)
