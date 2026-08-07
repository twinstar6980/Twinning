add_moduledirs('..')
includes('../common/xmake/utility.lua')

define_project('Kernel', '149')

includes('dependency/mscharconv')
includes('dependency/tinyxml2')
includes('dependency/cpp_md5_lib')
includes('dependency/Rijndael')
includes('dependency/zlib')
includes('dependency/bzip2')
includes('dependency/lzma')
includes('dependency/open_vcdiff')
includes('dependency/avir')
includes('dependency/libsquish')
includes('dependency/PVRTCCompressor')
includes('dependency/etcpak')
includes('dependency/astc_encoder')
includes('dependency/libpng')
includes('dependency/quickjs_ng')

includes('kernel')

includes('runner')
