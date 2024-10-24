-- quickjs_ng
-- 0.6.1 2b953a72bed53ad2a4fa61fb9cb10523fcb93dcf
-- https://github.com/quickjs-ng/quickjs

target('third.quickjs_ng', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./cutils.h',
		'./dirent_compat.h',
		'./getopt_compat.h',
		'./libbf.h',
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
		{ install = false }
	)
	add_files(
		'./cutils.c',
		'./libbf.c',
		'./libregexp.c',
		'./libunicode.c',
		'./qjs.c',
		'./qjsc.c',
		'./quickjs-libc.c',
		'./quickjs.c',
		'./unicode_gen.c',
		{}
	)
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
