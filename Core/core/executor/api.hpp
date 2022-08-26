#pragma once

#include "core/utility/utility.hpp"
#include "core/executor/shell_callback.hpp"
#include "core/executor/context.hpp"
#include "core/tool/data/hash/fnv.hpp"
#include "core/tool/data/hash/md5.hpp"
#include "core/tool/data/encode/base64.hpp"
#include "core/tool/data/encrypt/xor.hpp"
#include "core/tool/data/encrypt/rijndael.hpp"
#include "core/tool/data/compress/deflate.hpp"
#include "core/tool/data/compress/bzip2.hpp"
#include "core/tool/data/compress/lzma.hpp"
#include "core/tool/texture/encode.hpp"
#include "core/tool/texture/compress/etc1.hpp"
#include "core/tool/texture/compress/pvrtc4.hpp"
#include "core/tool/wwise/sound_bank/pack.hpp"
#include "core/tool/wwise/encoded_media/encode.hpp"
#include "core/tool/marmalade/dzip/pack.hpp"
#include "core/tool/popcap/zlib/compress.hpp"
#include "core/tool/popcap/reanim/encode.hpp"
#include "core/tool/popcap/rton/encode.hpp"
#include "core/tool/popcap/rton/encrypt.hpp"
#include "core/tool/popcap/pam/encode.hpp"
#include "core/tool/popcap/pak/pack.hpp"
#include "core/tool/popcap/rsgp/pack.hpp"
#include "core/tool/popcap/rsb/pack.hpp"
#include "core/tool/misc/pvz1_rsb_texture_20_series_layout/encode.hpp"
#include "core/tool/misc/pvz2_chs_rsb_texture_alpha_index/encode.hpp"

namespace TwinKleS::Core::Executor::API {

	#pragma region define generic class

	enum class GenericClassDefinitionFlag : ZIntegerU8 {
		// generic operation
		// - static default(): Type
		default_constructor = 1 << 0,
		// - static copy(that: Type): Type
		copy_constructor = 1 << 1,
		// value operation
		// - static value(value: _RawOf_<Type>): Type
		value_constructor = 1 << 2,
		// - get value(): _RawOf_<Type>
		value_getter = 1 << 3,
		// - set value(_RawOf_<Type>): Void
		value_setter = 1 << 4,
		// json operation
		// - static json(json: JSON.Value): Type
		json_constructor = 1 << 5,
		// - get json(): JSON.Value
		json_getter = 1 << 6,
		// - set json(JSON.Value): Void
		json_setter = 1 << 7,
		// mask
		none_mask = 0b000'000'00,
		all_mask = 0b111'111'11,
		generic_mask = default_constructor | copy_constructor,
		value_mask = value_constructor | value_getter | value_setter,
		json_mask = json_constructor | json_getter | json_setter,
		default_mask = generic_mask | value_mask,
	};

	// NOTE : alias
	using GCDF = GenericClassDefinitionFlag;

	// ----------------

	inline constexpr auto operator | (
		GCDF const & thix,
		GCDF const & that
	) -> GCDF {
		return static_cast<GCDF>(static_cast<AsEnumUnderlying<GCDF>>(thix) | static_cast<AsEnumUnderlying<GCDF>>(that));
	}

	inline constexpr auto operator & (
		GCDF const & thix,
		GCDF const & that
	) -> GCDF {
		return static_cast<GCDF>(static_cast<AsEnumUnderlying<GCDF>>(thix) & static_cast<AsEnumUnderlying<GCDF>>(that));
	}

	inline constexpr auto operator ~ (
		GCDF const & thix
	) -> GCDF {
		return static_cast<GCDF>(~static_cast<AsEnumUnderlying<GCDF>>(thix));
	}

	inline constexpr auto operator * (
		GCDF const & thix,
		GCDF const & that
	) -> Boolean {
		return mbw<Boolean>(static_cast<AsEnumUnderlying<GCDF>>(thix) & static_cast<AsEnumUnderlying<GCDF>>(that));
	}

	// ----------------

	template <typename Class, auto flag = GCDF::default_mask> requires
		CategoryConstraint<IsPureInstance<Class>>
		&& (IsSameV<flag, GCDF>)
	inline auto define_generic_class (
		JS::NamespaceBuilder & name_space,
		String const &         class_name
	) -> JS::ClassBuilder<Class> {
		auto builder = name_space.add_class<Class>(class_name);
		builder.template set_constructor<
			&normalized_lambda<
				[] (
			) -> JS::Handler<Class> {
					throw SimpleException{"js style constructor is not allowed"};
				}
			>
		>();
		if constexpr (flag * GCDF::default_constructor) {
			builder.template add_second_constructor_allocate_proxy<>("default"_s);
		}
		if constexpr (flag * GCDF::copy_constructor) {
			builder.template add_second_constructor_allocate_proxy<Class const &>("copy"_s);
		}
		if constexpr (flag * GCDF::value_constructor) {
			builder.template add_second_constructor<
				&normalized_lambda<
					[] (
					Class & that
				) -> JS::Handler<Class> {
						return JS::Handler<Class>::new_instance_allocate(that);
					}
				>
			>("value"_s);
		}
		if constexpr (flag * GCDF::value_getter || flag * GCDF::value_setter) {
			static_assert(flag * GCDF::value_getter);
			// NOTE : return reference is cheap
			constexpr auto & getter = normalized_lambda<
				[] (
				JS::Handler<Class> & thix
			) -> Class& {
					return thix.value();
				}
			>;
			if constexpr (!(flag * GCDF::value_setter)) {
				builder.template add_getter<&getter>("value"_s);
			} else {
				constexpr auto & setter = normalized_lambda<
					[] (
					JS::Handler<Class> & thix,
					Class &              value
				) -> Void {
						// NOTE : some type has not copy assignment
						restruct(thix.value(), value);
						return;
					}
				>;
				builder.template add_getter_setter<&getter, &setter>("value"_s);
			}
		}
		if constexpr (flag * GCDF::json_constructor) {
			builder.template add_second_constructor<
				&JS::proxy_function_by_handler<
					&normalized_lambda<
						[] (
						JSON::Value const & json
					) -> Class {
							auto it = Class{};
							json.to(it);
							return it;
						}
					>
				>
			>("json"_s);
		}
		if constexpr (flag * GCDF::json_getter || flag * GCDF::json_setter) {
			static_assert(flag * GCDF::json_getter);
			constexpr auto & getter = JS::proxy_function_by_handler<
				&normalized_lambda<
					[] (
					Class & thix
				) -> JSON::Value {
						auto json = JSON::Value{};
						json.from(thix);
						return json;
					}
				>
			>;
			if constexpr (!(flag * GCDF::json_setter)) {
				builder.template add_getter<&getter>("json"_s);
			} else {
				constexpr auto & setter = JS::proxy_function_by_handler<
					&normalized_lambda<
						[] (
						Class &             thix,
						JSON::Value const & json
					) -> Void {
							json.to(thix);
							return;
						}
					>
				>;
				builder.template add_getter_setter<&getter, &setter>("json"_s);
			}
		}
		return builder;
	}

	// json operation
	// - static json(json: JSON.Value, version: Version): Type
	// - json(version: Version): JSON.Value
	// - json(version: Version, value: JSON.Value): Void
	template <typename Version, typename VersionEnum, typename Class> requires
		CategoryConstraint<IsPureInstance<Version> && IsPureInstance<VersionEnum> && IsPureInstance<Class>>
	inline auto define_variant_class_version_method (
		JS::ClassBuilder<Class> & builder
	) -> JS::ClassBuilder<Class>& {
		constexpr auto & constructor = JS::proxy_function_by_handler<
			&normalized_lambda<
				[] (
				JSON::Value const & json,
				Version const &     version
			) -> Class {
					auto it = Class{};
					Tool::json_to_variant_of_version<VersionEnum>(version, json, it);
					return it;
				}
			>
		>;
		constexpr auto & getter = JS::proxy_function_by_handler<
			&normalized_lambda<
				[] (
				Class &         thix,
				Version const & version
			) -> JSON::Value {
					auto json = JSON::Value{};
					Tool::json_from_variant_of_version<VersionEnum>(version, json, thix);
					return json;
				}
			>
		>;
		constexpr auto & setter = JS::proxy_function_by_handler<
			&normalized_lambda<
				[] (
				Class &             thix,
				Version const &     version,
				JSON::Value const & json
			) -> Void {
					Tool::json_to_variant_of_version<VersionEnum>(version, json, thix);
					return;
				}
			>
		>;
		builder.template add_second_constructor<&constructor>("from_json"_s);
		builder.template add_member_function<&getter>("to_json"_s);
		builder.template add_member_function<&setter>("from_json"_s);
		return builder;
	}

	#pragma endregion

	#pragma region proxy function by special type promotion

	/* NOTE :
	 * 为了简化API，对部分类型进行提升
	 * (V|C)StringView          -> String
	 * (C)ByteListView          -> VByteListView            : ByteListView
	 * (I|O)ByteStreamView      -> IOByteStreamView         : ByteStreamView
	 * (C)CharacterListView     -> VCharacterListView       : CharacterListView
	 * (I|O)CharacterStreamView -> IOCharacterStreamView    : CharacterStreamView
	 * Image::(C)BitmapView     -> Image::VBitmapView       : Image::BitmapView
	 */

	namespace Detail {

		template <typename Type> requires
			NoneConstraint
		using AsPromotion = AsSwitch<
			IsSame<Type, CStringView, VStringView>,
			String,
			AsSwitch<
				IsSame<Type, CByteListView>,
				VByteListView,
				AsSwitch<
					IsSame<Type, IByteStreamView, OByteStreamView>,
					IOByteStreamView,
					AsSwitch<
						IsSame<Type, CCharacterListView>,
						VCharacterListView,
						AsSwitch<
							IsSame<Type, ICharacterStreamView, OCharacterStreamView>,
							IOCharacterStreamView,
							AsSwitch<
								IsSame<Type, Image::CBitmapView>,
								Image::VBitmapView,
								Type
							>
						>
					>
				>
			>
		>;

		/*template <typename Type> requires
			NoneConstraint
		using AsPromotion = decltype([] {
			if constexpr (IsSame<Type, CStringView, VStringView>) {
				return String{};
			} else if constexpr (IsSame<Type, CByteListView>) {
				return VByteListView{};
			} else if constexpr (IsSame<Type, IByteStreamView, OByteStreamView>) {
				return IOByteStreamView{};
			} else if constexpr (IsSame<Type, CCharacterListView>) {
				return CCharacterListView{};
			} else if constexpr (IsSame<Type, ICharacterStreamView, OCharacterStreamView>) {
				return IOCharacterStreamView{};
			} else if constexpr (IsSame<Type, Image::CBitmapView>) {
				return Image::VBitmapView{};
			} else {
				return Type{};
			}
		}());*/

		template <auto function, auto ...index> requires
			NoneConstraint
		inline constexpr auto make_proxy_function_by_some_type (
			ValuePackage<index...>
		) -> auto {
			if constexpr ((IsSame<AsPromotion<AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>>, AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>> && ...)) {
				return function;
			} else {
				return &proxy_global_function<function, AsPromotion<AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>> &...>;
			}
		}

	}

	// ----------------

	template <auto function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
	inline constexpr auto & proxy_function_by_special_type_promotion = *Detail::make_proxy_function_by_some_type<function>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

	// NOTE : alias
	template <auto function> requires
		AutoConstraint
	inline constexpr auto & stp = proxy_function_by_special_type_promotion<function>;

	#pragma endregion

	#pragma region inject

	inline auto inject (
		Context & context
	) -> Void {
		#if defined M_compiler_msvc
		#pragma warning(push)
		#pragma warning(disable:4456)
		#endif
		#if defined M_compiler_clang
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wshadow"
		#endif
		auto n_TwinKleS = JS::NamespaceBuilder{context.context(), as_lvalue(context.context().global_object()), k_null_optional, "TwinKleS"_s};
		auto n_Core = n_TwinKleS.add_namespace("Core"_s);
		// Boolean
		define_generic_class<Boolean>(n_Core, "Boolean"_s);
		// Number
		define_generic_class<IntegerU8>(n_Core, "IntegerU8"_s);
		define_generic_class<IntegerU32>(n_Core, "IntegerU32"_s);
		define_generic_class<IntegerU64>(n_Core, "IntegerU64"_s);
		define_generic_class<IntegerS32>(n_Core, "IntegerS32"_s);
		// Size
		define_generic_class<Size>(n_Core, "Size"_s);
		define_generic_class<Optional<Size>>(n_Core, "SizeOptional"_s);
		// Byte
		define_generic_class<Byte>(n_Core, "Byte"_s);
		// String
		define_generic_class<String>(n_Core, "String"_s);
		define_generic_class<List<String>>(n_Core, "StringList"_s);
		// Path
		define_generic_class<Path>(n_Core, "Path"_s);
		define_generic_class<Optional<Path>>(n_Core, "PathOptional"_s);
		define_generic_class<List<Path>>(n_Core, "PathList"_s);
		// ByteArray
		define_generic_class<ByteArray>(n_Core, "ByteArray"_s)
			.add_second_constructor_allocate_proxy<Size const &>("allocate"_s)
			.add_member_function_proxy<&ByteArray::allocate>("allocate"_s)
			.add_member_function_proxy<&ByteArray::reset>("reset"_s)
			.add_member_function_proxy<&ByteArray::size>("size"_s)
			.add_member_function_proxy<AsVMemberFunction<ByteArray, ByteArray::View>{&ByteArray::view}>("view"_s)
			.add_member_function<
				&normalized_lambda<
					[] (
					JS::Handler<ByteArray> & that
				) -> ByteArray&& {
						return as_moveable(that.value());
					}
				>
			>("release"_s);
		// ByteListView
		define_generic_class<VByteListView>(n_Core, "ByteListView"_s)
			.add_member_function_proxy<&VByteListView::size>("size"_s)
			.add_member_function_proxy<AsCMemberFunction<VByteListView, VByteListView, Size const &, Size const &>{&VByteListView::sub}>("sub"_s);
		// ByteStreamView
		define_generic_class<
				IOByteStreamView,
				GCDF::generic_mask
			>(n_Core, "ByteStreamView"_s)
			.add_second_constructor_allocate_proxy<VByteListView const &>("look"_s)
			.add_member_function_proxy<&IOByteStreamView::size>("size"_s)
			.add_member_function_proxy<&IOByteStreamView::position>("position"_s)
			.add_member_function_proxy<&IOByteStreamView::set_position>("set_position"_s)
			.add_member_function_proxy<&IOByteStreamView::view>("view"_s)
			.add_member_function_proxy<&IOByteStreamView::stream_view>("stream_view"_s)
			.add_member_function_proxy<AsVMemberFunction<IOByteStreamView, Byte>{&IOByteStreamView::read_of}>("read"_s)
			.add_member_function_proxy<AsVMemberFunction<IOByteStreamView, Void, Byte const &>{&IOByteStreamView::write}>("write"_s);
		n_Core.add_variable("g_byte_stream_use_big_endian"_s, context.context().new_value(JS::Handler<Boolean>::new_reference(g_byte_stream_use_big_endian)));
		// CharacterListView
		define_generic_class<
				VCharacterListView,
				GCDF::generic_mask
			>(n_Core, "CharacterListView"_s)
			.add_member_function_proxy<&VCharacterListView::size>("size"_s)
			.add_member_function_proxy<AsCMemberFunction<VCharacterListView, VCharacterListView, Size const &, Size const &>{&VCharacterListView::sub}>("sub"_s);
		// CharacterStreamView
		define_generic_class<
				IOCharacterStreamView,
				GCDF::generic_mask
			>(n_Core, "CharacterStreamView"_s)
			.add_second_constructor_allocate_proxy<VCharacterListView const &>("look"_s)
			.add_member_function_proxy<&IOCharacterStreamView::size>("size"_s)
			.add_member_function_proxy<&IOCharacterStreamView::position>("position"_s)
			.add_member_function_proxy<&IOCharacterStreamView::set_position>("set_position"_s)
			.add_member_function_proxy<&IOCharacterStreamView::view>("view"_s)
			.add_member_function_proxy<&IOCharacterStreamView::stream_view>("stream_view"_s);
		// JSON
		{
			auto n_JSON = n_Core.add_namespace("JSON"_s);
			define_generic_class<JSON::Value>(n_JSON, "Value"_s);
			{
				auto n_Write = n_JSON.add_namespace("Write"_s);
				n_Write
					.add_function_proxy<&stp<&JSON::Write::process>>("process"_s);
			}
			{
				auto n_Read = n_JSON.add_namespace("Read"_s);
				n_Read
					.add_function_proxy<&stp<&JSON::Read::process>>("process"_s);
			}
		}
		// XML
		{
			auto n_XML = n_Core.add_namespace("XML"_s);
			define_generic_class<XML::Node>(n_XML, "Node"_s);
			{
				auto n_Write = n_XML.add_namespace("Write"_s);
				n_Write
					.add_function_proxy<&stp<&XML::Write::process>>("process"_s);
			}
			{
				auto n_Read = n_XML.add_namespace("Read"_s);
				n_Read
					.add_function_proxy<&stp<&XML::Read::process>>("process"_s);
			}
		}
		// Image
		{
			auto n_Image = n_Core.add_namespace("Image"_s);
			define_generic_class<Image::ImageSize>(n_Image, "ImageSize"_s);
			define_generic_class<Image::ImagePosition>(n_Image, "ImagePosition"_s);
			define_generic_class<Image::Pixel>(n_Image, "Pixel"_s);
			define_generic_class<
					Image::VBitmapView,
					GCDF::generic_mask
				>(n_Image, "BitmapView"_s)
				.add_member_function_proxy<&Image::VBitmapView::size>("size"_s)
				.add_member_function_proxy<&Image::VBitmapView::fill>("fill"_s)
				.add_member_function_proxy<&Image::VBitmapView::draw>("draw"_s)
				.add_member_function_proxy<&Image::VBitmapView::sub>("sub"_s);
			define_generic_class<
					Image::Bitmap,
					GCDF::generic_mask
				>(n_Image, "Bitmap"_s)
				.add_second_constructor_allocate_proxy<Image::ImageSize const &>("allocate"_s)
				.add_member_function_proxy<&Image::Bitmap::allocate>("allocate"_s)
				.add_member_function_proxy<&Image::Bitmap::reset>("reset"_s)
				.add_member_function_proxy<&Image::Bitmap::size>("size"_s)
				.add_member_function_proxy<AsVMemberFunction<Image::Bitmap, Image::VBitmapView>{&Image::Bitmap::view}>("view"_s);
			{
				auto n_File = n_Image.add_namespace("File"_s);
				{
					auto n_PNG = n_File.add_namespace("PNG"_s);
					n_PNG
						.add_function_proxy<&stp<&Image::File::PNG::size>>("size"_s)
						.add_function_proxy<&stp<&Image::File::PNG::read_of>>("read"_s)
						.add_function_proxy<&stp<&Image::File::PNG::write>>("write"_s)
						.add_function_proxy<&stp<&Image::File::PNG::read>>("read_of"_s)
						.add_function_proxy<&stp<&Image::File::PNG::size_file>>("size_file"_s)
						.add_function_proxy<&stp<&Image::File::PNG::read_file>>("read_file"_s)
						.add_function_proxy<&stp<&Image::File::PNG::write_file>>("write_file"_s)
						.add_function_proxy<&stp<&Image::File::PNG::read_file_of>>("read_file_of"_s);
				}
			}
		}
		// FileSystem
		{
			auto n_FileSystem = n_Core.add_namespace("FileSystem"_s);
			n_FileSystem
				// exist
				.add_function_proxy<&stp<&FileSystem::exist>>("exist"_s)
				.add_function_proxy<&stp<&FileSystem::exist_file>>("exist_file"_s)
				.add_function_proxy<&stp<&FileSystem::exist_directory>>("exist_directory"_s)
				// generic
				.add_function_proxy<&stp<&FileSystem::copy>>("copy"_s)
				.add_function_proxy<&stp<&FileSystem::rename>>("rename"_s)
				.add_function_proxy<&stp<&FileSystem::remove>>("remove"_s)
				// link
				.add_function_proxy<&stp<&FileSystem::create_link>>("create_link"_s)
				.add_function_proxy<&stp<&FileSystem::parse_link>>("parse_link"_s)
				// hard link
				.add_function_proxy<&stp<&FileSystem::create_hard_link>>("create_hard_link"_s)
				// file
				.add_function_proxy<&stp<&FileSystem::size_file>>("size_file"_s)
				.add_function_proxy<&stp<&FileSystem::resize_file>>("resize_file"_s)
				.add_function_proxy<&stp<&FileSystem::read_file>>("read_file"_s)
				.add_function_proxy<&stp<&FileSystem::write_file>>("write_file"_s)
				.add_function_proxy<&stp<&FileSystem::read_stream_file>>("read_stream_file"_s)
				.add_function_proxy<&stp<&FileSystem::write_stream_file>>("write_stream_file"_s)
				// directory
				.add_function_proxy<&stp<&FileSystem::create_directory>>("create_directory"_s)
				// iterate
				.add_function_proxy<&stp<&FileSystem::count>>("count"_s)
				.add_function_proxy<&stp<&FileSystem::count_file>>("count_file"_s)
				.add_function_proxy<&stp<&FileSystem::count_directory>>("count_directory"_s)
				.add_function_proxy<&stp<&FileSystem::list>>("list"_s)
				.add_function_proxy<&stp<&FileSystem::list_file>>("list_file"_s)
				.add_function_proxy<&stp<&FileSystem::list_directory>>("list_directory"_s)
				// special
				.add_function_proxy<&stp<&FileSystem::get_working_directory>>("get_working_directory"_s)
				.add_function_proxy<&stp<&FileSystem::set_working_directory>>("set_working_directory"_s)
				.add_function_proxy<&stp<&FileSystem::get_temporary_directory>>("get_temporary_directory"_s);
		}
		// System
		{
			auto n_System = n_Core.add_namespace("System"_s);
			n_System
				.add_function_proxy<&stp<&System::exit>>("exit"_s)
				.add_function_proxy<&stp<&System::sleep>>("sleep"_s)
				.add_function_proxy<&stp<&System::system>>("system"_s)
				.add_function_proxy<&stp<&System::process>>("process"_s);
		}
		// Tool
		{
			auto n_Tool = n_Core.add_namespace("Tool"_s);
			// Data
			{
				auto n_Data = n_Tool.add_namespace("Data"_s);
				{
					auto n_Hash = n_Data.add_namespace("Hash"_s);
					{
						auto n_FNV = n_Hash.add_namespace("FNV"_s);
						{
							auto n_Hash = n_FNV.add_namespace("Hash"_s);
							n_Hash
								.add_function_proxy<&stp<&Tool::Data::Hash::FNV::Hash::process_1_32>>("process_1_32"_s)
								.add_function_proxy<&stp<&Tool::Data::Hash::FNV::Hash::process_1_64>>("process_1_64"_s)
								.add_function_proxy<&stp<&Tool::Data::Hash::FNV::Hash::process_1a_32>>("process_1a_32"_s)
								.add_function_proxy<&stp<&Tool::Data::Hash::FNV::Hash::process_1a_64>>("process_1a_64"_s);
						}
					}
					{
						auto n_MD5 = n_Hash.add_namespace("MD5"_s);
						{
							auto n_Hash = n_MD5.add_namespace("Hash"_s);
							n_Hash
								.add_function_proxy<&stp<&Tool::Data::Hash::MD5::Hash::process>>("process"_s)
								.add_function_proxy<&stp<&Tool::Data::Hash::MD5::Hash::process_to_string>>("process_to_string"_s);
						}
					}
				}
				{
					auto n_Encode = n_Data.add_namespace("Encode"_s);
					{
						auto n_Base64 = n_Encode.add_namespace("Base64"_s);
						{
							auto n_Encode = n_Base64.add_namespace("Encode"_s);
							n_Encode
								.add_function_proxy<&stp<&Tool::Data::Encode::Base64::Encode::compute_size>>("compute_size"_s)
								.add_function_proxy<&stp<&Tool::Data::Encode::Base64::Encode::process>>("process"_s);
						}
						{
							auto n_Decode = n_Base64.add_namespace("Decode"_s);
							n_Decode
								.add_function_proxy<&stp<&Tool::Data::Encode::Base64::Decode::compute_size>>("compute_size"_s)
								.add_function_proxy<&stp<&Tool::Data::Encode::Base64::Decode::process>>("process"_s);
						}
					}
				}
				{
					auto n_Encrypt = n_Data.add_namespace("Encrypt"_s);
					{
						auto n_XOR = n_Encrypt.add_namespace("XOR"_s);
						{
							auto n_Crypt = n_XOR.add_namespace("Crypt"_s);
							n_Crypt
								.add_function_proxy<&stp<&Tool::Data::Encrypt::XOR::Crypt::process>>("process"_s);
						}
					}
					// todo
					/*{
						auto n_Rijndael = n_Encrypt.add_namespace("Rijndael"_s);
						{
							auto n_Encrypt = n_Rijndael.add_namespace("Encrypt"_s);
							n_Encrypt
								.add_function_proxy<&stp<&Tool::Data::Encrypt::Rijndael::Encrypt::process>>("process"_s);
						}
						{
							auto n_Decrypt = n_Rijndael.add_namespace("Decrypt"_s);
							n_Decrypt
								.add_function_proxy<&stp<&Tool::Data::Encrypt::Rijndael::Decrypt::process>>("process"_s);
						}
					}*/
				}
				{
					auto n_Compress = n_Data.add_namespace("Compress"_s);
					{
						auto n_Deflate = n_Compress.add_namespace("Deflate"_s);
						define_generic_class<Tool::Data::Compress::Deflate::Strategy>(n_Deflate, "Strategy"_s);
						define_generic_class<Tool::Data::Compress::Deflate::Wrapper>(n_Deflate, "Wrapper"_s);
						{
							auto n_Compress = n_Deflate.add_namespace("Compress"_s);
							n_Compress
								.add_function_proxy<&stp<&Tool::Data::Compress::Deflate::Compress::compute_size_bound>>("compute_size_bound"_s)
								.add_function_proxy<&stp<&Tool::Data::Compress::Deflate::Compress::process>>("process"_s);
						}
						{
							auto n_Uncompress = n_Deflate.add_namespace("Uncompress"_s);
							n_Uncompress
								.add_function_proxy<&stp<&Tool::Data::Compress::Deflate::Uncompress::process>>("process"_s);
						}
					}
					{
						auto n_BZip2 = n_Compress.add_namespace("BZip2"_s);
						{
							auto n_Compress = n_BZip2.add_namespace("Compress"_s);
							n_Compress
								.add_function_proxy<&stp<&Tool::Data::Compress::BZip2::Compress::process>>("process"_s);
						}
						{
							auto n_Uncompress = n_BZip2.add_namespace("Uncompress"_s);
							n_Uncompress
								.add_function_proxy<&stp<&Tool::Data::Compress::BZip2::Uncompress::process>>("process"_s);
						}
					}
					{
						auto n_Lzma = n_Compress.add_namespace("Lzma"_s);
						{
							auto n_Compress = n_Lzma.add_namespace("Compress"_s);
							n_Compress
								.add_function_proxy<&stp<&Tool::Data::Compress::Lzma::Compress::process>>("process"_s);
						}
						{
							auto n_Uncompress = n_Lzma.add_namespace("Uncompress"_s);
							n_Uncompress
								.add_function_proxy<&stp<&Tool::Data::Compress::Lzma::Uncompress::process>>("process"_s);
						}
					}
				}
			}
			// Texture
			{
				auto n_Texture = n_Tool.add_namespace("Texture"_s);
				define_generic_class<Tool::Texture::Format>(n_Texture, "Format"_s);
				{
					auto n_Encode = n_Texture.add_namespace("Encode"_s);
					n_Encode
						.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Image::CBitmapView const &, Tool::Texture::Format const &>{&Tool::Texture::Encode::process}>>("process"_s);
				}
				{
					auto n_Decode = n_Texture.add_namespace("Decode"_s);
					n_Decode
						.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Image::VBitmapView const &, Tool::Texture::Format const &>{&Tool::Texture::Decode::process}>>("process"_s);
				}
				{
					auto n_Compress = n_Texture.add_namespace("Compress"_s);
					{
						auto n_ETC1 = n_Compress.add_namespace("ETC1"_s);
						{
							auto n_Compress = n_ETC1.add_namespace("Compress"_s);
							n_Compress
								.add_function_proxy<&stp<&Tool::Texture::Compress::ETC1::Compress::process>>("process"_s);
						}
						{
							auto n_Uncompress = n_ETC1.add_namespace("Uncompress"_s);
							n_Uncompress
								.add_function_proxy<&stp<&Tool::Texture::Compress::ETC1::Uncompress::process>>("process"_s);
						}
					}
					{
						auto n_PVRTC4 = n_Compress.add_namespace("PVRTC4"_s);
						{
							auto n_Compress = n_PVRTC4.add_namespace("Compress"_s);
							n_Compress
								.add_function_proxy<&stp<&Tool::Texture::Compress::PVRTC4::Compress::process>>("process"_s);
						}
						{
							auto n_Uncompress = n_PVRTC4.add_namespace("Uncompress"_s);
							n_Uncompress
								.add_function_proxy<&stp<&Tool::Texture::Compress::PVRTC4::Uncompress::process>>("process"_s);
						}
					}
				}
			}
			// Wwise
			{
				auto n_Wwise = n_Tool.add_namespace("Wwise"_s);
				{
					auto n_EncodedMedia = n_Wwise.add_namespace("EncodedMedia"_s);
					{
						auto n_Decode = n_EncodedMedia.add_namespace("Decode"_s);
						n_Decode
							.add_function_proxy<&stp<&Tool::Wwise::EncodedMedia::Decode::process>>("process"_s);
					}
				}
				{
					auto n_SoundBank = n_Wwise.add_namespace("SoundBank"_s);
					auto c_Version = define_generic_class<
						Tool::Wwise::SoundBank::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_SoundBank, "Version"_s);
					{
						auto n_Manifest = n_SoundBank.add_namespace("Manifest"_s);
						auto c_SoundBankVariant = define_generic_class<
							Tool::Wwise::SoundBank::Manifest::SoundBankVariant,
							GCDF::generic_mask
						>(n_Manifest, "SoundBankVariant"_s);
						define_variant_class_version_method<Tool::Wwise::SoundBank::Version, Tool::Wwise::SoundBank::VersionEnum>(c_SoundBankVariant);
					}
					{
						auto n_Pack = n_SoundBank.add_namespace("Pack"_s);
						n_Pack
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::Wwise::SoundBank::Manifest::SoundBankVariant const &, Path const &, Tool::Wwise::SoundBank::Version const &>{&Tool::Wwise::SoundBank::Pack::process}>>("process"_s);
					}
					{
						auto n_Unpack = n_SoundBank.add_namespace("Unpack"_s);
						n_Unpack
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::Wwise::SoundBank::Manifest::SoundBankVariant &, Optional<Path> const &, Tool::Wwise::SoundBank::Version const &>{&Tool::Wwise::SoundBank::Unpack::process}>>("process"_s);
					}
				}
			}
			// Marmalade
			{
				auto n_Marmalade = n_Tool.add_namespace("Marmalade"_s);
				{
					auto n_DZip = n_Marmalade.add_namespace("DZip"_s);
					auto c_Version = define_generic_class<
						Tool::Marmalade::DZip::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_DZip, "Version"_s);
					{
						auto n_Manifest = n_DZip.add_namespace("Manifest"_s);
						auto c_PackageVariant = define_generic_class<
							Tool::Marmalade::DZip::Manifest::PackageVariant,
							GCDF::generic_mask
						>(n_Manifest, "PackageVariant"_s);
						define_variant_class_version_method<Tool::Marmalade::DZip::Version, Tool::Marmalade::DZip::VersionEnum>(c_PackageVariant);
					}
					{
						auto n_Pack = n_DZip.add_namespace("Pack"_s);
						n_Pack
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::Marmalade::DZip::Manifest::PackageVariant const &, Path const &, Tool::Marmalade::DZip::Version const &>{&Tool::Marmalade::DZip::Pack::process}>>("process"_s);
					}
					{
						auto n_Unpack = n_DZip.add_namespace("Unpack"_s);
						n_Unpack
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::Marmalade::DZip::Manifest::PackageVariant &, Optional<Path> const &, Tool::Marmalade::DZip::Version const &>{&Tool::Marmalade::DZip::Unpack::process}>>("process"_s);
					}
				}
			}
			// PopCap
			{
				auto n_PopCap = n_Tool.add_namespace("PopCap"_s);
				{
					auto n_ZLib = n_PopCap.add_namespace("ZLib"_s);
					{
						auto n_Compress = n_ZLib.add_namespace("Compress"_s);
						n_Compress
							.add_function_proxy<&stp<&Tool::PopCap::ZLib::Compress::compute_size_bound>>("compute_size_bound"_s)
							.add_function_proxy<&stp<&Tool::PopCap::ZLib::Compress::process>>("process"_s);
					}
					{
						auto n_Uncompress = n_ZLib.add_namespace("Uncompress"_s);
						n_Uncompress
							.add_function_proxy<&stp<&Tool::PopCap::ZLib::Uncompress::compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::PopCap::ZLib::Uncompress::process>>("process"_s);
					}
				}
				{
					auto n_Reanim = n_PopCap.add_namespace("Reanim"_s);
					auto c_Version = define_generic_class<
						Tool::PopCap::Reanim::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_Reanim, "Version"_s);
					{
						auto n_Manifest = n_Reanim.add_namespace("Manifest"_s);
						auto c_AnimationVariant = define_generic_class<
							Tool::PopCap::Reanim::Manifest::AnimationVariant,
							GCDF::generic_mask
						>(n_Manifest, "AnimationVariant"_s);
						define_variant_class_version_method<Tool::PopCap::Reanim::Version, Tool::PopCap::Reanim::VersionEnum>(c_AnimationVariant);
					}
					{
						auto n_Encode = n_Reanim.add_namespace("Encode"_s);
						n_Encode
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::PopCap::Reanim::Manifest::AnimationVariant const &, Tool::PopCap::Reanim::Version const &>{&Tool::PopCap::Reanim::Encode::process}>>("process"_s);
					}
					{
						auto n_Decode = n_Reanim.add_namespace("Decode"_s);
						n_Decode
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::PopCap::Reanim::Manifest::AnimationVariant &, Tool::PopCap::Reanim::Version const &>{&Tool::PopCap::Reanim::Decode::process}>>("process"_s);
					}
				}
				{
					auto n_RTON = n_PopCap.add_namespace("RTON"_s);
					auto c_Version = define_generic_class<
						Tool::PopCap::RTON::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_RTON, "Version"_s);
					{
						auto n_Encode = n_RTON.add_namespace("Encode"_s);
						n_Encode
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, JSON::Value const &, Boolean const &, Boolean const &, Tool::PopCap::RTON::Version const &>{&Tool::PopCap::RTON::Encode::process}>>("process"_s);
					}
					{
						auto n_Decode = n_RTON.add_namespace("Decode"_s);
						n_Decode
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, JSON::Value &, Tool::PopCap::RTON::Version const &>{&Tool::PopCap::RTON::Decode::process}>>("process"_s);
					}
					{
						auto n_Encrypt = n_RTON.add_namespace("Encrypt"_s);
						n_Encrypt
							.add_function_proxy<&stp<&Tool::PopCap::RTON::Encrypt::compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::PopCap::RTON::Encrypt::process>>("process"_s);
					}
					{
						auto n_Decrypt = n_RTON.add_namespace("Decrypt"_s);
						n_Decrypt
							.add_function_proxy<&stp<&Tool::PopCap::RTON::Decrypt::compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::PopCap::RTON::Decrypt::process>>("process"_s);
					}
				}
				{
					auto n_PAM = n_PopCap.add_namespace("PAM"_s);
					auto c_Version = define_generic_class<
						Tool::PopCap::PAM::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_PAM, "Version"_s);
					{
						auto n_Manifest = n_PAM.add_namespace("Manifest"_s);
						auto c_AnimationVariant = define_generic_class<
							Tool::PopCap::PAM::Manifest::AnimationVariant,
							GCDF::generic_mask
						>(n_Manifest, "AnimationVariant"_s);
						define_variant_class_version_method<Tool::PopCap::PAM::Version, Tool::PopCap::PAM::VersionEnum>(c_AnimationVariant);
					}
					{
						auto n_Encode = n_PAM.add_namespace("Encode"_s);
						n_Encode
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::PopCap::PAM::Manifest::AnimationVariant const &, Tool::PopCap::PAM::Version const &>{&Tool::PopCap::PAM::Encode::process}>>("process"_s);
					}
					{
						auto n_Decode = n_PAM.add_namespace("Decode"_s);
						n_Decode
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::PopCap::PAM::Manifest::AnimationVariant &, Tool::PopCap::PAM::Version const &>{&Tool::PopCap::PAM::Decode::process}>>("process"_s);
					}
				}
				{
					auto n_PAK = n_PopCap.add_namespace("PAK"_s);
					auto c_Version = define_generic_class<
						Tool::PopCap::PAK::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_PAK, "Version"_s);
					{
						auto n_Manifest = n_PAK.add_namespace("Manifest"_s);
						auto c_PackageVariant = define_generic_class<
							Tool::PopCap::PAK::Manifest::PackageVariant,
							GCDF::generic_mask
						>(n_Manifest, "PackageVariant"_s);
						define_variant_class_version_method<Tool::PopCap::PAK::Version, Tool::PopCap::PAK::VersionEnum>(c_PackageVariant);
					}
					{
						auto n_Pack = n_PAK.add_namespace("Pack"_s);
						n_Pack
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::PopCap::PAK::Manifest::PackageVariant const &, Path const &, Tool::PopCap::PAK::Version const &>{&Tool::PopCap::PAK::Pack::process}>>("process"_s);
					}
					{
						auto n_Unpack = n_PAK.add_namespace("Unpack"_s);
						n_Unpack
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::PopCap::PAK::Manifest::PackageVariant &, Optional<Path> const &, Tool::PopCap::PAK::Version const &>{&Tool::PopCap::PAK::Unpack::process}>>("process"_s);
					}
				}
				{
					auto n_RSGP = n_PopCap.add_namespace("RSGP"_s);
					auto c_Version = define_generic_class<
						Tool::PopCap::RSGP::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_RSGP, "Version"_s);
					{
						auto n_Manifest = n_RSGP.add_namespace("Manifest"_s);
						auto PackageVariant = define_generic_class<
							Tool::PopCap::RSGP::Manifest::PackageVariant,
							GCDF::generic_mask
						>(n_Manifest, "PackageVariant"_s);
						define_variant_class_version_method<Tool::PopCap::RSGP::Version, Tool::PopCap::RSGP::VersionEnum>(PackageVariant);
					}
					{
						auto n_Pack = n_RSGP.add_namespace("Pack"_s);
						n_Pack
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::PopCap::RSGP::Manifest::PackageVariant const &, Path const &, Tool::PopCap::RSGP::Version const &>{&Tool::PopCap::RSGP::Pack::process}>>("process"_s);
					}
					{
						auto n_Unpack = n_RSGP.add_namespace("Unpack"_s);
						n_Unpack
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::PopCap::RSGP::Manifest::PackageVariant &, Optional<Path> const &, Tool::PopCap::RSGP::Version const &>{&Tool::PopCap::RSGP::Unpack::process}>>("process"_s);
					}
				}
				{
					auto n_RSB = n_PopCap.add_namespace("RSB"_s);
					auto c_Version = define_generic_class<
						Tool::PopCap::RSB::Version,
						GCDF::generic_mask | GCDF::json_mask
					>(n_RSB, "Version"_s);
					{
						auto n_Manifest = n_RSB.add_namespace("Manifest"_s);
						auto PackageVariant = define_generic_class<
							Tool::PopCap::RSB::Manifest::PackageVariant,
							GCDF::generic_mask
						>(n_Manifest, "PackageVariant"_s);
						define_variant_class_version_method<Tool::PopCap::RSB::Version, Tool::PopCap::RSB::VersionEnum>(PackageVariant);
					}
					{
						auto n_Description = n_RSB.add_namespace("Description"_s);
						auto c_PackageOptionalVariant = define_generic_class<
							Tool::PopCap::RSB::Description::PackageOptionalVariant,
							GCDF::generic_mask
						>(n_Description, "PackageOptionalVariant"_s);
						define_variant_class_version_method<Tool::PopCap::RSB::Version, Tool::PopCap::RSB::VersionEnum>(c_PackageOptionalVariant);
					}
					{
						auto n_Pack = n_RSB.add_namespace("Pack"_s);
						n_Pack
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Tool::PopCap::RSB::Manifest::PackageVariant const &, Tool::PopCap::RSB::Description::PackageOptionalVariant const &, Path const &, Optional<Path> const &, Optional<Path> const &, Tool::PopCap::RSB::Version const &>{&Tool::PopCap::RSB::Pack::process}>>("process"_s);
					}
					{
						auto n_Unpack = n_RSB.add_namespace("Unpack"_s);
						n_Unpack
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Tool::PopCap::RSB::Manifest::PackageVariant &, Tool::PopCap::RSB::Description::PackageOptionalVariant &, Optional<Path> const &, Optional<Path> const &, Tool::PopCap::RSB::Version const &>{&Tool::PopCap::RSB::Unpack::process}>>("process"_s);
					}
				}
			}
			// Misc
			{
				auto n_Misc = n_Tool.add_namespace("Misc"_s);
				{
					auto n_PvZ1RSBTexture20SeriesLayout = n_Misc.add_namespace("PvZ1RSBTexture20SeriesLayout"_s);
					{
						auto n_Encode = n_PvZ1RSBTexture20SeriesLayout.add_namespace("Encode"_s);
						n_Encode
							.add_function_proxy<&stp<AsGlobalFunction<Void, OByteStreamView &, Image::CBitmapView const &, Tool::Texture::Format const &>{&Tool::Other::PvZ1RSBTexture20SeriesLayout::Encode::process}>>("process"_s);
					}
					{
						auto n_Decode = n_PvZ1RSBTexture20SeriesLayout.add_namespace("Decode"_s);
						n_Decode
							.add_function_proxy<&stp<AsGlobalFunction<Void, IByteStreamView &, Image::VBitmapView const &, Tool::Texture::Format const &>{&Tool::Other::PvZ1RSBTexture20SeriesLayout::Decode::process}>>("process"_s);
					}
				}
				{
					// todo
					auto n_PvZ2CHSRSBTextureAlphaIndex = n_Misc.add_namespace("PvZ2CHSRSBTextureAlphaIndex"_s);
					{
						auto n_Encode = n_PvZ2CHSRSBTextureAlphaIndex.add_namespace("Encode"_s);
						n_Encode
							.add_function<
								&normalized_lambda<
									[] (
									JS::Handler<IOByteStreamView> &   data,
									JS::Handler<Image::VBitmapView> & image,
									JS::Value &                       map_js
								) -> Void {
										auto map = map_js.to_of<List<Image::Channel>>();
										Tool::Other::PvZ2CHSRSBTextureAlphaIndex::Encode::process(data.value(), image.value(), map);
										return;
									}
								>
							>("process"_s);
					}
					{
						auto n_Decode = n_PvZ2CHSRSBTextureAlphaIndex.add_namespace("Decode"_s);
						n_Decode
							.add_function<
								&normalized_lambda<
									[] (
									JS::Handler<IOByteStreamView> &   data,
									JS::Handler<Image::VBitmapView> & image,
									JS::Value &                       map_js
								) -> Void {
										auto map = map_js.to_of<List<Image::Channel>>();
										Tool::Other::PvZ2CHSRSBTextureAlphaIndex::Decode::process(data.value(), image.value(), map);
										return;
									}
								>
							>("process"_s);
					}
				}
			}
		}
		// Misc
		{
			auto n_Misc = n_Core.add_namespace("Misc"_s);
			define_generic_class<Thread, GCDF::default_constructor>(n_Misc, "Thread"_s);
			define_generic_class<Context, GCDF::none_mask>(n_Misc, "Context"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> &            thix,
						JS::Handler<VCharacterListView> & script,
						JS::Handler<String> &             name
					) -> JS::Value {
							return thix.value().evaluate(down_cast<VStringView>(script.value()), name.value());
						}
					>
				>("evaluate"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> &      thix,
						JS::Handler<List<String>> & argument
					) -> JS::Handler<List<String>> {
							return JS::Handler<List<String>>::new_reference(as_variable(thix.value().shell_callback(argument.value())));
						}
					>
				>("shell_callback"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> & thix
					) -> JS::Handler<Context> {
							return JS::Handler<Context>::new_instance_allocate(thix.value().spawn());
						}
					>
				>("spawn"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> & thix
					) -> Void {
							return thix.value().yield();
						}
					>
				>("yield"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> & thix,
						JS::Value &            executor,
						JS::Handler<Thread> &  thread
					) -> Void {
							return thix.value().execute(executor, thread.value());
						}
					>
				>("execute"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> & thix
					) -> JS::Handler<Boolean> {
							return JS::Handler<Boolean>::new_instance_allocate(thix.value().state());
						}
					>
				>("state"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JS::Handler<Context> & thix
					) -> JS::Value {
							return thix.value().result();
						}
					>
				>("result"_s);
			n_Misc.add_variable("g_context"_s, context.context().new_value(JS::Handler<Context>::new_reference(context)));
			n_Misc.add_function_proxy<
				&normalized_lambda<
					[] (
					VByteListView & it
				) -> VCharacterListView {
						return self_cast<VCharacterListView>(it);
					}
				>
			>("cast_ByteListView_to_CharacterListView"_s);
			n_Misc.add_function_proxy<
				&normalized_lambda<
					[] (
					VCharacterListView & it
				) -> VByteListView {
						return self_cast<VByteListView>(it);
					}
				>
			>("cast_CharacterListView_to_ByteListView"_s);
			n_Misc.add_function_proxy<
				&normalized_lambda<
					[] (
					String & it
				) -> VCharacterListView {
						return up_cast<VCharacterListView>(it.view());
					}
				>
			>("cast_String_to_CharacterListView"_s);
			n_Misc.add_function_proxy<
				&normalized_lambda<
					[] (
					String & it
				) -> ByteArray {
						auto result = ByteArray{};
						result.bind(to_byte_view(it.view()));
						it.unbind();
						return result;
					}
				>
			>("cast_moveable_String_to_ByteArray"_s);
			n_Misc.add_function<
				&normalized_lambda<
					[] (
					JS::Handler<VCharacterListView> & it
				) -> VStringView& {
						// NOTE : return StringView is cheap
						return down_cast<VStringView>(it.value());
					}
				>
			>("cast_CharacterListView_to_JS_String"_s);
			return;
			#if defined M_compiler_msvc
			#pragma warning(pop)
			#endif
			#if defined M_compiler_clang
			#pragma clang diagnostic pop
			#endif
		}
	}

	#pragma endregion

}
