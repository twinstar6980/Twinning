set_project('Kernel')

add_rules('mode.debug', 'mode.release')

add_moduledirs('./../common/xmake')
includes('./../common/xmake/helper.lua')
apply_common_setting()

includes('./third/mscharconv')
includes('./third/fmt')
includes('./third/tinyxml2')
includes('./third/md5')
includes('./third/Rijndael')
includes('./third/zlib')
includes('./third/bzip2')
includes('./third/lzma')
includes('./third/open_vcdiff')
includes('./third/avir')
includes('./third/etcpak')
includes('./third/PVRTCCompressor')
includes('./third/libpng')
includes('./third/quickjs_ng')

includes('./kernel')

includes('./test')
if m.system:is('windows', 'linux', 'macintosh') then
	includes('./../Shell/third/tinyfiledialogs')
end
