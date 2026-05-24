-- astc_encoder
-- 5.4.0 3ee54d103ca66a5b1f751ad49a4b8064235e2cf9
-- https://github.com/ARM-software/astc-encoder

target('third.astc_encoder', function()
	set_group('source/third')
	set_kind('static')
	add_headerfiles(
		'Source/astcenc.h',
		'Source/astcenc_diagnostic_trace.h',
		'Source/astcenc_internal.h',
		'Source/astcenc_internal_entry.h',
		'Source/astcenc_mathlib.h',
		'Source/astcenc_vecmathlib.h',
		'Source/astcenc_vecmathlib_avx2_8.h',
		'Source/astcenc_vecmathlib_common_4.h',
		'Source/astcenc_vecmathlib_neon_4.h',
		'Source/astcenc_vecmathlib_none_4.h',
		'Source/astcenc_vecmathlib_rvv_n.h',
		'Source/astcenc_vecmathlib_sse_4.h',
		'Source/astcenc_vecmathlib_sve_8.h',
		{ install = false }
	)
	add_files(
		'Source/astcenc_averages_and_directions.cpp',
		'Source/astcenc_block_sizes.cpp',
		'Source/astcenc_color_quantize.cpp',
		'Source/astcenc_color_unquantize.cpp',
		'Source/astcenc_compress_symbolic.cpp',
		'Source/astcenc_compute_variance.cpp',
		'Source/astcenc_decompress_symbolic.cpp',
		'Source/astcenc_diagnostic_trace.cpp',
		'Source/astcenc_entry.cpp',
		'Source/astcenc_find_best_partitioning.cpp',
		'Source/astcenc_ideal_endpoints_and_weights.cpp',
		'Source/astcenc_image.cpp',
		'Source/astcenc_integer_sequence.cpp',
		'Source/astcenc_mathlib.cpp',
		'Source/astcenc_mathlib_softfloat.cpp',
		'Source/astcenc_partition_tables.cpp',
		'Source/astcenc_percentile_tables.cpp',
		'Source/astcenc_pick_best_endpoint_format.cpp',
		'Source/astcenc_quantization.cpp',
		'Source/astcenc_symbolic_physical.cpp',
		'Source/astcenc_weight_align.cpp',
		'Source/astcenc_weight_quant_xfer_tables.cpp',
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
