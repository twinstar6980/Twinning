-- fmt
-- 11.0.2 0c9fce2ffefecfdce794e1859584e25877b7b592
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
