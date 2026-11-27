target('dependency.libpng', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/png.h',
		'origin/pngconf.h',
		'origin/pngdebug.h',
		'origin/pnginfo.h',
		'origin/pnglibconf.h',
		'origin/pngpriv.h',
		'origin/pngstruct.h',
		{ install = false }
	)
	add_files(
		'origin/png.c',
		'origin/pngerror.c',
		'origin/pngget.c',
		'origin/pngmem.c',
		'origin/pngpread.c',
		'origin/pngread.c',
		'origin/pngrio.c',
		'origin/pngrtran.c',
		'origin/pngrutil.c',
		'origin/pngset.c',
		'origin/pngtrans.c',
		'origin/pngwio.c',
		'origin/pngwrite.c',
		'origin/pngwtran.c',
		'origin/pngwutil.c',
		{}
	)
	add_includedirs(
		'../zlib/origin',
		{ public = true }
	)
	add_defines(
		{ public = true }
	)
	add_deps(
		'dependency.zlib',
		{}
	)
	if list_contain(manifest.architecture, 'arm64') then
		add_defines(
			'PNG_ARM_NEON_OPT=0',
			{ public = true }
		)
	end
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_disable(target)
		return
	end)
end)
