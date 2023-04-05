#pragma once

#include "core/utility/container/optional/optional.hpp"
#include "core/utility/container/list/list.hpp"
#include "core/utility/string/string.hpp"

namespace TwinStar::Core {

	#pragma region type

	class Path {

	protected:

		Optional<String> m_root;
		Boolean          m_absolute;
		List<String>     m_sub_path;

	public:

		#pragma region structor

		~Path (
		) = default;

		// ----------------

		Path (
		) = default;

		Path (
			Path const & that
		) = default;

		Path (
			Path && that
		) = default;

		// ----------------

		explicit Path (
			String const & path
		) :
			Path{} {
			thiz.from_string(path);
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			Path const & that
		) -> Path& = default;

		auto operator = (
			Path && that
		) -> Path& = default;

		// ----------------

		auto operator / (
			String const & that
		) const -> Path {
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_sub_path.allocate(thiz.m_sub_path.size() + 1_sz);
			result.m_sub_path.append_list(thiz.m_sub_path);
			result.m_sub_path.append(that);
			return result;
		}

		auto operator / (
			String && that
		) const -> Path {
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_sub_path.allocate(thiz.m_sub_path.size() + 1_sz);
			result.m_sub_path.append_list(thiz.m_sub_path);
			result.m_sub_path.append(as_moveable(that));
			return result;
		}

		// ----------------

		auto operator / (
			Path const & that
		) const -> Path {
			assert_test(!that.m_root && !that.m_absolute);
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_sub_path.allocate(thiz.m_sub_path.size() + that.m_sub_path.size());
			result.m_sub_path.append_list(thiz.m_sub_path);
			result.m_sub_path.append_list(that.m_sub_path);
			return result;
		}

		auto operator / (
			Path && that
		) const -> Path {
			assert_test(!that.m_root && !that.m_absolute);
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_sub_path.allocate(thiz.m_sub_path.size() + that.m_sub_path.size());
			result.m_sub_path.append_list(thiz.m_sub_path);
			result.m_sub_path.append_list(make_moveable_range_of(that.m_sub_path));
			return result;
		}

		// ----------------

		auto operator /= (
			String const & that
		) -> Path& {
			thiz.m_sub_path.append(that);
			return thiz;
		}

		auto operator /= (
			String && that
		) -> Path& {
			thiz.m_sub_path.append(as_moveable(that));
			return thiz;
		}

		// ----------------

		auto operator /= (
			Path const & that
		) -> Path& {
			assert_test(!that.m_root && !that.m_absolute);
			thiz.m_sub_path.append_list(that.m_sub_path);
			return thiz;
		}

		auto operator /= (
			Path && that
		) -> Path& {
			assert_test(!that.m_root && !that.m_absolute);
			thiz.m_sub_path.append_list(make_moveable_range_of(that.m_sub_path));
			return thiz;
		}

		#pragma endregion

		#pragma region value

		auto root (
		) const -> Optional<String> const& {
			return thiz.m_root;
		}

		auto absolute (
		) const -> Boolean const& {
			return thiz.m_absolute;
		}

		auto sub_path (
		) const -> List<String> const& {
			return thiz.m_sub_path;
		}

		#pragma endregion

		#pragma region split

		auto parent (
		) const -> Path {
			assert_test(!thiz.m_sub_path.empty());
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_sub_path = thiz.m_sub_path.head(thiz.m_sub_path.size() - 1_sz);
			return result;
		}

		auto name (
		) const -> String {
			assert_test(!thiz.m_sub_path.empty());
			return thiz.m_sub_path.last();
		}

		#pragma endregion

		#pragma region string convert

		auto from_string (
			String const & path
		) -> Void {
			thiz.m_root.reset();
			thiz.m_absolute = k_false;
			thiz.m_sub_path.reset();
			auto relative_path_begin = k_begin_index;
			if (path.size() >= 2_sz && path[2_ix] == ':'_c && CharacterType::is_alpha(path[1_ix])) {
				thiz.m_root.set(path.sub(1_ix, 2_sz));
				relative_path_begin += 2_sz;
			}
			if (path.size() > relative_path_begin && CharacterType::is_path_separator(path[relative_path_begin])) {
				thiz.m_absolute = k_true;
				relative_path_begin += 1_sz;
			}
			thiz.m_sub_path = split_string<List<String>>(path.tail(path.size() - relative_path_begin), CharacterType::PathSeparator::set);
			return;
		}

		auto to_string (
			Character const & separator = CharacterType::PathSeparator::generic
		) const -> String {
			auto result = String{};
			if (thiz.m_root) {
				result.append_list(thiz.m_root.get());
			}
			if (thiz.m_absolute) {
				result.append(separator);
			}
			result.append_list(catenate_string<String>(thiz.m_sub_path, separator));
			return result;
		}

		#pragma endregion

	public:

		#pragma region operator

		friend auto operator == (
			Path const & thix,
			Path const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}
