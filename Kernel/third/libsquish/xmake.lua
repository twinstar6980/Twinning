-- libsquish
-- 1.15.1.4 4f48f6752814e3e62e75087fc09b816ac0e1f0f0
-- https://github.com/oblivioncth/libsquish

target('third.libsquish', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'lib/src/alpha.h',
		'lib/src/clusterfit.h',
		'lib/src/colourblock.h',
		'lib/src/colourfit.h',
		'lib/src/colourset.h',
		'lib/src/config.h',
		'lib/src/maths.h',
		'lib/src/rangefit.h',
		'lib/src/simd.h',
		'lib/src/simd_float.h',
		'lib/src/simd_sse.h',
		'lib/src/simd_ve.h',
		'lib/src/singlecolourfit.h',
		'lib/src/singlecolourlookup.inl',
		'lib/include/squish/squish.h',
		{ install = false }
	)
	add_files(
		'lib/src/alpha.cpp',
		'lib/src/clusterfit.cpp',
		'lib/src/colourblock.cpp',
		'lib/src/colourfit.cpp',
		'lib/src/colourset.cpp',
		'lib/src/maths.cpp',
		'lib/src/rangefit.cpp',
		'lib/src/singlecolourfit.cpp',
		'lib/src/squish.cpp',
		{}
	)
	add_includedirs(
		'lib/include',
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
