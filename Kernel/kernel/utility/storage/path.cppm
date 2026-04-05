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
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.string.basic_string;

export namespace Twinning::Kernel::Storage {

	#pragma region type

	M_enumeration(
		M_wrap(PathType),
		M_wrap(
			nothing,
			relative,
			absolute,
		),
	);

	M_enumeration(
		M_wrap(PathStyle),
		M_wrap(
			generic,
			native,
			posix,
			windows,
		),
	);

	// ----------------

	class Path {

	protected:

		PathType m_type;

		Optional<String> m_root;

		List<String> m_part;

	public:

		#pragma region constructor

		~Path(
		) = default;

		// ----------------

		Path(
		) :
			m_type{},
			m_root{},
			m_part{} {
			return;
		}

		Path(
			Path const & that
		) = default;

		Path(
			Path && that
		) = default;

		// ----------------

		explicit Path(
			PathType const & type
		) :
			Path{} {
			thiz.m_type = type;
			return;
		}

		explicit Path(
			String const & text
		) :
			Path{} {
			thiz.parse(text);
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			Path const & that
		) -> Path & = default;

		auto operator =(
			Path && that
		) -> Path & = default;

		#pragma endregion

		#pragma region query

		auto type(
		) const -> PathType const & {
			return thiz.m_type;
		}

		auto root(
		) const -> Optional<String> const & {
			return thiz.m_root;
		}

		auto part(
		) const -> List<String> const & {
			return thiz.m_part;
		}

		#pragma endregion

		#pragma region segment

		auto parent(
		) const -> Optional<Path> {
			auto result = Optional<Path>{};
			if (thiz.m_type != PathType::Constant::nothing() && !thiz.m_part.empty()) {
				result.set();
				result.get().m_type = thiz.m_type;
				result.get().m_root = thiz.m_root;
				result.get().m_part = thiz.m_part.head(thiz.m_part.size() - 1_sz);
			}
			return result;
		}

		auto name(
		) const -> Optional<String> {
			auto result = Optional<String>{};
			if (thiz.m_type != PathType::Constant::nothing() && !thiz.m_part.empty()) {
				result.set(thiz.m_part.last());
			}
			return result;
		}

		// ----------------

		auto stem(
		) const -> Optional<String> {
			auto result = Optional<String>{};
			if (thiz.m_type != PathType::Constant::nothing() && !thiz.m_part.empty()) {
				auto position = Range::find_index(Range::make_reverse_range_of(thiz.m_part.last()), '.'_c);
				if (position.has()) {
					result.set(thiz.m_part.last().head(thiz.m_part.last().size() - k_next_index - position.get()));
				}
				else {
					result.set(thiz.m_part.last());
				}
			}
			return result;
		}

		auto extension(
		) const -> Optional<String> {
			auto result = Optional<String>{};
			if (thiz.m_type != PathType::Constant::nothing() && !thiz.m_part.empty()) {
				auto position = Range::find_index(Range::make_reverse_range_of(thiz.m_part.last()), '.'_c);
				if (position.has()) {
					result.set(thiz.m_part.last().tail(position.get()));
				}
			}
			return result;
		}

		#pragma endregion

		#pragma region compose

		auto join(
			String && other
		) const -> Path {
			assert_test(thiz.m_type != PathType::Constant::nothing());
			auto result = Path{};
			result.m_type = thiz.m_type;
			result.m_root = thiz.m_root;
			result.m_part.allocate(thiz.m_part.size() + 1_sz);
			result.m_part.append_list(thiz.m_part);
			result.m_part.append(as_moveable(other));
			return result;
		}

		auto join(
			String const & other
		) const -> Path {
			return thiz.join(String{other});
		}

		auto push(
			Path && other
		) const -> Path {
			assert_test(thiz.m_type != PathType::Constant::nothing());
			assert_test(other.m_type == PathType::Constant::relative());
			auto result = Path{};
			result.m_type = thiz.m_type;
			result.m_root = thiz.m_root;
			result.m_part.allocate(thiz.m_part.size() + 1_sz);
			result.m_part.append_list(thiz.m_part);
			result.m_part.append_list(Range::make_moveable_range_of(other.m_part));
			return result;
		}

		auto push(
			Path const & other
		) const -> Path {
			return thiz.push(Path{other});
		}

		#pragma endregion

		#pragma region convert

		auto parse(
			String const & text
		) -> Void {
			thiz.m_root.reset();
			thiz.m_type = PathType::Constant::nothing();
			thiz.m_part.reset();
			if (!text.empty()) {
				auto position = k_begin_index;
				if (text.size() >= 2_sz && text[2_ix] == ':'_c && CharacterType::is_letter(text[1_ix])) {
					thiz.m_root.set(text.sub(1_ix, 2_sz));
					position += 2_sz;
				}
				if (text.size() > position && CharacterType::is_path_separator(text[position])) {
					thiz.m_type = PathType::Constant::absolute();
					position += 1_sz;
				}
				else {
					thiz.m_type = PathType::Constant::relative();
				}
				auto location = position;
				while (k_true) {
					if (position == text.size() || CharacterType::is_path_separator(text[position])) {
						auto segment = text.sub(location, position - location);
						if (segment == ""_sv || segment == "."_sv) {
						}
						else if (segment == ".."_sv && !thiz.m_part.empty() && thiz.m_part.last() != ".."_sv) {
							thiz.m_part.remove_tail();
						}
						else {
							thiz.m_part.append(segment);
						}
						if (position == text.size()) {
							break;
						}
						position += 1_sz;
						location = position;
						continue;
					}
					position += 1_sz;
				}
			}
			return;
		}

		auto emit(
			PathStyle const & style = PathStyle::Constant::generic()
		) const -> String {
			auto text = String{};
			if (thiz.m_type != PathType::Constant::nothing()) {
				auto mapped_style = style;
				if (style == PathStyle::Constant::generic()) {
					mapped_style = PathStyle::Constant::posix();
				}
				if (style == PathStyle::Constant::native()) {
					#if defined M_system_windows
					mapped_style = PathStyle::Constant::windows();
					#endif
					#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
					mapped_style = PathStyle::Constant::posix();
					#endif
				}
				auto separator = mapped_style == PathStyle::Constant::posix() ? CharacterType::k_path_separator_poisx : CharacterType::k_path_separator_windows;
				if (thiz.m_root.has()) {
					text.append_list(thiz.m_root.get());
				}
				if (thiz.m_type == PathType::Constant::relative()) {
					text.append('.'_c);
				}
				if (thiz.m_part.empty()) {
					text.append(separator);
				}
				for (auto & segment : thiz.m_part) {
					text.append(separator);
					if (mapped_style == PathStyle::Constant::posix()) {
						text.append_list(segment);
					}
					if (mapped_style == PathStyle::Constant::windows()) {
						auto segment_size = segment.size();
						while (segment_size != k_none_size) {
							if (segment[segment_size - 1_sz] == ' '_c) {
								segment_size -= 1_sz;
								continue;
							}
							if (segment[segment_size - 1_sz] == '.'_c && !((segment_size == 1_sz) || (segment_size == 2_sz && segment[1_ix] == '.'_c))) {
								segment_size -= 1_sz;
								continue;
							}
							break;
						}
						text.append_list(Range::make_range_n(segment.begin(), segment_size));
					}
				}
			}
			return text;
		}

		// ----------------

		auto emit_generic(
		) const -> String {
			return thiz.emit(PathStyle::Constant::generic());
		}

		auto emit_native(
		) const -> String {
			return thiz.emit(PathStyle::Constant::native());
		}

		auto emit_posix(
		) const -> String {
			return thiz.emit(PathStyle::Constant::posix());
		}

		auto emit_windows(
		) const -> String {
			return thiz.emit(PathStyle::Constant::windows());
		}

		#pragma endregion

	public:

		#pragma region operator

		inline friend auto operator ==(
			Path const & thix,
			Path const & that
		) -> bool = default;

		#pragma endregion

	};

	#pragma endregion

}

// NOTE: USING

export namespace Twinning::Kernel {
	using Storage::Path;
}
