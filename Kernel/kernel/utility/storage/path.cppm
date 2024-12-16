module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.storage.path;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.container.optional.optional;
import twinning.kernel.utility.container.list.list;
import twinning.kernel.utility.string.string;
import twinning.kernel.utility.miscellaneous.character_series.type;
import twinning.kernel.utility.range.range_wrapper;
import twinning.kernel.utility.string.basic_string;

export namespace Twinning::Kernel::Storage {

	#pragma region type

	class Path {

	protected:

		Optional<String> m_root;

		Boolean m_absolute;

		List<String> m_relative;

	public:

		#pragma region structor

		~Path (
		) = default;

		// ----------------

		Path (
		) :
			m_root{},
			m_absolute{},
			m_relative{} {
		}

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
		) -> Path & = default;

		auto operator = (
			Path && that
		) -> Path & = default;

		// ----------------

		auto operator / (
			String const & that
		) const -> Path {
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_relative.allocate(thiz.m_relative.size() + 1_sz);
			result.m_relative.append_list(thiz.m_relative);
			result.m_relative.append(that);
			return result;
		}

		auto operator / (
			String && that
		) const -> Path {
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_relative.allocate(thiz.m_relative.size() + 1_sz);
			result.m_relative.append_list(thiz.m_relative);
			result.m_relative.append(as_moveable(that));
			return result;
		}

		// ----------------

		auto operator / (
			Path const & that
		) const -> Path {
			assert_test(!that.m_root.has() && !that.m_absolute);
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_relative.allocate(thiz.m_relative.size() + that.m_relative.size());
			result.m_relative.append_list(thiz.m_relative);
			result.m_relative.append_list(that.m_relative);
			return result;
		}

		auto operator / (
			Path && that
		) const -> Path {
			assert_test(!that.m_root.has() && !that.m_absolute);
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_relative.allocate(thiz.m_relative.size() + that.m_relative.size());
			result.m_relative.append_list(thiz.m_relative);
			result.m_relative.append_list(Range::make_moveable_range_of(that.m_relative));
			return result;
		}

		// ----------------

		auto operator /= (
			String const & that
		) -> Path & {
			thiz.m_relative.append(that);
			return thiz;
		}

		auto operator /= (
			String && that
		) -> Path & {
			thiz.m_relative.append(as_moveable(that));
			return thiz;
		}

		// ----------------

		auto operator /= (
			Path const & that
		) -> Path & {
			assert_test(!that.m_root.has() && !that.m_absolute);
			thiz.m_relative.append_list(that.m_relative);
			return thiz;
		}

		auto operator /= (
			Path && that
		) -> Path & {
			assert_test(!that.m_root.has() && !that.m_absolute);
			thiz.m_relative.append_list(Range::make_moveable_range_of(that.m_relative));
			return thiz;
		}

		#pragma endregion

		#pragma region value

		auto root (
		) const -> Optional<String> const & {
			return thiz.m_root;
		}

		auto absolute (
		) const -> Boolean const & {
			return thiz.m_absolute;
		}

		auto relative (
		) const -> List<String> const & {
			return thiz.m_relative;
		}

		#pragma endregion

		#pragma region split

		auto parent (
		) const -> Path {
			assert_test(!thiz.m_relative.empty());
			auto result = Path{};
			result.m_root = thiz.m_root;
			result.m_absolute = thiz.m_absolute;
			result.m_relative = thiz.m_relative.head(thiz.m_relative.size() - 1_sz);
			return result;
		}

		auto name (
		) const -> String {
			assert_test(!thiz.m_relative.empty());
			return thiz.m_relative.last();
		}

		#pragma endregion

		#pragma region string convert

		auto from_string (
			String const & path
		) -> Void {
			thiz.m_root.reset();
			thiz.m_absolute = k_false;
			thiz.m_relative.reset();
			auto relative_path_begin = k_begin_index;
			if (path.size() >= 2_sz && path[2_ix] == ':'_c && CharacterType::is_alpha(path[1_ix])) {
				thiz.m_root.set(path.sub(1_ix, 2_sz));
				relative_path_begin += 2_sz;
			}
			if (path.size() > relative_path_begin && CharacterType::is_path_separator(path[relative_path_begin])) {
				thiz.m_absolute = k_true;
				relative_path_begin += 1_sz;
			}
			thiz.m_relative = split_string<List<String>>(path.tail(path.size() - relative_path_begin), CharacterType::k_path_separator_set);
			return;
		}

		auto to_string (
			Character const & separator = CharacterType::k_path_separator_generic
		) const -> String {
			auto result = String{};
			if (thiz.m_root.has()) {
				result.append_list(thiz.m_root.get());
			}
			if (thiz.m_absolute) {
				result.append(separator);
			}
			result.append_list(catenate_string<String>(thiz.m_relative, separator));
			return result;
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator == (
			Path const & thix,
			Path const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}

// NOTE : USING

export namespace Twinning::Kernel {
	using Storage::Path;
}
