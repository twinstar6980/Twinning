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
import twinning.kernel.utility.range.number_range;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.string.basic_string;

export namespace Twinning::Kernel::Storage {

	#pragma region type

	M_enumeration(
		M_wrap(PathType),
		M_wrap(
			detached,
			relative,
			absolute,
		),
	);

	M_enumeration(
		M_wrap(PathStyle),
		M_wrap(
			posix,
			windows,
		),
	);

	// ----------------

	class Path {

	protected:

		PathType m_type;

		Optional<String> m_root;

		List<String> m_segment;

	public:

		#pragma region constructor

		~Path(
		) = default;

		// ----------------

		Path(
		) :
			m_type{PathType::Constant::detached()},
			m_root{},
			m_segment{} {
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

		auto segment(
		) const -> List<String> const & {
			return thiz.m_segment;
		}

		#pragma endregion

		#pragma region part

		auto parent(
		) const -> Optional<Path> {
			auto result = Optional<Path>{};
			if (!thiz.m_segment.empty()) {
				result.set();
				result.get().m_type = thiz.m_type;
				result.get().m_root = thiz.m_root;
				result.get().m_segment = thiz.m_segment.head(thiz.m_segment.size() - 1_sz);
			}
			return result;
		}

		auto name(
		) const -> Optional<String> {
			auto result = Optional<String>{};
			if (!thiz.m_segment.empty()) {
				result.set(thiz.m_segment.last());
			}
			return result;
		}

		// ----------------

		auto stem(
		) const -> Optional<String> {
			auto result = Optional<String>{};
			if (!thiz.m_segment.empty()) {
				auto & segment = thiz.m_segment.last();
				auto   position = Range::find_index(Range::make_reverse_range_of(segment), CharacterType::k_path_dot);
				if (position.has()) {
					result.set(segment.head(segment.size() - k_next_index - position.get()));
				}
				else {
					result.set(segment);
				}
			}
			return result;
		}

		auto extension(
		) const -> Optional<String> {
			auto result = Optional<String>{};
			if (!thiz.m_segment.empty()) {
				auto & segment = thiz.m_segment.last();
				auto   position = Range::find_index(Range::make_reverse_range_of(segment), CharacterType::k_path_dot);
				if (position.has()) {
					result.set(segment.tail(position.get()));
				}
			}
			return result;
		}

		#pragma endregion

		#pragma region compose

		auto join(
			String && other
		) const -> Path {
			auto result = Path{};
			result.m_type = thiz.m_type;
			result.m_root = thiz.m_root;
			result.m_segment.allocate(thiz.m_segment.size() + 1_sz);
			result.m_segment.append_list(thiz.m_segment);
			result.m_segment.append(as_moveable(other));
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
			assert_test(other.m_type == PathType::Constant::detached());
			auto result = Path{};
			result.m_type = thiz.m_type;
			result.m_root = thiz.m_root;
			result.m_segment.allocate(thiz.m_segment.size() + other.m_segment.size());
			result.m_segment.append_list(thiz.m_segment);
			result.m_segment.append_list(Range::make_moveable_range_of(other.m_segment));
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
			thiz.m_type = PathType::Constant::detached();
			thiz.m_root.reset();
			thiz.m_segment.reset();
			auto position = k_begin_index;
			if (text.size() >= 2_sz && CharacterType::is_letter(text[1_ix]) && CharacterType::is_path_volume_separator(text[2_ix])) {
				position += 2_sz;
				if (position < text.size() && CharacterType::is_path_directory_separator(text[position])) {
					thiz.m_type = PathType::Constant::absolute();
					thiz.m_root.set(text.sub(1_ix, position));
					position += 1_sz;
				}
				else {
					thiz.m_type = PathType::Constant::relative();
					thiz.m_root.set(text.sub(1_ix, position));
				}
			}
			else if (text.size() >= 2_sz && CharacterType::is_path_directory_separator(text[1_ix]) && CharacterType::is_path_directory_separator(text[2_ix])) {
				position += 2_sz;
				for (; position < text.size(); ++position) {
					if (CharacterType::is_path_directory_separator(text[position])) {
						break;
					}
				}
				thiz.m_type = PathType::Constant::absolute();
				thiz.m_root.set("//"_s + text.sub(3_ix, position - 3_ix));
				if (position < text.size() && CharacterType::is_path_directory_separator(text[position])) {
					position += 1_sz;
				}
			}
			else if (text.size() >= 1_sz && CharacterType::is_path_directory_separator(text[1_ix])) {
				position += 1_sz;
				thiz.m_type = PathType::Constant::absolute();
				thiz.m_root.set();
			}
			else if (text.size() >= 2_sz && CharacterType::is_path_dot(text[1_ix]) && CharacterType::is_path_dot(text[2_ix])) {
				position += 2_sz;
				if (position == text.size() || CharacterType::is_path_directory_separator(text[position])) {
					thiz.m_type = PathType::Constant::relative();
					thiz.m_root.set();
				}
				position = 0_sz;
			}
			else if (text.size() >= 1_sz && CharacterType::is_path_dot(text[1_ix])) {
				position += 1_sz;
				if (position == text.size() || CharacterType::is_path_directory_separator(text[position])) {
					thiz.m_type = PathType::Constant::relative();
					thiz.m_root.set();
					if (position < text.size()) {
						position += 1_sz;
					}
				}
				else {
					position = 0_sz;
				}
			}
			auto location = position;
			for (; position <= text.size(); ++position) {
				if (position == text.size() || CharacterType::is_path_directory_separator(text[position])) {
					auto segment = text.sub(location, position - location);
					if (!segment.empty()) {
						thiz.m_segment.append(segment);
					}
					location = position + 1_sz;
				}
			}
			return;
		}

		auto emit(
			PathStyle const & style = PathStyle::Constant::posix(),
			Boolean const &   rectify = k_false
		) const -> String {
			auto text = String{};
			auto path_dot = CharacterType::k_path_dot;
			auto path_directory_separator = style == PathStyle::Constant::posix() ? CharacterType::k_path_directory_separator_poisx : CharacterType::k_path_directory_separator_windows;
			if (thiz.m_type != PathType::Constant::detached()) {
				for (auto & index : SizeRange{thiz.m_root.get().size()}) {
					text.append(!CharacterType::is_path_directory_separator(thiz.m_root.get()[index]) ? thiz.m_root.get()[index] : path_directory_separator);
				}
				if (thiz.m_type == PathType::Constant::relative()) {
					text.append(path_dot);
				}
				text.append(path_directory_separator);
			}
			for (auto & segment_index : SizeRange{thiz.m_segment.size()}) {
				auto & segment = thiz.m_segment[segment_index];
				if (segment_index != k_begin_index) {
					text.append(path_directory_separator);
				}
				if (!rectify) {
					text.append_list(segment);
				}
				else {
					if (style == PathStyle::Constant::posix()) {
						text.append_list(segment);
					}
					if (style == PathStyle::Constant::windows()) {
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

		auto emit_posix(
			Boolean const & rectify = k_false
		) const -> String {
			return thiz.emit(PathStyle::Constant::posix(), rectify);
		}

		auto emit_windows(
			Boolean const & rectify = k_false
		) const -> String {
			return thiz.emit(PathStyle::Constant::windows(), rectify);
		}

		auto emit_native(
		) const -> String {
			auto mapped_style = PathStyle{};
			#if defined M_system_windows
			mapped_style = PathStyle::Constant::windows();
			#endif
			#if defined M_system_linux || defined M_system_macintosh || defined M_system_android || defined M_system_iphone
			mapped_style = PathStyle::Constant::posix();
			#endif
			return thiz.emit(mapped_style, k_true);
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
