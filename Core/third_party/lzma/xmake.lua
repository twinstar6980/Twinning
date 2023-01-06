-- lzma
-- 22.01
-- https://www.7-zip.org/sdk.html

-- TODO

target('lzma', function()
	set_group('source/third_party')
	set_kind('static')
	add_headerfiles(
	-- './7z.h',
	-- './7zAlloc.h',
	-- './7zBuf.h',
	-- './7zCrc.h',
	-- './7zFile.h',
	-- './7zTypes.h',
	-- './7zVersion.h',
	-- './Aes.h',
	-- './Alloc.h',
	-- './Bcj2.h',
	-- './Bra.h',
	-- './Compiler.h',
	-- './CpuArch.h',
	-- './Delta.h',
	-- './DllSecur.h',
	-- './LzFind.h',
	-- './LzFindMt.h',
	-- './LzHash.h',
	-- './Lzma2Dec.h',
	-- './Lzma2DecMt.h',
	-- './Lzma2Enc.h',
	-- './Lzma86.h',
	-- './LzmaDec.h',
	-- './LzmaEnc.h',
	-- './LzmaLib.h',
	-- './MtCoder.h',
	-- './MtDec.h',
	-- './Ppmd.h',
	-- './Ppmd7.h',
	-- './Precomp.h',
	-- './RotateDefs.h',
	-- './Sha256.h',
	-- './Sort.h',
	-- './Threads.h',
	-- './Xz.h',
	-- './XzCrc64.h',
	-- './XzEnc.h',
		'7zTypes.h',
		'LzFind.h',
		'LzHash.h',
		'Lzma2Dec.h',
		'Lzma2Enc.h',
		'LzmaDec.h',
		'LzmaEnc.h',
		'LzmaLib.h',
		'LzFindMt.h',
		'Lzma2DecMt.h',
		{ install = false }
	)
	add_files(
		'./Alloc.c',
		'./LzFind.c',
		'./LzFindOpt.c',
		'./Lzma2Dec.c',
		'./Lzma2Enc.c',
		'./LzmaDec.c',
		'./LzmaEnc.c',
		'./LzmaLib.c',
		'./CpuArch.c',

		'./LzFindMt.c',
		'./MtCoder.c',
		'./MtDec.c',
		'./Threads.c',
		'./DllSecur.c',
		'./Lzma2DecMt.c',

		-- './7zAlloc.c',
		-- './7zArcIn.c',
		-- './7zBuf.c',
		-- './7zBuf2.c',
		-- './7zCrc.c',
		-- './7zCrcOpt.c',
		-- './7zDec.c',
		-- './7zFile.c',
		-- './7zStream.c',
		-- './Aes.c',
		-- './AesOpt.c',
		-- './Alloc.c',
		-- './Bcj2.c',
		-- './Bcj2Enc.c',
		-- './Bra.c',
		-- './Bra86.c',
		-- './BraIA64.c',
		-- './CpuArch.c',
		-- './Delta.c',
		-- './DllSecur.c',
		-- './LzFind.c',
		-- './LzFindMt.c',
		-- './LzFindOpt.c',
		-- './Lzma2Dec.c',
		-- './Lzma2DecMt.c',
		-- './Lzma2Enc.c',
		-- './Lzma86Dec.c',
		-- './Lzma86Enc.c',
		-- './LzmaDec.c',
		-- './LzmaEnc.c',
		-- './LzmaLib.c',
		-- './MtCoder.c',
		-- './MtDec.c',
		-- './Ppmd7.c',
		-- './Ppmd7Dec.c',
		-- './Ppmd7Enc.c',
		-- './Sha256.c',
		-- './Sha256Opt.c',
		-- './Sort.c',
		-- './Threads.c',
		-- './Xz.c',
		-- './XzCrc64.c',
		-- './XzCrc64Opt.c',
		-- './XzDec.c',
		-- './XzEnc.c',
		-- './XzIn.c',
		{}
	)
	if m.system:is('windows') then
	end
	if m.system:is('linux', 'macos', 'android', 'ios') then
		add_defines(
			'_7ZIP_ST',
			{}
		)
	end
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
