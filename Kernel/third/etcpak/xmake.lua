-- etcpak
-- 2.1 7f15da4739cc5f8562311bdbb800436986d268ca
-- https://github.com/wolfpld/etcpak

target('third.etcpak', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'getopt/getopt.h',
		'Bitmap.hpp',
		'BitmapDownsampled.hpp',
		'BlockData.hpp',
		'ColorSpace.hpp',
		'DataProvider.hpp',
		'Debug.hpp',
		'Decode.hpp',
		'Dither.hpp',
		'Error.hpp',
		'ForceInline.hpp',
		'Math.hpp',
		'MipMap.hpp',
		'ProcessCommon.hpp',
		'ProcessDxtc.hpp',
		'ProcessRGB.hpp',
		'Semaphore.hpp',
		'System.hpp',
		'Tables.hpp',
		'TaskDispatch.hpp',
		'TextureHeader.hpp',
		'Timing.hpp',
		'Vector.hpp',
		'bc7enc.h',
		'bcdec.h',
		'mmap.hpp',
		{ install = false }
	)
	add_files(
		'getopt/getopt.c',
		'Application.cpp',
		'Bitmap.cpp',
		'BitmapDownsampled.cpp',
		'BlockData.cpp',
		'ColorSpace.cpp',
		'DataProvider.cpp',
		'Debug.cpp',
		'Decode.cpp',
		'Dither.cpp',
		'Error.cpp',
		'ProcessDxtc.cpp',
		'ProcessRGB.cpp',
		'System.cpp',
		'Tables.cpp',
		'TaskDispatch.cpp',
		'TextureHeader.cpp',
		'Timing.cpp',
		'bc7enc.cpp',
		'bcdec.c',
		'mmap.cpp',
		{}
	)
	add_includedirs(
		'../libpng',
		{ public = true }
	)
	add_deps(
		'third.libpng',
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
		return
	end)
end)
