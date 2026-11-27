target('dependency.quickjs_ng', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/builtin-array-fromasync.h',
		'origin/builtin-iterator-zip.h',
		'origin/builtin-iterator-zip-keyed.h',
		'origin/cutils.h',
		'origin/dtoa.h',
		'origin/libregexp-opcode.h',
		'origin/libregexp.h',
		'origin/libunicode-table.h',
		'origin/libunicode.h',
		'origin/list.h',
		'origin/quickjs-atom.h',
		'origin/quickjs-c-atomics.h',
		'origin/quickjs-opcode.h',
		'origin/quickjs.h',
		{ install = false }
	)
	add_files(
		'origin/dtoa.c',
		'origin/libregexp.c',
		'origin/libunicode.c',
		'origin/quickjs.c',
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
