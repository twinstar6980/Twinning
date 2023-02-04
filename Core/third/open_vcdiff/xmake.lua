-- open_vcdiff
-- 0.8.4 9af10d36e691c15dceff04419b9e3a71ec5d8bec
-- https://github.com/google/open-vcdiff

target('third.open_vcdiff', function()
	set_group('source/third')
	set_kind('static')
	set_languages('cxx14', { private = true })
	add_headerfiles(
		'./google/format_extension_flags.h',
		'./google/output_string.h',
		'./addrcache.h',
		'./blockhash.h',
		'./checksum.h',
		'./codetable.h',
		'./codetablewriter_interface.h',
		'./compile_assert.h',
		'./config.h',
		'./decodetable.h',
		'./encodetable.h',
		'./headerparser.h',
		'./instruction_map.h',
		'./jsonwriter.h',
		'./logging.h',
		'./rolling_hash.h',
		'./unique_ptr.h',
		'./varint_bigendian.h',
		'./vcdiff_defs.h',
		'./vcdiffengine.h',
		{ install = false }
	)
	add_files(
		'./addrcache.cc',
		'./blockhash.cc',
		'./codetable.cc',
		'./decodetable.cc',
		'./encodetable.cc',
		'./headerparser.cc',
		'./instruction_map.cc',
		'./jsonwriter.cc',
		'./logging.cc',
		'./varint_bigendian.cc',
		'./vcdecoder.cc',
		'./vcdiffengine.cc',
		'./vcencoder.cc',
		{}
	)
	add_includedirs(
		'.',
		'../zlib',
		{ public = true }
	)
	on_load(function(target)
		import('custom')
		custom.apply_compiler_option_basic(target)
		custom.apply_compiler_option_warning_disable(target)
	end)
end)
