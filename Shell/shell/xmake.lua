-- shell

target('shell', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'./common.hpp',
		'./third/system/windows.hpp',
		'./third/system/posix.hpp',
		'./utility/macro.hpp',
		'./utility/exception.hpp',
		'./utility/string.hpp',
		'./utility/function.hpp',
		'./utility/library.hpp',
		'./utility/interaction.hpp',
		'./utility/miscellaneous.hpp',
		'./core/interface.hpp',
		'./core/symbol.hpp',
		'./core/converter.hpp',
		'./core/library.hpp',
		'./core/invoker.hpp',
		'./core/static_library.hpp',
		'./core/dynamic_library.hpp',
		'./host/host.hpp',
		'./host/launcher.hpp',
		'./host/cli_host.hpp',
		{ install = false }
	)
	add_files(
		'./main.cpp',
		{}
	)
	add_includedirs(
		m.root .. '',
		m.root .. '/../Core',
		{ private = true }
	)
	add_defines(
		'_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING',
		{ private = true }
	)
	if m.system:is('windows') then
		add_files(
			'./resource/windows/application.manifest',
			{}
		)
	end
	on_load(function(target)
		import('custom')
		custom.apply_condition_definition_basic(target)
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_regular(target)
		custom.apply_import_vld_if_can(target)
	end)
end)
