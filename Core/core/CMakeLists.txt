project(
	core
	LANGUAGES CXX
)

add_library(
	${PROJECT_NAME} SHARED
	
	./version.hpp
	./macro.hpp
	./base_type.hpp
	
	./third_party/fmt.hpp
	./third_party/tinyxml2.hpp
	./third_party/quickjs.hpp
	./third_party/md5.hpp
	./third_party/zlib.hpp
	./third_party/bzip2.hpp
	./third_party/lzma.hpp
	./third_party/libpng.hpp
	./third_party/ETCPACK.hpp
	./third_party/rg_etc1.hpp
	./third_party/PVRTCCompressor.hpp
	./third_party/system/windows.hpp
	./third_party/system/posix.hpp
	
	./utility/exception/source_location.hpp
	./utility/exception/exception.hpp
	./utility/exception/simple_exception.hpp
	./utility/exception/special_exception.hpp
	./utility/exception/base_exception/utility.hpp
	./utility/exception/base_exception/exception_message.hpp
	./utility/exception/base_exception/base_exception.hpp
	
	./utility/trait/base.hpp
	./utility/trait/category.hpp
	./utility/trait/template_instance.hpp
	./utility/trait/object.hpp
	./utility/trait/builtin.hpp
	./utility/trait/enum.hpp
	./utility/trait/range.hpp
	./utility/trait/package/type_package.hpp
	./utility/trait/package/value_package.hpp
	./utility/trait/callable.hpp
	./utility/trait/reflection/string.hpp
	./utility/trait/reflection/information.hpp
	./utility/trait/reflection/field.hpp
	./utility/trait/reflection/utility.hpp
	./utility/trait/generalization.hpp
	./utility/trait/trait.hpp
	
	./utility/base_wrapper/base.hpp
	./utility/base_wrapper/boolean.hpp
	./utility/base_wrapper/number.hpp
	./utility/base_wrapper/byte.hpp
	./utility/base_wrapper/character.hpp
	./utility/base_wrapper/pointer.hpp
	./utility/base_wrapper/null_pointer.hpp
	./utility/base_wrapper/wrapper.hpp
	
	./utility/misc/allocator.hpp
	
	./utility/null.hpp
	
	./utility/container/wrapper/wrapper_view.hpp
	./utility/container/wrapper/wrapper.hpp
	./utility/container/optional/optional_view.hpp
	./utility/container/optional/optional.hpp
	./utility/container/optional/null_optional.hpp
	./utility/container/variant/variant.hpp
	./utility/container/variant/enumerable_variant.hpp
	./utility/container/any/any.hpp
	./utility/container/tuple/tuple.hpp
	
	./utility/range/range_wrapper.hpp
	./utility/range/number_iterator.hpp
	./utility/range/number_range.hpp
	./utility/range/algorithm.hpp
	
	./utility/container/list/list_view.hpp
	./utility/container/list/list.hpp
	./utility/container/map/map_view.hpp
	./utility/container/map/map.hpp
	./utility/container/array/array.hpp
	./utility/container/static_array/static_array.hpp
	./utility/container/stream/stream_method.hpp
	./utility/container/stream/stream_view.hpp
	
	./utility/misc/math.hpp
	./utility/misc/enum.hpp
	./utility/misc/fourcc.hpp
	./utility/misc/number_variant.hpp
	./utility/misc/finalizer.hpp
	
	./utility/misc/byte_series/container.hpp
	./utility/misc/byte_series/utility.hpp
	./utility/misc/byte_series/stream_adapter.hpp
	./utility/misc/byte_series/stream.hpp
	./utility/misc/byte_series/stream_adapter_utility.hpp
	
	./utility/misc/character_series/type.hpp
	./utility/misc/character_series/container.hpp
	./utility/misc/character_series/stream_adapter.hpp
	./utility/misc/character_series/stream.hpp
	
	./utility/string/basic_string_adapter.hpp
	./utility/string/basic_string_view.hpp
	./utility/string/basic_string.hpp
	./utility/string/basic_static_string.hpp
	./utility/string/string.hpp
	./utility/string/encoding.hpp
	./utility/string/format.hpp
	./utility/string/parser.hpp
	
	./utility/image/size.hpp
	./utility/image/channel.hpp
	./utility/image/pixel.hpp
	./utility/image/basic_bitmap_view.hpp
	./utility/image/basic_bitmap.hpp
	./utility/image/bitmap.hpp
	./utility/image/file/png.hpp
	
	./utility/data/json/value_adapter.hpp
	./utility/data/json/value.hpp
	./utility/data/json/value_adapter_utility.hpp
	./utility/data/json/parser.hpp
	
	./utility/data/xml/node.hpp
	./utility/data/xml/parser.hpp
	
	./utility/script/js/value_adapter.hpp
	./utility/script/js/handler.hpp
	./utility/script/js/js.hpp
	./utility/script/js/utility.hpp
	
	./utility/file_system/path.hpp
	./utility/file_system/file_system.hpp
	
	./utility/system/windows.hpp
	./utility/system/posix.hpp
	./utility/system/system.hpp
	
	./utility/misc/bitset.hpp
	./utility/misc/kv_pair.hpp
	./utility/misc/2d_type.hpp
	./utility/misc/padding_block.hpp
	./utility/misc/string_block.hpp
	./utility/misc/thread.hpp
	
	./utility/support/byte_stream.hpp
	./utility/support/character_stream.hpp
	./utility/support/string.hpp
	./utility/support/json.hpp
	./utility/support/js.hpp
	./utility/support/stl.hpp
	./utility/support/fmt.hpp

	./utility/utility.hpp
	
	./tool/common/version_utility.hpp
	./tool/common/protocol_buffer_variable_length_integer.hpp
	./tool/common/wave_structure.hpp
	
	./tool/data/hash/fnv.hpp
	./tool/data/hash/md5.hpp
	
	./tool/data/encode/base64.hpp
	
	./tool/data/encrypt/xor.hpp
	./tool/data/encrypt/rijndael.hpp
	
	./tool/data/compress/deflate.hpp
	./tool/data/compress/bzip2.hpp
	./tool/data/compress/lzma.hpp
	
	./tool/texture/encode.hpp
	./tool/texture/compress/etc1.hpp
	./tool/texture/compress/pvrtc4.hpp
	
	./tool/wwise/sound_bank/version.hpp
	./tool/wwise/sound_bank/manifest.hpp
	./tool/wwise/sound_bank/structure.hpp
	./tool/wwise/sound_bank/pack.hpp
	
	./tool/wwise/encoded_media/structure.hpp
	./tool/wwise/encoded_media/encode.hpp
	
	./tool/marmalade/dzip/version.hpp
	./tool/marmalade/dzip/manifest.hpp
	./tool/marmalade/dzip/structure.hpp
	./tool/marmalade/dzip/pack.hpp
	
	./tool/popcap/zlib/structure.hpp
	./tool/popcap/zlib/compress.hpp
	
	./tool/popcap/reanim/version.hpp
	./tool/popcap/reanim/manifest.hpp
	./tool/popcap/reanim/structure.hpp
	./tool/popcap/reanim/encode.hpp
	
	./tool/popcap/rton/version.hpp
	./tool/popcap/rton/structure.hpp
	./tool/popcap/rton/encode.hpp
	./tool/popcap/rton/encrypt.hpp
	
	./tool/popcap/pam/version.hpp
	./tool/popcap/pam/manifest.hpp
	./tool/popcap/pam/structure.hpp
	./tool/popcap/pam/encode.hpp
	
	./tool/popcap/pak/version.hpp
	./tool/popcap/pak/manifest.hpp
	./tool/popcap/pak/structure.hpp
	./tool/popcap/pak/pack.hpp
	
	./tool/popcap/rsgp/version.hpp
	./tool/popcap/rsgp/manifest.hpp
	./tool/popcap/rsgp/structure.hpp
	./tool/popcap/rsgp/pack.hpp
	
	./tool/popcap/rsb/version.hpp
	./tool/popcap/rsb/common.hpp
	./tool/popcap/rsb/structure.hpp
	./tool/popcap/rsb/manifest.hpp
	./tool/popcap/rsb/description.hpp
	./tool/popcap/rsb/pack.hpp
	
	./tool/misc/pvz1_rsb_texture_20_series_layout/encode.hpp
	./tool/misc/pvz2_chs_rsb_texture_alpha_index/encode.hpp
	
	./executor/shell_callback.hpp
	./executor/context.hpp
	./executor/api.hpp
	./executor/executor.hpp
	
	./interface/implement.hpp
	./interface/interface.cpp
	./interface/interface.hpp
)

include(${M_root_project_directory}/cmake/condition_definition.cmake)
include(${M_root_project_directory}/cmake/compile_option.cmake)

target_link_libraries(
	${PROJECT_NAME} PRIVATE
	fmt
	tinyxml2
	quickjs
	md5
	zlib
	bzip2
	lzma
	libpng
	ETCPACK
	rg_etc1
	PVRTCCompressor
)

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
)

target_compile_definitions(
	${PROJECT_NAME} PRIVATE
	CONFIG_BIGNUM
)
if(M_compiler_msvc)
	target_compile_definitions(
		${PROJECT_NAME} PRIVATE
		JS_STRICT_NAN_BOXING
	)
endif()
