target('dependency.zlib', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/zlib.h',
		'origin/crc32.h',
		'origin/deflate.h',
		'origin/gzguts.h',
		'origin/inffast.h',
		'origin/inffixed.h',
		'origin/inflate.h',
		'origin/inftrees.h',
		'origin/trees.h',
		'origin/zutil.h',
		'origin/zconf.h',
		{ install = false }
	)
	add_files(
		'origin/adler32.c',
		'origin/compress.c',
		'origin/crc32.c',
		'origin/deflate.c',
		'origin/gzclose.c',
		'origin/gzlib.c',
		'origin/gzread.c',
		'origin/gzwrite.c',
		'origin/inflate.c',
		'origin/infback.c',
		'origin/inftrees.c',
		'origin/inffast.c',
		'origin/trees.c',
		'origin/uncompr.c',
		'origin/zutil.c',
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_defines(
		'ZLIB_CONST',
		{ public = true }
	)
	add_deps(
		{}
	)
	if list_contain(manifest.system, 'linux', 'macintosh', 'android', 'iphone') then
		add_defines(
			'Z_HAVE_UNISTD_H',
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
