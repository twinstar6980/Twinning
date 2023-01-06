set_project('TwinStar.ToolKit.Shell')
set_version('16')

add_rules('mode.debug', 'mode.release')

add_moduledirs('../common/xmake')
includes('../common/xmake/custom.lua')
apply_common_setting()

includes('./shell')
