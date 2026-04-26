-- quickjs_ng
-- 0.14.0 3c051980ab7e783dfbfb1c70c014ce5e05ecf24c
-- https://github.com/quickjs-ng/quickjs

target('third.quickjs_ng', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'builtin-array-fromasync.h',
		'builtin-iterator-zip.h',
		'builtin-iterator-zip-keyed.h',
		'cutils.h',
		'dtoa.h',
		'libregexp-opcode.h',
		'libregexp.h',
		'libunicode-table.h',
		'libunicode.h',
		'list.h',
		'quickjs-atom.h',
		'quickjs-c-atomics.h',
		'quickjs-libc.h',
		'quickjs-opcode.h',
		'quickjs.h',
		'unicode_gen_def.h',
		{ install = false }
	)
	add_files(
		'dtoa.c',
		'libregexp.c',
		'libunicode.c',
		'quickjs-libc.c',
		'quickjs.c',
		'unicode_gen.c',
		{}
	)
	add_includedirs(
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
