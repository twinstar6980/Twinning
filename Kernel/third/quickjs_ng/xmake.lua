-- quickjs_ng
-- 0.10.1 3c9afc9943323ee9c7dbd123c0cd991448f4b6c2
-- https://github.com/quickjs-ng/quickjs

target('third.quickjs_ng', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./builtin-array-fromasync.h',
		'./cutils.h',
		'./dirent_compat.h',
		'./libregexp-opcode.h',
		'./libregexp.h',
		'./libunicode-table.h',
		'./libunicode.h',
		'./list.h',
		'./quickjs-atom.h',
		'./quickjs-c-atomics.h',
		'./quickjs-libc.h',
		'./quickjs-opcode.h',
		'./quickjs.h',
		'./unicode_gen_def.h',
		'./xsum.h',
		{ install = false }
	)
	add_files(
		'./cutils.c',
		'./libregexp.c',
		'./libunicode.c',
		'./quickjs-libc.c',
		'./quickjs.c',
		'./unicode_gen.c',
		'./xsum.c',
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
	end)
end)
