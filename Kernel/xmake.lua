set_project('Kernel')

add_rules('mode.debug', 'mode.release')

add_moduledirs('./../common/xmake')
includes('./../common/xmake/helper.lua')
apply_common_setting()

includes('./third/mscharconv')
includes('./third/tinyxml2')
includes('./third/cpp_md5_lib')
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
