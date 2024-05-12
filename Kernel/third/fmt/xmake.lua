-- fmt
-- 10.2.1 e69e5f977d458f2650bb346dadf2ad30c5320281
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
