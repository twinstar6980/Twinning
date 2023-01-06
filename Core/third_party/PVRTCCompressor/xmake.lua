-- PVRTCCompressor
-- ?
-- https://github.com/brenwill/PVRTCCompressor

target('PVRTCCompressor', function()
	set_group('source/third_party')
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
