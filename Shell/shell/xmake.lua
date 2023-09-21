-- shell

target('shell', function()
	set_group('source')
	set_kind('binary')
	add_headerfiles(
		'./common.hpp',
		'./third/system/windows.hpp',
		'./third/system/posix.hpp',
		'./third/tinyfiledialogs.hpp',
		'./utility/macro.hpp',
		'./utility/exception.hpp',
		'./utility/string.hpp',
		'./utility/function.hpp',
		'./utility/library.hpp',
		'./utility/interaction.hpp',
		'./utility/miscellaneous.hpp',
		'./utility/system/windows.hpp',
		'./bridge/interface.hpp',
		'./bridge/symbol.hpp',
		'./bridge/converter.hpp',
		'./bridge/library.hpp',
		'./bridge/invoker.hpp',
		'./bridge/static_library.hpp',
		'./bridge/dynamic_library.hpp',
		'./bridge/host.hpp',
		'./bridge/launcher.hpp',
		'./bridge/cli_host.hpp',
		{ install = false }
	)
	add_files(
		'./main.cpp',
		{}
	)
	add_includedirs(
		m.root .. '',
		m.root .. '/../Kernel',
		{ private = true }
	)
	if m.system:is('windows', 'linux', 'macintosh') then
		add_deps(
			'third.tinyfiledialogs',
			{}
		)
	end
	if m.system:is('windows') then
		add_files(
			'./resource/windows/application.manifest',
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
end)
