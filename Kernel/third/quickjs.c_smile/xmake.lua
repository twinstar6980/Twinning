-- quickjs.c_smile
-- 2021-03-27 121dca5c9c71763dacbf086563d81c09f727d84d
-- https://github.com/c-smile/quickjspp

target('third.quickjs', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./cutils.h',
		'./libbf.h',
		'./libregexp.h',
		'./libregexp-opcode.h',
		'./libunicode.h',
		'./libunicode-table.h',
		'./list.h',
		'./quickjs.h',
		'./quickjs-atom.h',
		'./quickjs-libc.h',
		'./quickjs-opcode.h',
		'./quickjs-version.h',
		'./unicode_gen_def.h',
		'./win/dirent.h',
		{ install = false }
	)
	add_files(
		'./cutils.c',
		'./libbf.c',
		'./libregexp.c',
		'./libunicode.c',
		'./quickjs.c',
		'./unicode_gen.c',
		{}
	)
	add_defines(
		'JS_STRICT_NAN_BOXING',
		'CONFIG_BIGNUM',
		{ public = true }
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
