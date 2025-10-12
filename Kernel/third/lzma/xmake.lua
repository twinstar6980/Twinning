-- lzma
-- 25.01 5e96a8279489832924056b1fa82f29d5837c9469
-- https://github.com/ip7z/7zip

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
