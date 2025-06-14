-- fmt
-- 11.2.0 40626af88bd7df9a5fb80be7b25ac85b122d6c21
-- https://github.com/fmtlib/fmt

target('third.fmt', function()
	set_group('source/third')
	set_kind('headeronly')
	add_headerfiles(
		'./include/fmt/args.h',
		'./include/fmt/base.h',
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
	add_files(
		{}
	)
	add_includedirs(
		{ public = true }
	)
	add_deps(
		{}
	)
	add_defines(
		'FMT_HEADER_ONLY',
		{ public = true }
	)
end)
