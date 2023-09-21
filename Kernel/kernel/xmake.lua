-- kernel

target('kernel', function()
	set_group('source')
	set_kind('shared')
	add_headerfiles(
		'./**.hpp',
		{ install = false }
	)
	add_files(
		'./**.cpp',
		{}
	)
	add_includedirs(
		m.root .. '',
		{ private = true }
	)
	add_deps(
		'third.mscharconv',
		'third.fmt',
		'third.tinyxml2',
		'third.quickjs',
		'third.md5',
		'third.Rijndael',
		'third.zlib',
		'third.bzip2',
		'third.lzma',
		'third.open_vcdiff',
		'third.libpng',
		'third.ETCPACK',
		'third.etcpak',
		'third.PVRTCCompressor',
		{}
	)
	on_load(function(target)
		import('custom')
		custom.apply_condition_definition_basic(target)
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_regular(target)
		custom.apply_import_vld_if_can(target)
	end)
end)
