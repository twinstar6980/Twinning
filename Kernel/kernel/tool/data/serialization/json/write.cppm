module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.data.serialization.json.write;
import twinning.kernel.utility;
import twinning.kernel.tool.data.serialization.json.common;

export namespace Twinning::Kernel::Tool::Data::Serialization::JSON {

	struct Write :
		Common {

		using Common = Common;

		// ----------------

		inline static auto process_value (
			OCharacterStreamView & data,
			Value const &          value,
			Boolean const &        disable_array_trailing_comma,
			Boolean const &        disable_array_line_breaking,
			Boolean const &        disable_object_trailing_comma,
			Boolean const &        disable_object_line_breaking,
			Size const &           indent_level
		) -> Void {
			auto write_space = [&] (
				Boolean const & disable_line_breaking,
				Boolean const & is_inner
			) -> Void {
				if (!disable_line_breaking) {
					data.write('\n'_c);
					StringParser::write_character_repeat(data, '\t'_c, indent_level + (is_inner ? (1_sz) : (0_sz)));
				}
				return;
			};
			switch (value.type().value) {
				case ValueType::Constant::null().value : {
					StringParser::write_null(data, value.get_null());
					break;
				}
				case ValueType::Constant::boolean().value : {
					StringParser::write_boolean(data, value.get_boolean());
					break;
				}
				case ValueType::Constant::number().value : {
					StringParser::write_number(data, value.get_number(), k_true);
					break;
				}
				case ValueType::Constant::string().value : {
					data.write('"'_c);
					StringParser::write_escape_utf8_string_until(data, as_lvalue(ICharacterStreamView{value.get_string()}), '"'_c);
					data.write('"'_c);
					break;
				}
				case ValueType::Constant::array().value : {
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
				case ValueType::Constant::object().value : {
					auto & object = value.get_object();
					data.write('{'_c);
					for (auto & member : object) {
						write_space(disable_object_line_breaking, k_true);
						data.write('"'_c);
						StringParser::write_escape_utf8_string_until(data, as_lvalue(ICharacterStreamView{member.key}), '"'_c);
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
			}
			return;
		}

		inline static auto process_whole (
			OCharacterStreamView & data,
			Value const &          value,
			Boolean const &        disable_array_trailing_comma,
			Boolean const &        disable_array_line_breaking,
			Boolean const &        disable_object_trailing_comma,
			Boolean const &        disable_object_line_breaking
		) -> Void {
			process_value(data, value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking, k_begin_index);
			return;
		}

		// ----------------

		inline static auto process (
			OCharacterStreamView & data_,
			Value const &          value,
			Boolean const &        disable_array_trailing_comma,
			Boolean const &        disable_array_line_breaking,
			Boolean const &        disable_object_trailing_comma,
			Boolean const &        disable_object_line_breaking
		) -> Void {
			M_use_zps_of(data);
			return process_whole(data, value, disable_array_trailing_comma, disable_array_line_breaking, disable_object_trailing_comma, disable_object_line_breaking);
		}

	};

}
