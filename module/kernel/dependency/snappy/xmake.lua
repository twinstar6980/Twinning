target('dependency.snappy', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/snappy.h',
		'origin/snappy-c.h',
		'origin/snappy-internal.h',
		'origin/snappy-sinksource.h',
		'origin/snappy-stubs-internal.h',
		'origin/snappy-stubs-public.h',
		{ install = false }
	)
	add_files(
		'origin/snappy.cc',
		'origin/snappy-c.cc',
		'origin/snappy-sinksource.cc',
		'origin/snappy-stubs-internal.cc',
		{}
	)
	add_includedirs(
		'origin',
		{ public = true }
	)
	add_defines(
		{ public = true }
	)
	add_defines(
		'HAVE_ATTRIBUTE_ALWAYS_INLINE=1',
		'HAVE_BUILTIN_CTZ=1',
		'HAVE_BUILTIN_EXPECT=1',
		'HAVE_BUILTIN_PREFETCH=1',
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
