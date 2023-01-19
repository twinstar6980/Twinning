-- implement

target('implement', function()
	set_group('source')
	set_kind('shared')
	add_headerfiles(
		'./version.hpp',
		'./common.hpp',
		'./base_command.hpp',
		'./method_invoke_command.hpp',
		'./method_invoke_command_expose.hpp',
		{ install = false }
	)
	add_files(
		'./dllmain.cpp',
		'./Source.def',
		'./windows/application.manifest',
		'./windows/application.rc',
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
		os.cp(target:targetfile(), m.root .. '/package/Assets/Library/implement.dll')
	end)
end)
