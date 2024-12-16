-- etcpak
-- 2.0 a43d6925bee49277945cf3e311e4a022ae0c2073
-- https://github.com/wolfpld/etcpak

target('third.etcpak', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./getopt/getopt.h',
		'./lz4/lz4.h',
		'./Bitmap.hpp',
		'./BitmapDownsampled.hpp',
		'./BlockData.hpp',
		'./ColorSpace.hpp',
		'./DataProvider.hpp',
		'./Debug.hpp',
		'./Dither.hpp',
		'./Error.hpp',
		'./ForceInline.hpp',
		'./Math.hpp',
		'./MipMap.hpp',
		'./ProcessCommon.hpp',
		'./ProcessDxtc.hpp',
		'./ProcessRGB.hpp',
		'./Semaphore.hpp',
		'./System.hpp',
		'./Tables.hpp',
		'./TaskDispatch.hpp',
		'./Timing.hpp',
		'./Vector.hpp',
		'./bc7enc.h',
		'./bcdec.h',
		'./mmap.hpp',
		{ install = false }
	)
	add_files(
		'./getopt/getopt.c',
		'./lz4/lz4.c',
		'./Application.cpp',
		'./Bitmap.cpp',
		'./BitmapDownsampled.cpp',
		'./BlockData.cpp',
		'./ColorSpace.cpp',
		'./DataProvider.cpp',
		'./Debug.cpp',
		'./Dither.cpp',
		'./Error.cpp',
		'./ProcessDxtc.cpp',
		'./ProcessRGB.cpp',
		'./System.cpp',
		'./Tables.cpp',
		'./TaskDispatch.cpp',
		'./Timing.cpp',
		'./bc7enc.cpp',
		'./bcdec.c',
		'./mmap.cpp',
		{}
	)
	add_includedirs(
		'./..',
		{ public = true }
	)
	add_deps(
		'third.libpng',
		{}
	)
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
