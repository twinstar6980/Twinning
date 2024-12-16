-- lzma
-- 24.09 e5431fa6f5505e385c6f9367260717e9c47dc2ee
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
		import('helper')
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_disable(target)
	end)
end)
