set_project('TwinStar.ToolKit.Core')

add_rules('mode.debug', 'mode.release')

add_moduledirs('../common/xmake')
includes('../common/xmake/custom.lua')
apply_common_setting()

includes('./third_party/mscharconv')
includes('./third_party/fmt')
includes('./third_party/tinyxml2')
includes('./third_party/md5')
includes('./third_party/Rijndael')
includes('./third_party/zlib')
includes('./third_party/bzip2')
includes('./third_party/lzma')
includes('./third_party/libpng')
includes('./third_party/ETCPACK')
includes('./third_party/rg_etc1')
includes('./third_party/PVRTCCompressor')
if m.system:is('linux', 'macintosh', 'android', 'iphone') then
	includes('./third_party/quickjs')
end
if m.system:is('windows') then
	includes('./third_party/quickjs.c_smile')
end

includes('./core')

includes('./test')
