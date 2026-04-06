add_moduledirs('..')
includes('../common/xmake/utility.lua')

define_project('Kernel', '119')

includes('third/mscharconv')
includes('third/tinyxml2')
includes('third/cpp_md5_lib')
includes('third/Rijndael')
includes('third/zlib')
includes('third/bzip2')
includes('third/lzma')
includes('third/open_vcdiff')
includes('third/avir')
includes('third/etcpak')
includes('third/PVRTCCompressor')
includes('third/libpng')
includes('third/quickjs_ng')

includes('kernel')

includes('test')
