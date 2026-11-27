module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.serialization.json.encode;
import twinning.kernel.utility;
import twinning.kernel.tool.data.serialization.json.common;

export namespace Twinning::Kernel::Tool::Data::Serialization::Json {

	struct Encode :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_value(
			OutputStringStreamView &      data,
			Notation::Json::Value const & value,
			Boolean const &               disable_array_trailing_comma,
			Boolean const &               disable_array_line_breaking,
			Boolean const &               disable_object_trailing_comma,
			Boolean const &               disable_object_line_breaking,
			Size const &                  indent_level
		) -> Void {
			auto write_space = [&](
				Boolean const & disable_line_breaking,
				Boolean const & is_inner
			) -> Void {
				if (!disable_line_breaking) {
					data.write('\n'_c);
					data.write_space(indent_level + (is_inner ? (1_sz) : (0_sz)), '\t'_c);
				}
				return;
			};
			switch (value.type().value) {
				case Notation::Json::ValueType::Constant::null().value: {
					data.write(value.get_null());
					break;
				}
				case Notation::Json::ValueType::Constant::boolean().value: {
					data.write(value.get_boolean());
					break;
				}
				case Notation::Json::ValueType::Constant::number().value: {
					data.write(value.get_number(), k_true);
					break;
				}
				case Notation::Json::ValueType::Constant::string().value: {
					data.write('"'_c);
					StringUtility::write_escape_utf8_string_until(data, as_left(InputStringStreamView{value.get_string()}), '"'_c);
					data.write('"'_c);
					break;
				}
				case Notation::Json::ValueType::Constant::array().value: {
					auto & array = value.get_array();
					data.write('['_c);
					for (auto & element : array) {
						write_space(disable_array_line_breaking, k_true);
						process_value(data, element, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, indent_level + (!disable_array_line_breaking ? (1_sz) : (0_sz)));
						data.write(','_c);
					}
					if (disable_array_trailing_comma && !array.empty()) {
						data.backward();
					}
					write_space(disable_array_line_breaking, k_false);
					data.write(']'_c);
					break;
				}
				case Notation::Json::ValueType::Constant::object().value: {
					auto & object = value.get_object();
					data.write('{'_c);
					for (auto & member : object) {
						write_space(disable_object_line_breaking, k_true);
						data.write('"'_c);
						StringUtility::write_escape_utf8_string_until(data, as_left(InputStringStreamView{member.key}), '"'_c);
						data.write('"'_c);
						data.write(':'_c);
						if (!disable_object_line_breaking) {
							data.write(' '_c);
						}
						process_value(data, member.value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, indent_level + (!disable_object_line_breaking ? (1_sz) : (0_sz)));
						data.write(','_c);
					}
					if (disable_object_trailing_comma && !object.empty()) {
						data.backward();
					}
					write_space(disable_object_line_breaking, k_false);
					data.write('}'_c);
					break;
				}
				default: throw UnreachableException{};
			}
			return;
		}

		inline static auto process_whole(
			OutputStringStreamView &      data,
			Notation::Json::Value const & value,
			Boolean const &               disable_array_trailing_comma,
			Boolean const &               disable_array_line_breaking,
			Boolean const &               disable_object_trailing_comma,
			Boolean const &               disable_object_line_breaking
		) -> Void {
			process_value(data, value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, 0_sz);
			return;
		}

		// ----------------

		inline static auto process(
			OutputStringStreamView &      data_,
			Notation::Json::Value const & value,
			Boolean const &               disable_array_trailing_comma,
			Boolean const &               disable_array_line_breaking,
			Boolean const &               disable_object_trailing_comma,
			Boolean const &               disable_object_line_breaking
		) -> Void {
			M_use_split_stream_of(data);
			return process_whole(data, value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking);
		}

	};

}
