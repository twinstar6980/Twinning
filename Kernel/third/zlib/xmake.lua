-- zlib
-- 1.3.1 51b7f2abdade71cd9bb0e7a373ef2610ec6f9daf
-- https://github.com/madler/zlib

target('third.zlib', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./crc32.h',
		'./deflate.h',
		'./gzguts.h',
		'./inffast.h',
		'./inffixed.h',
		'./inflate.h',
		'./inftrees.h',
		'./trees.h',
		'./zconf.h',
		'./zlib.h',
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
		'./infback.c',
		'./inffast.c',
		'./inflate.c',
		'./inftrees.c',
		'./trees.c',
		'./uncompr.c',
		'./zutil.c',
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_deps(
		{}
	)
	if m.system:is('linux', 'macintosh', 'android', 'iphone') then
		add_defines(
			'Z_HAVE_UNISTD_H',
			{ public = true }
		)
	end
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
