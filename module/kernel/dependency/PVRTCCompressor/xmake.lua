target('dependency.PVRTCCompressor', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/AlphaBitmap.h',
		'origin/Bitmap.h',
		'origin/BitScale.h',
		'origin/BitUtility.h',
		'origin/ColorRgba.h',
		'origin/Interval.h',
		'origin/MortonTable.h',
		'origin/Point2.h',
		'origin/PvrTcDecoder.h',
		'origin/PvrTcEncoder.h',
		'origin/PvrTcPacket.h',
		'origin/RgbaBitmap.h',
		'origin/RgbBitmap.h',
		{ install = false }
	)
	add_files(
		'origin/BitScale.cpp',
		'origin/main.cpp',
		'origin/MortonTable.cpp',
		'origin/PvrTcDecoder.cpp',
		'origin/PvrTcEncoder.cpp',
		'origin/PvrTcPacket.cpp',
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_defines(
		{ public = true }
	)
	add_deps(
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
		return
	end)
end)
