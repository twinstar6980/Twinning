-- libpng
-- 1.6.51 49363adcfaf098748d7a4c8c624ad8c45a8c3a86
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
	if m.architecture:is('arm64') then
		add_defines(
			'PNG_ARM_NEON_OPT=0',
			{ public = true }
		)
	end
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
	end)
end)
