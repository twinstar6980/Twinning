-- kernel

target('kernel', function()
	set_group('source')
	set_kind('shared')
	add_headerfiles(
		'./**.hpp',
		{ install = false }
	)
	add_files(
		'./**.cppm',
		'./**.cpp',
		m.root .. '/../common/cpp/std.cppm',
		{}
	)
	add_includedirs(
		m.root .. '',
		{}
	)
	add_deps(
		'third.mscharconv',
		'third.fmt',
		'third.tinyxml2',
		'third.md5',
		'third.Rijndael',
		'third.zlib',
		'third.bzip2',
		'third.lzma',
		'third.open_vcdiff',
		'third.avir',
		'third.etcpak',
		'third.PVRTCCompressor',
		'third.libpng',
		'third.quickjs_ng',
		{}
	)
	on_load(function(target)
		import('helper')
		helper.apply_condition_definition_basic(target)
		helper.apply_compiler_option_basic(target)
		helper.apply_compiler_option_warning_regular(target)
		helper.import_vld_if_needed(target)
	end)
end)
