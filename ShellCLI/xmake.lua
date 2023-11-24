set_project('ShellCLI')

add_rules('mode.debug', 'mode.release')

add_moduledirs('../common/xmake')
includes('../common/xmake/custom.lua')
apply_common_setting()

if m.system:is('windows', 'linux', 'macintosh') then
	includes('./third/tinyfiledialogs')
end

includes('./shell_cli')
