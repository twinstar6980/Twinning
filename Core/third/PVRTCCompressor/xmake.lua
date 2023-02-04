-- PVRTCCompressor
-- ? 234ecb7db4785bd3c971d01d7f5556192fe9ab28
-- https://github.com/brenwill/PVRTCCompressor

target('third.PVRTCCompressor', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./AlphaBitmap.h',
		'./Bitmap.h',
		'./BitScale.h',
		'./BitUtility.h',
		'./ColorRgba.h',
		'./Interval.h',
		'./MortonTable.h',
		'./Point2.h',
		'./PvrTcDecoder.h',
		'./PvrTcEncoder.h',
		'./PvrTcPacket.h',
		'./RgbaBitmap.h',
		'./RgbBitmap.h',
		{ install = false }
	)
	add_files(
		'./BitScale.cpp',
		'./MortonTable.cpp',
		'./PvrTcDecoder.cpp',
		'./PvrTcEncoder.cpp',
		'./PvrTcPacket.cpp',
		'./main.cpp',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
