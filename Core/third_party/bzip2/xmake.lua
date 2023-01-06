-- bzip2
-- 1.0.8
-- https://sourceware.org/bzip2/

target('bzip2', function()
	set_group('source/third_party')
	set_kind('static')
	add_headerfiles(
		'./bzlib.h',
		'./bzlib_private.h',
		{ install = false }
	)
	add_files(
		'./blocksort.c',
		'./huffman.c',
		'./crctable.c',
		'./randtable.c',
		'./compress.c',
		'./decompress.c',
		'./bzlib.c',
		'./bzip2.c',
		'./bzip2recover.c',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
