set_project('Shell')

add_rules('mode.debug', 'mode.release')

add_moduledirs('./../common/xmake')
includes('./../common/xmake/helper.lua')
apply_common_setting()

if m.system:is('windows', 'linux', 'macintosh') then
	includes('./third/tinyfiledialogs')
end

includes('./shell')
