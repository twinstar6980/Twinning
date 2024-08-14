-- lzma
-- 24.08 ?
-- https://www.7-zip.org/sdk.html

target('third.lzma', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'./7zTypes.h',
		'./7zWindows.h',
		'./Alloc.h',
		'./Compiler.h',
		'./CpuArch.h',
		'./LzFind.h',
		'./LzFindMt.h',
		'./LzHash.h',
		'./LzmaDec.h',
		'./LzmaEnc.h',
		'./LzmaLib.h',
		'./Precomp.h',
		'./Threads.h',
		{ install = false }
	)
	add_files(
		'./Alloc.c',
		'./CpuArch.c',
		'./LzFind.c',
		'./LzFindMt.c',
		'./LzFindOpt.c',
		'./LzmaDec.c',
		'./LzmaEnc.c',
		'./LzmaLib.c',
		'./Threads.c',
		{}
	)
	on_load(function(target)
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
