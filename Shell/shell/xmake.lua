-- shell

target('shell', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'./core_interface.hpp',
		'./version.hpp',
		'./third_party/system/windows.hpp',
		'./third_party/system/posix.hpp',
		'./common.hpp',
		'./library/library.hpp',
		'./library/static_library.hpp',
		'./library/dynamic_library.hpp',
		'./host/host.hpp',
		'./host/cli_host.hpp',
		'./launch.hpp',
		{ install = false }
	)
	add_files(
		'./main.cpp',
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
	if m.system:is('windows') then
		add_files(
			'./windows/application.manifest',
			'./windows/application.rc',
			{}
		)
		add_links(
			'Ole32',
			{ private = true }
		)
	end
	on_load(function(target)
		import('custom')
		custom.apply_condition_definition_basic(target)
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_regular(target)
	end)
end)
