-- quickjs
-- 2021-03-27 b5e62895c619d4ffc75c9d822c8d85f1ece77e5b
-- https://github.com/bellard/quickjs

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
		'./unicode_gen_def.h',
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
		'CONFIG_VERSION="2021-03-27"',
		'CONFIG_BIGNUM',
		{ public = true }
	)
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
