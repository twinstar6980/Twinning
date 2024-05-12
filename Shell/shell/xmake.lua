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
		'./utility/library.hpp',
		'./utility/interaction.hpp',
		'./utility/miscellaneous.hpp',
		'./bridge/data.hpp',
		'./bridge/proxy.hpp',
		'./bridge/service.hpp',
		'./bridge/library.hpp',
		'./bridge/client.hpp',
		'./bridge/launcher.hpp',
		'./main_console_bridge_client.hpp',
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
			'Ole32',
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
