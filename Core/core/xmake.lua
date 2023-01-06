-- core

target('core', function()
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
	add_defines(
		'_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING',
		{ private = true }
	)
	add_deps(
		'mscharconv',
		'fmt',
		'tinyxml2',
		'quickjs',
		'md5',
		'Rijndael',
		'zlib',
		'bzip2',
		'lzma',
		'libpng',
		'ETCPACK',
		'rg_etc1',
		'PVRTCCompressor',
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
