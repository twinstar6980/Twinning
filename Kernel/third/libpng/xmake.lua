-- libpng
-- 1.6.47 872555f4ba910252783af1507f9e7fe1653be252
-- https://github.com/pnggroup/libpng

-- TODO : apply special processor setting, see https://github.com/xmake-io/xmake-repo/blob/master/packages/l/libpng/xmake.lua

target('third.libpng', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./png.h',
		'./pngconf.h',
		'./pngdebug.h',
		'./pnginfo.h',
		'./pnglibconf.h',
		'./pngpriv.h',
		'./pngstruct.h',
		{ install = false }
	)
	add_files(
		'./png.c',
		'./pngerror.c',
		'./pngget.c',
		'./pngmem.c',
		'./pngpread.c',
		'./pngread.c',
		'./pngrio.c',
		'./pngrtran.c',
		'./pngrutil.c',
		'./pngset.c',
		'./pngtrans.c',
		'./pngwio.c',
		'./pngwrite.c',
		'./pngwtran.c',
		'./pngwutil.c',
		{}
	)
	add_includedirs(
		'./../zlib',
		{ public = true }
	)
	add_deps(
		'third.zlib',
		{}
	)
	if m.architecture:is('arm_32', 'arm_64') then
		add_defines(
			'PNG_ARM_NEON_OPT=0',
			{ public = true }
		)
	end
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
