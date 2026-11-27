target('dependency.bzip2', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/bzlib.h',
		'origin/bzlib_private.h',
		{ install = false }
	)
	add_files(
		'origin/blocksort.c',
		'origin/huffman.c',
		'origin/crctable.c',
		'origin/randtable.c',
		'origin/compress.c',
		'origin/decompress.c',
		'origin/bzlib.c',
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
