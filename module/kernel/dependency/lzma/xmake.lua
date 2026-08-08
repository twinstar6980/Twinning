-- lzma
-- 26.02 f9d78aff31a5f2521ae7ddbdc97c4a8855808959
-- https://github.com/ip7z/7zip

target('dependency.lzma', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'7zTypes.h',
		'7zWindows.h',
		'Alloc.h',
		'Compiler.h',
		'CpuArch.h',
		'LzFind.h',
		'LzFindMt.h',
		'LzHash.h',
		'LzmaDec.h',
		'LzmaEnc.h',
		'LzmaLib.h',
		'Precomp.h',
		'Threads.h',
		{ install = false }
	)
	add_files(
		'Alloc.c',
		'CpuArch.c',
		'LzFind.c',
		'LzFindMt.c',
		'LzFindOpt.c',
		'LzmaDec.c',
		'LzmaEnc.c',
		'LzmaLib.c',
		'Threads.c',
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
