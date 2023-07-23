-- zlib
-- 1.2.13 04f42ceca40f73e2978b50e93806c2a18c1281fc
-- https://github.com/madler/zlib

target('third.zlib', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./zconf.h',
		'./zlib.h',
		'./crc32.h',
		'./deflate.h',
		'./gzguts.h',
		'./inffast.h',
		'./inffixed.h',
		'./inflate.h',
		'./inftrees.h',
		'./trees.h',
		'./zutil.h',
		{ install = false }
	)
	add_files(
		'./adler32.c',
		'./compress.c',
		'./crc32.c',
		'./deflate.c',
		'./gzclose.c',
		'./gzlib.c',
		'./gzread.c',
		'./gzwrite.c',
		'./inflate.c',
		'./infback.c',
		'./inftrees.c',
		'./inffast.c',
		'./trees.c',
		'./uncompr.c',
		'./zutil.c',
		{}
	)
	if m.system:is('linux', 'macintosh', 'android', 'iphone') then
		add_defines(
			'Z_HAVE_UNISTD_H',
			'_LARGEFILE64_SOURCE=1',
			{ public = true }
		)
	end
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
