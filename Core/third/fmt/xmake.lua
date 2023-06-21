-- fmt
-- 10.0.0 a0b8a92e3d1532361c2f7feb63babc5c18d00ef2
-- https://github.com/fmtlib/fmt

target('third.fmt', function()
	set_group('source/third')
	set_kind('headeronly')
	add_headerfiles(
		'./include/fmt/args.h',
		'./include/fmt/chrono.h',
		'./include/fmt/color.h',
		'./include/fmt/compile.h',
		'./include/fmt/core.h',
		'./include/fmt/format-inl.h',
		'./include/fmt/format.h',
		'./include/fmt/os.h',
		'./include/fmt/ostream.h',
		'./include/fmt/printf.h',
		'./include/fmt/ranges.h',
		'./include/fmt/std.h',
		'./include/fmt/xchar.h',
		{ install = false }
	)
	add_defines(
		'FMT_HEADER_ONLY',
		{ public = true }
	)
end)
