target('dependency.lz4', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/lib/lz4.h',
		'origin/lib/lz4hc.h',
		'origin/lib/lz4frame.h',
		'origin/lib/lz4frame_static.h',
		'origin/lib/lz4file.h',
		'origin/lib/xxhash.h',
		{ install = false }
	)
	add_files(
		'origin/lib/lz4.c',
		'origin/lib/lz4hc.c',
		'origin/lib/lz4frame.c',
		'origin/lib/lz4file.c',
		'origin/lib/xxhash.c',
		{}
	)
	add_includedirs(
		'origin/lib',
		{ public = true }
	)
	add_defines(
		{ public = true }
	)
	add_defines(
		'XXH_NAMESPACE=LZ4_',
		{ public = false }
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
