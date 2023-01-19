-- libpng
-- 1.6.37
-- http://www.libpng.org/pub/png/libpng.html

-- TODO : apply special processor setting, see https://github.com/xmake-io/xmake-repo/blob/master/packages/l/libpng/xmake.lua

target('third.libpng', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./png.h',
		'./pngconf.h',
		'./pnglibconf.h',
		'./pngpriv.h',
		'./pngdebug.h',
		'./pnginfo.h',
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
		'../zlib',
		{ public = true }
	)
	add_deps(
		'third.zlib',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
