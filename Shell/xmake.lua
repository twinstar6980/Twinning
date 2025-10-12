set_project('Shell')

add_rules('mode.debug', 'mode.release')

add_moduledirs('./..')
includes('./../common/xmake/utility.lua')
apply_common_setting()

includes('./shell')
