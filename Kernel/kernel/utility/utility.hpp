#pragma once

#include "kernel/utility/macro.hpp"
#include "kernel/utility/builtin.hpp"

#include "kernel/utility/exception/source_location.hpp"
#include "kernel/utility/exception/exception.hpp"
#include "kernel/utility/exception/utility.hpp"

#include "kernel/utility/trait/base.hpp"
#include "kernel/utility/trait/category.hpp"
#include "kernel/utility/trait/template_instance.hpp"
#include "kernel/utility/trait/object.hpp"
#include "kernel/utility/trait/builtin.hpp"
#include "kernel/utility/trait/range.hpp"
#include "kernel/utility/trait/package/type_package.hpp"
#include "kernel/utility/trait/package/value_package.hpp"
#include "kernel/utility/trait/callable.hpp"
#include "kernel/utility/trait/reflection/string.hpp"
#include "kernel/utility/trait/reflection/field.hpp"
#include "kernel/utility/trait/generalization.hpp"
#include "kernel/utility/trait/trait.hpp"

#include "kernel/utility/base_wrapper/base.hpp"
#include "kernel/utility/base_wrapper/boolean.hpp"
#include "kernel/utility/base_wrapper/number.hpp"
#include "kernel/utility/base_wrapper/character.hpp"
#include "kernel/utility/base_wrapper/byte.hpp"
#include "kernel/utility/base_wrapper/pointer.hpp"
#include "kernel/utility/base_wrapper/null_pointer.hpp"
#include "kernel/utility/base_wrapper/wrapper.hpp"

#include "kernel/utility/miscellaneous/allocator.hpp"

#include "kernel/utility/null.hpp"

#include "kernel/utility/container/wrapper/wrapper_view.hpp"
#include "kernel/utility/container/wrapper/wrapper.hpp"
#include "kernel/utility/container/optional/optional_view.hpp"
#include "kernel/utility/container/optional/optional.hpp"
#include "kernel/utility/container/optional/null_optional.hpp"
#include "kernel/utility/container/variant/variant.hpp"
#include "kernel/utility/container/variant/enumerable_variant.hpp"
#include "kernel/utility/container/any/any.hpp"
#include "kernel/utility/container/tuple/tuple.hpp"

#include "kernel/utility/range/range_wrapper.hpp"
#include "kernel/utility/range/number_iterator.hpp"
#include "kernel/utility/range/number_range.hpp"
#include "kernel/utility/range/algorithm.hpp"

#include "kernel/utility/container/list/list_view.hpp"
#include "kernel/utility/container/list/list.hpp"
#include "kernel/utility/container/map/map_view.hpp"
#include "kernel/utility/container/map/map.hpp"
#include "kernel/utility/container/array/array.hpp"
#include "kernel/utility/container/static_array/static_array.hpp"
#include "kernel/utility/container/stream/stream_mode.hpp"
#include "kernel/utility/container/stream/stream_view.hpp"

#include "kernel/utility/miscellaneous/record.hpp"
#include "kernel/utility/miscellaneous/math.hpp"
#include "kernel/utility/miscellaneous/fourcc.hpp"
#include "kernel/utility/miscellaneous/number_variant.hpp"
#include "kernel/utility/miscellaneous/finalizer.hpp"

#include "kernel/utility/miscellaneous/byte_series/container.hpp"
#include "kernel/utility/miscellaneous/byte_series/utility.hpp"
#include "kernel/utility/miscellaneous/byte_series/stream_adapter.hpp"
#include "kernel/utility/miscellaneous/byte_series/stream.hpp"

#include "kernel/utility/miscellaneous/character_series/type.hpp"
#include "kernel/utility/miscellaneous/character_series/container.hpp"
#include "kernel/utility/miscellaneous/character_series/stream_adapter.hpp"
#include "kernel/utility/miscellaneous/character_series/stream.hpp"

#include "kernel/utility/string/basic_string_adapter.hpp"
#include "kernel/utility/string/basic_string_view.hpp"
#include "kernel/utility/string/basic_string.hpp"
#include "kernel/utility/string/basic_static_string.hpp"
#include "kernel/utility/string/string.hpp"
#include "kernel/utility/string/encoding.hpp"
#include "kernel/utility/string/format.hpp"
#include "kernel/utility/string/parser.hpp"

#include "kernel/utility/image/size.hpp"
#include "kernel/utility/image/color.hpp"
#include "kernel/utility/image/pixel.hpp"
#include "kernel/utility/image/image_view.hpp"
#include "kernel/utility/image/image.hpp"
#include "kernel/utility/image/regular.hpp"

#include "kernel/utility/data/json/value_adapter.hpp"
#include "kernel/utility/data/json/value.hpp"

#include "kernel/utility/data/xml/node.hpp"

#include "kernel/utility/file_system/path.hpp"
#include "kernel/utility/file_system/file_system.hpp"

#include "kernel/utility/process/process.hpp"

#include "kernel/utility/script/java_script/value_adapter.hpp"
#include "kernel/utility/script/java_script/value.hpp"
#include "kernel/utility/script/java_script/utility.hpp"

#include "kernel/utility/miscellaneous/bit_set.hpp"
#include "kernel/utility/miscellaneous/kv_pair.hpp"
#include "kernel/utility/miscellaneous/dimension.hpp"
#include "kernel/utility/miscellaneous/constant_block.hpp"
#include "kernel/utility/miscellaneous/string_block.hpp"
#include "kernel/utility/miscellaneous/thread.hpp"

#include "kernel/utility/support/std.hpp"
#include "kernel/utility/support/fmt.hpp"
#include "kernel/utility/support/byte_stream.hpp"
#include "kernel/utility/support/character_stream.hpp"
#include "kernel/utility/support/string.hpp"
#include "kernel/utility/support/json.hpp"
#include "kernel/utility/support/java_script.hpp"

namespace Twinning::Kernel {
}
