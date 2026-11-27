target('dependency.lzma', function()
	set_group('source/dependency')
	set_kind('static')
	add_headerfiles(
		'origin/C/7zTypes.h',
		'origin/C/7zWindows.h',
		'origin/C/Alloc.h',
		'origin/C/Compiler.h',
		'origin/C/CpuArch.h',
		'origin/C/LzFind.h',
		'origin/C/LzHash.h',
		'origin/C/LzmaDec.h',
		'origin/C/LzmaEnc.h',
		'origin/C/LzmaLib.h',
		'origin/C/Precomp.h',
		{ install = false }
	)
	add_files(
		'origin/C/Alloc.c',
		'origin/C/CpuArch.c',
		'origin/C/LzFind.c',
		'origin/C/LzFindOpt.c',
		'origin/C/LzmaDec.c',
		'origin/C/LzmaEnc.c',
		'origin/C/LzmaLib.c',
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_defines(
		'Z7_ST',
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
