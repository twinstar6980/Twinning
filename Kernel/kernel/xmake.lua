-- kernel

target('kernel', function()
	set_group('source')
	set_kind('shared')
	add_headerfiles(
		'**.hpp',
		{ install = false }
	)
	add_files(
		'**.cppm',
		'**.cpp',
		manifest.root .. '/../common/cpp/std.cppm',
		{}
	)
	add_includedirs(
		manifest.root .. '',
		{}
	)
	add_defines(
		{ public = true }
	)
	add_deps(
		'dependency.mscharconv',
		'dependency.tinyxml2',
		'dependency.cpp_md5_lib',
		'dependency.Rijndael',
		'dependency.zlib',
		'dependency.bzip2',
		'dependency.lzma',
		'dependency.open_vcdiff',
		'dependency.avir',
		'dependency.libsquish',
		'dependency.PVRTCCompressor',
		'dependency.etcpak',
		'dependency.astc_encoder',
		'dependency.libpng',
		'dependency.quickjs_ng',
		{}
	)
	on_load(function(target)
		import('common.xmake.utility')
		utility.apply_condition_definition_basic(target)
		utility.apply_compiler_option_basic(target)
		utility.apply_compiler_option_warning_regular(target)
		return
	end)
end)
