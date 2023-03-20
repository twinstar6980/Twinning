#pragma once

#include "core/utility/utility.hpp"
#include "core/executor/context.hpp"
#include "core/tool/data/hash/fnv.hpp"
#include "core/tool/data/hash/md5.hpp"
#include "core/tool/data/encoding/base64.hpp"
#include "core/tool/data/encryption/xor.hpp"
#include "core/tool/data/encryption/rijndael.hpp"
#include "core/tool/data/compression/deflate.hpp"
#include "core/tool/data/compression/bzip2.hpp"
#include "core/tool/data/compression/lzma.hpp"
#include "core/tool/data/differentiation/vcdiff.hpp"
#include "core/tool/data/serialization/json.hpp"
#include "core/tool/data/serialization/xml.hpp"
#include "core/tool/image/transformation/transform.hpp"
#include "core/tool/image/texture/encode.hpp"
#include "core/tool/image/texture/compression/etc1.hpp"
#include "core/tool/image/texture/compression/etc2.hpp"
#include "core/tool/image/texture/compression/pvrtc4.hpp"
#include "core/tool/image/file/png.hpp"
#include "core/tool/wwise/media/encode.hpp"
#include "core/tool/wwise/sound_bank/encode.hpp"
#include "core/tool/marmalade/dzip/pack.hpp"
#include "core/tool/popcap/zlib/compress.hpp"
#include "core/tool/popcap/crypt_data/encrypt.hpp"
#include "core/tool/popcap/reflection_object_notation/encode.hpp"
#include "core/tool/popcap/reflection_object_notation/encrypt.hpp"
#include "core/tool/popcap/u_texture/encode.hpp"
#include "core/tool/popcap/sexy_texture/encode.hpp"
#include "core/tool/popcap/animation/encode.hpp"
#include "core/tool/popcap/re_animation/encode.hpp"
#include "core/tool/popcap/particle/encode.hpp"
#include "core/tool/popcap/trail/encode.hpp"
#include "core/tool/popcap/effect/encode.hpp"
#include "core/tool/popcap/character_font_widget_2/encode.hpp"
#include "core/tool/popcap/package/pack.hpp"
#include "core/tool/popcap/resource_stream_group/pack.hpp"
#include "core/tool/popcap/resource_stream_bundle/pack.hpp"
#include "core/tool/popcap/resource_stream_bundle_patch/encode.hpp"
#include "core/tool/miscellaneous/xbox_tiled_texture/encode.hpp"
#include "core/tool/miscellaneous/pvz2_chinese_android_alpha_palette_texture/encode.hpp"

namespace TwinStar::Core::Executor::Interface {

	#pragma region define generic class

	enum class GenericClassDefinitionFlag : ZIntegerU8 {
		// generic operation
		// - static default(): T;
		default_constructor = 1 << 0,
		// - static copy(it: T): T;
		copy_constructor = 1 << 1,
		// value operation
		// - static value(it: typeof T.Value) : T;
		value_constructor = 1 << 2,
		// - get value(): typeof T.Value;
		value_getter = 1 << 3,
		// - set value(it: typeof T.Value);
		value_setter = 1 << 4,
		// mask
		none_mask    = 0b000'00,
		all_mask     = 0b111'11,
		generic_mask = default_constructor | copy_constructor,
		value_mask   = value_constructor | value_getter | value_setter,
		default_mask = generic_mask | value_mask,
	};

	// NOTE : alias
	using GCDF = GenericClassDefinitionFlag;

	// ----------------

	inline constexpr auto operator | (
		GCDF const & thix,
		GCDF const & that
	) -> GCDF {
		return static_cast<GCDF>(static_cast<std::underlying_type_t<GCDF>>(thix) | static_cast<std::underlying_type_t<GCDF>>(that));
	}

	inline constexpr auto operator & (
		GCDF const & thix,
		GCDF const & that
	) -> GCDF {
		return static_cast<GCDF>(static_cast<std::underlying_type_t<GCDF>>(thix) & static_cast<std::underlying_type_t<GCDF>>(that));
	}

	inline constexpr auto operator ~ (
		GCDF const & thix
	) -> GCDF {
		return static_cast<GCDF>(~static_cast<std::underlying_type_t<GCDF>>(thix));
	}

	inline constexpr auto operator * (
		GCDF const & thix,
		GCDF const & that
	) -> Boolean {
		return mbw<Boolean>(static_cast<std::underlying_type_t<GCDF>>(thix) & static_cast<std::underlying_type_t<GCDF>>(that));
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
		return builder;
	}

	// json operation
	// - static json(json: JSON.Value, version: Version): T;
	// - get_json(version: Version): JSON.Value;
	// - set_json(version: Version, value: JSON.Value);
	template <typename Version, typename VersionPackage, typename Class> requires
		CategoryConstraint<IsPureInstance<Version> && IsPureInstance<VersionPackage> && IsPureInstance<Class>>
	inline auto define_variant_class_version_method (
		JS::ClassBuilder<Class> & builder
	) -> JS::ClassBuilder<Class>& {
		constexpr auto & json_constructor = JS::proxy_function_by_handler<
			&normalized_lambda<
				[] (
				JSON::Value const & json,
				Version const &     version
			) -> Class {
					auto it = Class{};
					Generalization::match<VersionPackage>(
						version,
						[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
							json.to(it, mbw<Size>(index));
						}
					);
					return it;
				}
			>
		>;
		constexpr auto & json_getter = JS::proxy_function_by_handler<
			&normalized_lambda<
				[] (
				Class &         thix,
				Version const & version
			) -> JSON::Value {
					auto json = JSON::Value{};
					Generalization::match<VersionPackage>(
						version,
						[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
							json.from(thix, mbw<Size>(index));
						}
					);
					return json;
				}
			>
		>;
		constexpr auto & json_setter = JS::proxy_function_by_handler<
			&normalized_lambda<
				[] (
				Class &             thix,
				Version const &     version,
				JSON::Value const & json
			) -> Void {
					Generalization::match<VersionPackage>(
						version,
						[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
							json.to(thix, mbw<Size>(index));
						}
					);
					return;
				}
			>
		>;
		builder.template add_second_constructor<&json_constructor>("json"_s);
		builder.template add_member_function<&json_getter>("get_json"_s);
		builder.template add_member_function<&json_setter>("set_json"_s);
		return builder;
	}

	#pragma endregion

	#pragma region proxy function by special type promotion

	// NOTE
	// 为了简化接口，对部分类型进行提升
	// (V|C)StringView          -> String
	// (C)ByteListView          -> VByteListView            : ByteListView
	// (I|O)ByteStreamView      -> IOByteStreamView         : ByteStreamView
	// (C)CharacterListView     -> VCharacterListView       : CharacterListView
	// (I|O)CharacterStreamView -> IOCharacterStreamView    : CharacterStreamView
	// Image::(C)ImageView      -> Image::VImageView        : Image::ImageView

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
								IsSame<Type, Image::CImageView>,
								Image::VImageView,
								Type
							>
						>
					>
				>
			>
		>;

		template <auto function, auto ... index> requires
			NoneConstraint
		inline constexpr auto make_proxy_function_by_special_type_promotion (
			ValuePackage<index ...>
		) -> auto {
			if constexpr ((IsSame<AsPromotion<AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>>, AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>> && ...)) {
				return function;
			} else {
				return &proxy_global_function<function, AsPromotion<AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>> & ...>;
			}
		}

	}

	// ----------------

	template <auto function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
	inline constexpr auto & proxy_function_by_special_type_promotion = *Detail::make_proxy_function_by_special_type_promotion<function>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

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
		auto n_TwinStar = JS::NamespaceBuilder{context.context(), as_lvalue(context.context().global_object()), k_null_optional, "TwinStar"_s};
		auto n_Core = n_TwinStar.add_namespace("Core"_s);
		// Boolean
		define_generic_class<Boolean>(n_Core, "Boolean"_s);
		// Number
		define_generic_class<IntegerU32>(n_Core, "IntegerU32"_s);
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
		define_generic_class<IOByteStreamView, GCDF::generic_mask>(n_Core, "ByteStreamView"_s)
			.add_second_constructor_allocate_proxy<VByteListView const &>("watch"_s)
			.add_member_function_proxy<&IOByteStreamView::size>("size"_s)
			.add_member_function_proxy<&IOByteStreamView::position>("position"_s)
			.add_member_function_proxy<&IOByteStreamView::set_position>("set_position"_s)
			.add_member_function_proxy<&IOByteStreamView::view>("view"_s)
			.add_member_function_proxy<&IOByteStreamView::stream_view>("stream_view"_s);
		// CharacterListView
		define_generic_class<VCharacterListView, GCDF::generic_mask>(n_Core, "CharacterListView"_s)
			.add_member_function_proxy<&VCharacterListView::size>("size"_s)
			.add_member_function_proxy<AsCMemberFunction<VCharacterListView, VCharacterListView, Size const &, Size const &>{&VCharacterListView::sub}>("sub"_s);
		// CharacterStreamView
		define_generic_class<IOCharacterStreamView, GCDF::generic_mask>(n_Core, "CharacterStreamView"_s)
			.add_second_constructor_allocate_proxy<VCharacterListView const &>("watch"_s)
			.add_member_function_proxy<&IOCharacterStreamView::size>("size"_s)
			.add_member_function_proxy<&IOCharacterStreamView::position>("position"_s)
			.add_member_function_proxy<&IOCharacterStreamView::set_position>("set_position"_s)
			.add_member_function_proxy<&IOCharacterStreamView::view>("view"_s)
			.add_member_function_proxy<&IOCharacterStreamView::stream_view>("stream_view"_s);
		// JSON
		{
			auto n_JSON = n_Core.add_namespace("JSON"_s);
			define_generic_class<JSON::Value>(n_JSON, "Value"_s);
		}
		// XML
		{
			auto n_XML = n_Core.add_namespace("XML"_s);
			define_generic_class<XML::Node>(n_XML, "Node"_s);
		}
		// Image
		{
			auto n_Image = n_Core.add_namespace("Image"_s);
			define_generic_class<Image::ImageSize>(n_Image, "ImageSize"_s);
			define_generic_class<Image::ImagePosition>(n_Image, "ImagePosition"_s);
			define_generic_class<Image::Color>(n_Image, "Color"_s);
			define_generic_class<List<Image::Color>>(n_Image, "ColorList"_s);
			define_generic_class<Image::Pixel>(n_Image, "Pixel"_s);
			define_generic_class<Image::VImageView, GCDF::generic_mask>(n_Image, "ImageView"_s)
				.add_member_function_proxy<&Image::VImageView::size>("size"_s)
				.add_member_function_proxy<&Image::VImageView::fill>("fill"_s)
				.add_member_function_proxy<&Image::VImageView::draw>("draw"_s)
				.add_member_function_proxy<&Image::VImageView::sub>("sub"_s);
			define_generic_class<Image::Image, GCDF::generic_mask>(n_Image, "Image"_s)
				.add_second_constructor_allocate_proxy<Image::ImageSize const &>("allocate"_s)
				.add_member_function_proxy<&Image::Image::allocate>("allocate"_s)
				.add_member_function_proxy<&Image::Image::reset>("reset"_s)
				.add_member_function_proxy<&Image::Image::size>("size"_s)
				.add_member_function_proxy<AsVMemberFunction<Image::Image, Image::VImageView>{&Image::Image::view}>("view"_s);
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
				.add_function_proxy<&stp<&FileSystem::create_file>>("create_file"_s)
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
		// Process
		{
			auto n_Process = n_Core.add_namespace("Process"_s);
			n_Process
				.add_function_proxy<&stp<&Process::environment>>("environment"_s)
				.add_function_proxy<&stp<&Process::exit>>("exit"_s)
				.add_function_proxy<&stp<&Process::execute>>("execute"_s)
				.add_function_proxy<&stp<&Process::system>>("system"_s);
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
						define_generic_class<Tool::Data::Hash::FNV::Mode>(n_FNV, "Mode"_s);
						define_generic_class<Tool::Data::Hash::FNV::BitCount>(n_FNV, "BitCount"_s);
						n_FNV.add_namespace("Hash"_s)
							.add_function_proxy<&stp<Tool::Data::Hash::FNV::Hash::do_process_whole>>("process_whole"_s);
					}
					{
						auto n_MD5 = n_Hash.add_namespace("MD5"_s);
						n_MD5.add_namespace("Hash"_s)
							.add_function_proxy<&stp<&Tool::Data::Hash::MD5::Hash::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto n_Encoding = n_Data.add_namespace("Encoding"_s);
					{
						auto n_Base64 = n_Encoding.add_namespace("Base64"_s);
						n_Base64.add_namespace("Encode"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Encode::do_compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Encode::do_process_whole>>("process_whole"_s);
						n_Base64.add_namespace("Decode"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Decode::do_compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Decode::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto n_Encryption = n_Data.add_namespace("Encryption"_s);
					{
						auto n_XOR = n_Encryption.add_namespace("XOR"_s);
						n_XOR.add_namespace("Encrypt"_s)
							.add_function_proxy<&stp<&Tool::Data::Encryption::XOR::Encrypt::do_process_whole>>("process_whole"_s);
					}
					{
						auto n_Rijndael = n_Encryption.add_namespace("Rijndael"_s);
						define_generic_class<Tool::Data::Encryption::Rijndael::Mode>(n_Rijndael, "Mode"_s);
						n_Rijndael.add_namespace("Encrypt"_s)
							.add_function_proxy<&stp<&Tool::Data::Encryption::Rijndael::Encrypt::do_process_whole>>("process_whole"_s);
						n_Rijndael.add_namespace("Decrypt"_s)
							.add_function_proxy<&stp<&Tool::Data::Encryption::Rijndael::Decrypt::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto n_Compression = n_Data.add_namespace("Compression"_s);
					{
						auto n_Deflate = n_Compression.add_namespace("Deflate"_s);
						define_generic_class<Tool::Data::Compression::Deflate::Strategy>(n_Deflate, "Strategy"_s);
						define_generic_class<Tool::Data::Compression::Deflate::Wrapper>(n_Deflate, "Wrapper"_s);
						n_Deflate.add_namespace("Compress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Deflate::Compress::do_compute_size_bound>>("compute_size_bound"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Deflate::Compress::do_process_whole>>("process_whole"_s);
						n_Deflate.add_namespace("Uncompress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Deflate::Uncompress::do_process_whole>>("process_whole"_s);
					}
					{
						auto n_BZip2 = n_Compression.add_namespace("BZip2"_s);
						n_BZip2.add_namespace("Compress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::BZip2::Compress::do_process_whole>>("process_whole"_s);
						n_BZip2.add_namespace("Uncompress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::BZip2::Uncompress::do_process_whole>>("process_whole"_s);
					}
					{
						auto n_Lzma = n_Compression.add_namespace("Lzma"_s);
						n_Lzma.add_namespace("Compress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Lzma::Compress::do_process_whole>>("process_whole"_s);
						n_Lzma.add_namespace("Uncompress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Lzma::Uncompress::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto n_Differentiation = n_Data.add_namespace("Differentiation"_s);
					{
						auto n_VCDiff = n_Differentiation.add_namespace("VCDiff"_s);
						n_VCDiff.add_namespace("Encode"_s)
							.add_function_proxy<&stp<&Tool::Data::Differentiation::VCDiff::Encode::do_process_whole>>("process_whole"_s);
						n_VCDiff.add_namespace("Decode"_s)
							.add_function_proxy<&stp<&Tool::Data::Differentiation::VCDiff::Decode::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto n_Serialization = n_Data.add_namespace("Serialization"_s);
					{
						auto n_JSON = n_Serialization.add_namespace("JSON"_s);
						n_JSON.add_namespace("Write"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::JSON::Write::do_process_whole>>("process_whole"_s);
						n_JSON.add_namespace("Read"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::JSON::Read::do_process_whole>>("process_whole"_s);
					}
					{
						auto n_XML = n_Serialization.add_namespace("XML"_s);
						n_XML.add_namespace("Write"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::XML::Write::do_process_whole>>("process_whole"_s);
						n_XML.add_namespace("Read"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::XML::Read::do_process_whole>>("process_whole"_s);
					}
				}
			}
			// Image
			{
				auto n_Image = n_Tool.add_namespace("Image"_s);
				{
					auto n_Transformation = n_Image.add_namespace("Transformation"_s);
					n_Transformation.add_namespace("Flip"_s)
						.add_function_proxy<&stp<&Tool::Image::Transformation::Flip::do_process_image>>("process_image"_s);
					n_Transformation.add_namespace("Scale"_s)
						.add_function_proxy<&stp<&Tool::Image::Transformation::Scale::do_process_image>>("process_image"_s);
				}
				{
					auto n_Texture = n_Image.add_namespace("Texture"_s);
					define_generic_class<Tool::Image::Texture::Format>(n_Texture, "Format"_s);
					n_Texture.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &                    data,
							Image::CImageView const &            image,
							Tool::Image::Texture::Format const & format
						) -> Void {
								Generalization::match<Tool::Image::Texture::FormatPackage>(
									format,
									[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
										Tool::Image::Texture::Encode<format>::do_process_image(data, image);
									}
								);
							}
						>>>("process_image"_s);
					n_Texture.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &                    data,
							Image::VImageView const &            image,
							Tool::Image::Texture::Format const & format
						) -> Void {
								Generalization::match<Tool::Image::Texture::FormatPackage>(
									format,
									[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
										Tool::Image::Texture::Decode<format>::do_process_image(data, image);
									}
								);
							}
						>>>("process_image"_s);
					{
						auto n_Compression = n_Texture.add_namespace("Compression"_s);
						{
							auto n_ETC1 = n_Compression.add_namespace("ETC1"_s);
							n_ETC1.add_namespace("Compress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC1::Compress::do_process_image>>("process_image"_s);
							n_ETC1.add_namespace("Uncompress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC1::Uncompress::do_process_image>>("process_image"_s);
						}
						{
							auto n_ETC2 = n_Compression.add_namespace("ETC2"_s);
							n_ETC2.add_namespace("Compress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC2::Compress::do_process_image>>("process_image"_s);
							n_ETC2.add_namespace("Uncompress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC2::Uncompress::do_process_image>>("process_image"_s);
						}
						{
							auto n_PVRTC4 = n_Compression.add_namespace("PVRTC4"_s);
							n_PVRTC4.add_namespace("Compress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::PVRTC4::Compress::do_process_image>>("process_image"_s);
							n_PVRTC4.add_namespace("Uncompress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::PVRTC4::Uncompress::do_process_image>>("process_image"_s);
						}
					}
				}
				{
					auto n_File = n_Image.add_namespace("File"_s);
					{
						auto n_PNG = n_File.add_namespace("PNG"_s);
						n_PNG.add_namespace("Write"_s)
							.add_function_proxy<&stp<&Tool::Image::File::PNG::Write::do_process_image>>("process_image"_s);
						n_PNG.add_namespace("Read"_s)
							.add_function_proxy<&stp<&Tool::Image::File::PNG::Read::do_compute_image_size>>("compute_image_size"_s)
							.add_function_proxy<&stp<&Tool::Image::File::PNG::Read::do_process_image>>("process_image"_s);
					}
				}
			}
			// Wwise
			{
				auto n_Wwise = n_Tool.add_namespace("Wwise"_s);
				{
					using Tool::Wwise::Media::Version;
					using Tool::Wwise::Media::VersionPackage;
					auto n_Media = n_Wwise.add_namespace("Media"_s);
					define_generic_class<Version>(n_Media, "Version"_s);
					n_Media.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							CByteListView const & ripe,
							ByteArray &           raw,
							Path const &          ffmpeg_program,
							Path const &          ww2ogg_program,
							Path const &          ww2ogg_code_book,
							Path const &          temporary_directory,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::Media::Decode<version>::do_process_media(ripe, raw, ffmpeg_program, ww2ogg_program, ww2ogg_code_book, temporary_directory);
									}
								);
							}
						>>>("process_media"_s);
				}
				{
					using Tool::Wwise::SoundBank::Version;
					using Tool::Wwise::SoundBank::VersionPackage;
					using Tool::Wwise::SoundBank::Manifest;
					using SoundBankManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<2_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<3_ixz>>::SoundBank
					>;
					auto n_SoundBank = n_Wwise.add_namespace("SoundBank"_s);
					define_generic_class<Version>(n_SoundBank, "Version"_s);
					{
						auto n_Manifest = n_SoundBank.add_namespace("Manifest"_s);
						auto c_SoundBank = define_generic_class<SoundBankManifest, GCDF::generic_mask>(n_Manifest, "SoundBank"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_SoundBank);
					}
					n_SoundBank.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &         sound_bank_data,
							SoundBankManifest const & sound_bank_manifest,
							Path const &              embedded_audio_directory,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::SoundBank::Encode<version>::do_process_sound_bank(sound_bank_data, sound_bank_manifest.template get_of_index<mbw<Size>(index)>(), embedded_audio_directory);
									}
								);
							}
						>>>("process_sound_bank"_s);
					n_SoundBank.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &      sound_bank_data,
							SoundBankManifest &    sound_bank_manifest,
							Optional<Path> const & embedded_audio_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::SoundBank::Decode<version>::do_process_sound_bank(sound_bank_data, sound_bank_manifest.template set_of_index<mbw<Size>(index)>(), embedded_audio_directory);
									}
								);
							}
						>>>("process_sound_bank"_s);
				}
			}
			// Marmalade
			{
				auto n_Marmalade = n_Tool.add_namespace("Marmalade"_s);
				{
					using Tool::Marmalade::DZip::Version;
					using Tool::Marmalade::DZip::VersionPackage;
					using Tool::Marmalade::DZip::Manifest;
					using PackageManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Package
					>;
					auto n_DZip = n_Marmalade.add_namespace("DZip"_s);
					define_generic_class<Version>(n_DZip, "Version"_s);
					{
						auto n_Manifest = n_DZip.add_namespace("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(n_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					n_DZip.add_namespace("Pack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &       package_data,
							PackageManifest const & package_manifest,
							Path const &            resource_directory,
							Version const &         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Marmalade::DZip::Pack<version>::do_process_package(package_data, package_manifest.template get_of_index<mbw<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process_package"_s);
					n_DZip.add_namespace("Unpack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &      package_data,
							PackageManifest &      package_manifest,
							Optional<Path> const & resource_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Marmalade::DZip::Unpack<version>::do_process_package(package_data, package_manifest.template set_of_index<mbw<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process_package"_s);
				}
			}
			// PopCap
			{
				auto n_PopCap = n_Tool.add_namespace("PopCap"_s);
				{
					using Tool::PopCap::ZLib::Version;
					using Tool::PopCap::ZLib::VersionPackage;
					auto n_ZLib = n_PopCap.add_namespace("ZLib"_s);
					define_generic_class<Version>(n_ZLib, "Version"_s);
					n_ZLib.add_namespace("Compress"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							Size const &    raw_size,
							Size &          ripe_size_bound,
							Size const &    window_bits,
							Size const &    memory_level,
							Version const & version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Compress<version>::do_compute_size_bound(raw_size, ripe_size_bound, window_bits, memory_level);
									}
								);
							}
						>>>("compute_size_bound"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &                                  raw,
							OByteStreamView &                                  ripe,
							Size const &                                       level,
							Size const &                                       window_bits,
							Size const &                                       memory_level,
							Tool::Data::Compression::Deflate::Strategy const & strategy,
							Version const &                                    version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Compress<version>::do_process_whole(raw, ripe, level, window_bits, memory_level, strategy);
									}
								);
							}
						>>>("process_whole"_s);
					n_ZLib.add_namespace("Uncompress"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							CByteListView const & ripe,
							Size &                raw_size,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Uncompress<version>::do_compute_size(ripe, raw_size);
									}
								);
							}
						>>>("compute_size"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & ripe,
							OByteStreamView & raw,
							Size const &      window_bits,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Uncompress<version>::do_process_whole(ripe, raw, window_bits);
									}
								);
							}
						>>>("process_whole"_s);
				}
				{
					using Tool::PopCap::CryptData::Version;
					using Tool::PopCap::CryptData::VersionPackage;
					auto n_CryptData = n_PopCap.add_namespace("CryptData"_s);
					define_generic_class<Version>(n_CryptData, "Version"_s);
					n_CryptData.add_namespace("Encrypt"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							Size const &    plain_size,
							Size &          cipher_size,
							Size const &    limit,
							Version const & version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Encrypt<version>::do_compute_size(plain_size, cipher_size, limit);
									}
								);
							}
						>>>("compute_size"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & plain,
							OByteStreamView & cipher,
							Size const &      limit,
							String const &    key,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Encrypt<version>::do_process_whole(plain, cipher, limit, key);
									}
								);
							}
						>>>("process_whole"_s);
					n_CryptData.add_namespace("Decrypt"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							CByteListView const & cipher,
							Size &                plain_size,
							Size const &          limit,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Decrypt<version>::do_compute_size(cipher, plain_size, limit);
									}
								);
							}
						>>>("compute_size"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & cipher,
							OByteStreamView & plain,
							Size const &      limit,
							String const &    key,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Decrypt<version>::do_process_whole(cipher, plain, limit, key);
									}
								);
							}
						>>>("process_whole"_s);
				}
				{
					using Tool::PopCap::ReflectionObjectNotation::Version;
					using Tool::PopCap::ReflectionObjectNotation::VersionPackage;
					auto n_ReflectionObjectNotation = n_PopCap.add_namespace("ReflectionObjectNotation"_s);
					define_generic_class<Version>(n_ReflectionObjectNotation, "Version"_s);
					n_ReflectionObjectNotation.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &   data,
							JSON::Value const & value,
							Boolean const &     enable_string_index,
							Boolean const &     enable_rtid,
							Version const &     version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReflectionObjectNotation::Encode<version>::do_process_whole(data, value, enable_string_index, enable_rtid);
									}
								);
							}
						>>>("process_whole"_s);
					n_ReflectionObjectNotation.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & data,
							JSON::Value &     value,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReflectionObjectNotation::Decode<version>::do_process_whole(data, value);
									}
								);
							}
						>>>("process_whole"_s);
					n_ReflectionObjectNotation.add_namespace("Encrypt"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Encrypt::do_compute_size>>("compute_size"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Encrypt::do_process_whole>>("process_whole"_s);
					n_ReflectionObjectNotation.add_namespace("Decrypt"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Decrypt::do_compute_size>>("compute_size"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Decrypt::do_process_whole>>("process_whole"_s);
				}
				{
					using Tool::PopCap::UTexture::Version;
					using Tool::PopCap::UTexture::VersionPackage;
					auto n_UTexture = n_PopCap.add_namespace("UTexture"_s);
					define_generic_class<Version>(n_UTexture, "Version"_s);
					n_UTexture.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							Size &                               data_size_bound,
							Image::ImageSize const &             image_size,
							Tool::Image::Texture::Format const & format,
							Version const &                      version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Encode<version>::do_compute_data_size_bound(data_size_bound, image_size, format);
									}
								);
							}
						>>>("compute_data_size_bound"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &                    data,
							Image::CImageView const &            image,
							Tool::Image::Texture::Format const & format,
							Version const &                      version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Encode<version>::do_process_image(data, image, format);
									}
								);
							}
						>>>("process_image"_s);
					n_UTexture.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							CByteListView &    data,
							Image::ImageSize & image_size,
							Version const &    version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Decode<version>::do_compute_image_size(data, image_size);
									}
								);
							}
						>>>("compute_image_size"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &         data,
							Image::VImageView const & image,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Decode<version>::do_process_image(data, image);
									}
								);
							}
						>>>("process_image"_s);
				}
				{
					using Tool::PopCap::SexyTexture::Version;
					using Tool::PopCap::SexyTexture::VersionPackage;
					auto n_SexyTexture = n_PopCap.add_namespace("SexyTexture"_s);
					define_generic_class<Version>(n_SexyTexture, "Version"_s);
					n_SexyTexture.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							Size &                               data_size_bound,
							Image::ImageSize const &             image_size,
							Tool::Image::Texture::Format const & format,
							Boolean const &                      compress_texture_data,
							Version const &                      version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Encode<version>::do_compute_data_size_bound(data_size_bound, image_size, format, compress_texture_data);
									}
								);
							}
						>>>("compute_data_size_bound"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &                    data,
							Image::CImageView const &            image,
							Tool::Image::Texture::Format const & format,
							Boolean const &                      compress_texture_data,
							Version const &                      version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Encode<version>::do_process_image(data, image, format, compress_texture_data);
									}
								);
							}
						>>>("process_image"_s);
					n_SexyTexture.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							CByteListView &    data,
							Image::ImageSize & image_size,
							Version const &    version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Decode<version>::do_compute_image_size(data, image_size);
									}
								);
							}
						>>>("compute_image_size"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &         data,
							Image::VImageView const & image,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Decode<version>::do_process_image(data, image);
									}
								);
							}
						>>>("process_image"_s);
				}
				{
					using Tool::PopCap::Animation::Version;
					using Tool::PopCap::Animation::VersionPackage;
					using Tool::PopCap::Animation::Manifest;
					using AnimationManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Animation,
						typename Manifest<VersionPackage::element<2_ixz>>::Animation,
						typename Manifest<VersionPackage::element<3_ixz>>::Animation,
						typename Manifest<VersionPackage::element<4_ixz>>::Animation,
						typename Manifest<VersionPackage::element<5_ixz>>::Animation,
						typename Manifest<VersionPackage::element<6_ixz>>::Animation
					>;
					auto n_Animation = n_PopCap.add_namespace("Animation"_s);
					define_generic_class<Version>(n_Animation, "Version"_s);
					{
						auto n_Manifest = n_Animation.add_namespace("Manifest"_s);
						auto c_Animation = define_generic_class<AnimationManifest, GCDF::generic_mask>(n_Manifest, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					n_Animation.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &         animation_data,
							AnimationManifest const & animation_manifest,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Animation::Encode<version>::do_process_animation(animation_data, animation_manifest.template get_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_animation"_s);
					n_Animation.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &   animation_data,
							AnimationManifest & animation_manifest,
							Version const &     version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Animation::Decode<version>::do_process_animation(animation_data, animation_manifest.template set_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_animation"_s);
				}
				{
					using Tool::PopCap::ReAnimation::Version;
					using Tool::PopCap::ReAnimation::VersionPackage;
					using Tool::PopCap::ReAnimation::Manifest;
					using AnimationManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Animation,
						typename Manifest<VersionPackage::element<2_ixz>>::Animation,
						typename Manifest<VersionPackage::element<3_ixz>>::Animation,
						typename Manifest<VersionPackage::element<4_ixz>>::Animation
					>;
					auto n_ReAnimation = n_PopCap.add_namespace("ReAnimation"_s);
					define_generic_class<Version>(n_ReAnimation, "Version"_s);
					{
						auto n_Manifest = n_ReAnimation.add_namespace("Manifest"_s);
						auto c_Animation = define_generic_class<AnimationManifest, GCDF::generic_mask>(n_Manifest, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					n_ReAnimation.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &         animation_data,
							AnimationManifest const & animation_manifest,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReAnimation::Encode<version>::do_process_animation(animation_data, animation_manifest.template get_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_animation"_s);
					n_ReAnimation.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &   animation_data,
							AnimationManifest & animation_manifest,
							Version const &     version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReAnimation::Decode<version>::do_process_animation(animation_data, animation_manifest.template set_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_animation"_s);
				}
				{
					using Tool::PopCap::Particle::Version;
					using Tool::PopCap::Particle::VersionPackage;
					using Tool::PopCap::Particle::Manifest;
					using ParticleManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Particle,
						typename Manifest<VersionPackage::element<2_ixz>>::Particle,
						typename Manifest<VersionPackage::element<3_ixz>>::Particle,
						typename Manifest<VersionPackage::element<4_ixz>>::Particle
					>;
					auto n_Particle = n_PopCap.add_namespace("Particle"_s);
					define_generic_class<Version>(n_Particle, "Version"_s);
					{
						auto n_Manifest = n_Particle.add_namespace("Manifest"_s);
						auto c_Particle = define_generic_class<ParticleManifest, GCDF::generic_mask>(n_Manifest, "Particle"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Particle);
					}
					n_Particle.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &        particle_data,
							ParticleManifest const & particle_manifest,
							Version const &          version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Particle::Encode<version>::do_process_particle(particle_data, particle_manifest.template get_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_particle"_s);
					n_Particle.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &  particle_data,
							ParticleManifest & particle_manifest,
							Version const &    version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Particle::Decode<version>::do_process_particle(particle_data, particle_manifest.template set_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_particle"_s);
				}
				{
					using Tool::PopCap::Trail::Version;
					using Tool::PopCap::Trail::VersionPackage;
					using Tool::PopCap::Trail::Manifest;
					using TrailManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Trail,
						typename Manifest<VersionPackage::element<2_ixz>>::Trail,
						typename Manifest<VersionPackage::element<3_ixz>>::Trail,
						typename Manifest<VersionPackage::element<4_ixz>>::Trail
					>;
					auto n_Trail = n_PopCap.add_namespace("Trail"_s);
					define_generic_class<Version>(n_Trail, "Version"_s);
					{
						auto n_Manifest = n_Trail.add_namespace("Manifest"_s);
						auto c_Trail = define_generic_class<TrailManifest, GCDF::generic_mask>(n_Manifest, "Trail"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Trail);
					}
					n_Trail.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &     trail_data,
							TrailManifest const & trail_manifest,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Trail::Encode<version>::do_process_trail(trail_data, trail_manifest.template get_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_trail"_s);
					n_Trail.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & trail_data,
							TrailManifest &   trail_manifest,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Trail::Decode<version>::do_process_trail(trail_data, trail_manifest.template set_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_trail"_s);
				}
				{
					using Tool::PopCap::Effect::Version;
					using Tool::PopCap::Effect::VersionPackage;
					using Tool::PopCap::Effect::Manifest;
					using EffectManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Effect
					>;
					auto n_Effect = n_PopCap.add_namespace("Effect"_s);
					define_generic_class<Version>(n_Effect, "Version"_s);
					{
						auto n_Manifest = n_Effect.add_namespace("Manifest"_s);
						auto c_Effect = define_generic_class<EffectManifest, GCDF::generic_mask>(n_Manifest, "Effect"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Effect);
					}
					n_Effect.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &      effect_data,
							EffectManifest const & effect_manifest,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Effect::Encode<version>::do_process_effect(effect_data, effect_manifest.template get_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_effect"_s);
					n_Effect.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & effect_data,
							EffectManifest &  effect_manifest,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Effect::Decode<version>::do_process_effect(effect_data, effect_manifest.template set_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_effect"_s);
				}
				{
					using Tool::PopCap::CharacterFontWidget2::Version;
					using Tool::PopCap::CharacterFontWidget2::VersionPackage;
					using Tool::PopCap::CharacterFontWidget2::Manifest;
					using FontWidgetManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::FontWidget
					>;
					auto n_CharacterFontWidget2 = n_PopCap.add_namespace("CharacterFontWidget2"_s);
					define_generic_class<Version>(n_CharacterFontWidget2, "Version"_s);
					{
						auto n_Manifest = n_CharacterFontWidget2.add_namespace("Manifest"_s);
						auto c_FontWidget = define_generic_class<FontWidgetManifest, GCDF::generic_mask>(n_Manifest, "FontWidget"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_FontWidget);
					}
					n_CharacterFontWidget2.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &          font_widget_data,
							FontWidgetManifest const & font_widget_manifest,
							Version const &            version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CharacterFontWidget2::Encode<version>::do_process_font_widget(font_widget_data, font_widget_manifest.template get_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_font_widget"_s);
					n_CharacterFontWidget2.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &    font_widget_data,
							FontWidgetManifest & font_widget_manifest,
							Version const &      version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CharacterFontWidget2::Decode<version>::do_process_font_widget(font_widget_data, font_widget_manifest.template set_of_index<mbw<Size>(index)>());
									}
								);
							}
						>>>("process_font_widget"_s);
				}
				{
					using Tool::PopCap::Package::Version;
					using Tool::PopCap::Package::VersionPackage;
					using Tool::PopCap::Package::Manifest;
					using PackageManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Package,
						typename Manifest<VersionPackage::element<2_ixz>>::Package
					>;
					auto n_Package = n_PopCap.add_namespace("Package"_s);
					define_generic_class<Version>(n_Package, "Version"_s);
					{
						auto n_Manifest = n_Package.add_namespace("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(n_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					n_Package.add_namespace("Pack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &       package_data,
							PackageManifest const & package_manifest,
							Path const &            resource_directory,
							Version const &         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Package::Pack<version>::do_process_package(package_data, package_manifest.template get_of_index<mbw<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process_package"_s);
					n_Package.add_namespace("Unpack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &      package_data,
							PackageManifest &      package_manifest,
							Optional<Path> const & resource_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Package::Unpack<version>::do_process_package(package_data, package_manifest.template set_of_index<mbw<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process_package"_s);
				}
				{
					using Tool::PopCap::ResourceStreamGroup::Version;
					using Tool::PopCap::ResourceStreamGroup::VersionPackage;
					using Tool::PopCap::ResourceStreamGroup::Manifest;
					using PackageManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Package,
						typename Manifest<VersionPackage::element<2_ixz>>::Package
					>;
					auto n_ResourceStreamGroup = n_PopCap.add_namespace("ResourceStreamGroup"_s);
					define_generic_class<Version>(n_ResourceStreamGroup, "Version"_s);
					{
						auto n_Manifest = n_ResourceStreamGroup.add_namespace("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(n_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					n_ResourceStreamGroup.add_namespace("Pack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &       package_data,
							PackageManifest const & package_manifest,
							Path const &            resource_directory,
							Version const &         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamGroup::Pack<version>::do_process_package(package_data, package_manifest.template get_of_index<mbw<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process_package"_s);
					n_ResourceStreamGroup.add_namespace("Unpack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &      package_data,
							PackageManifest &      package_manifest,
							Optional<Path> const & resource_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamGroup::Unpack<version>::do_process_package(package_data, package_manifest.template set_of_index<mbw<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process_package"_s);
				}
				{
					using Tool::PopCap::ResourceStreamBundle::Version;
					using Tool::PopCap::ResourceStreamBundle::VersionPackage;
					using Tool::PopCap::ResourceStreamBundle::Manifest;
					using Tool::PopCap::ResourceStreamBundle::Description;
					using PackageManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Package,
						typename Manifest<VersionPackage::element<2_ixz>>::Package,
						typename Manifest<VersionPackage::element<3_ixz>>::Package,
						typename Manifest<VersionPackage::element<4_ixz>>::Package
					>;
					using PackageDescriptionOptional = Variant<
						Optional<typename Description<VersionPackage::element<1_ixz>>::Package>,
						Optional<typename Description<VersionPackage::element<2_ixz>>::Package>,
						Optional<typename Description<VersionPackage::element<3_ixz>>::Package>,
						Optional<typename Description<VersionPackage::element<4_ixz>>::Package>
					>;
					auto n_ResourceStreamBundle = n_PopCap.add_namespace("ResourceStreamBundle"_s);
					define_generic_class<Version>(n_ResourceStreamBundle, "Version"_s);
					{
						auto n_Manifest = n_ResourceStreamBundle.add_namespace("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(n_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					{
						auto n_Description = n_ResourceStreamBundle.add_namespace("Description"_s);
						auto c_PackageOptional = define_generic_class<PackageDescriptionOptional, GCDF::generic_mask>(n_Description, "PackageOptional"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_PackageOptional);
					}
					n_ResourceStreamBundle.add_namespace("Pack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &                  package_data,
							PackageManifest const &            package_manifest,
							PackageDescriptionOptional const & package_description,
							Path const &                       resource_directory,
							Optional<Path> const &             packet_file,
							Optional<Path> const &             new_packet_file,
							Version const &                    version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundle::Pack<version>::do_process_package(package_data, package_manifest.template get_of_index<mbw<Size>(index)>(), package_description.template get_of_index<mbw<Size>(index)>(), resource_directory, packet_file, new_packet_file);
									}
								);
							}
						>>>("process_package"_s);
					n_ResourceStreamBundle.add_namespace("Unpack"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &            package_data,
							PackageManifest &            package_manifest,
							PackageDescriptionOptional & package_description,
							Optional<Path> const &       resource_directory,
							Optional<Path> const &       packet_file,
							Version const &              version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundle::Unpack<version>::do_process_package(package_data, package_manifest.template set_of_index<mbw<Size>(index)>(), package_description.template set_of_index<mbw<Size>(index)>(), resource_directory, packet_file);
									}
								);
							}
						>>>("process_package"_s);
				}
				{
					using Tool::PopCap::ResourceStreamBundlePatch::Version;
					using Tool::PopCap::ResourceStreamBundlePatch::VersionPackage;
					auto n_ResourceStreamBundlePatch = n_PopCap.add_namespace("ResourceStreamBundlePatch"_s);
					define_generic_class<Version>(n_ResourceStreamBundlePatch, "Version"_s);
					n_ResourceStreamBundlePatch.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & before,
							IByteStreamView & after,
							OByteStreamView & patch,
							Boolean const &   use_raw_packet,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundlePatch::Encode<version>::do_process_whole(before, after, patch, use_raw_packet);
									}
								);
							}
						>>>("process_whole"_s);
					n_ResourceStreamBundlePatch.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView & before,
							OByteStreamView & after,
							IByteStreamView & patch,
							Boolean const &   use_raw_packet,
							Version const &   version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundlePatch::Decode<version>::do_process_whole(before, after, patch, use_raw_packet);
									}
								);
							}
						>>>("process_whole"_s);
				}
			}
			// Miscellaneous
			{
				auto n_Miscellaneous = n_Tool.add_namespace("Miscellaneous"_s);
				{
					auto n_XboxTiledTexture = n_Miscellaneous.add_namespace("XboxTiledTexture"_s);
					n_XboxTiledTexture.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::XboxTiledTexture::Encode::do_process_image>>("process_image"_s);
					n_XboxTiledTexture.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::XboxTiledTexture::Decode::do_process_image>>("process_image"_s);
				}
				{
					auto n_PvZ2ChineseAndroidAlphaPaletteTexture = n_Miscellaneous.add_namespace("PvZ2ChineseAndroidAlphaPaletteTexture"_s);
					n_PvZ2ChineseAndroidAlphaPaletteTexture.add_namespace("Encode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::PvZ2ChineseAndroidAlphaPaletteTexture::Encode::do_process_image>>("process_image"_s);
					n_PvZ2ChineseAndroidAlphaPaletteTexture.add_namespace("Decode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::PvZ2ChineseAndroidAlphaPaletteTexture::Decode::do_process_image>>("process_image"_s);
				}
			}
		}
		// Miscellaneous
		{
			auto n_Miscellaneous = n_Core.add_namespace("Miscellaneous"_s);
			define_generic_class<Thread, GCDF::default_constructor>(n_Miscellaneous, "Thread"_s)
				.add_member_function_proxy<&Thread::joinable>("joinable"_s)
				.add_member_function_proxy<&Thread::join>("join"_s)
				.add_member_function_proxy<&Thread::detach>("detach"_s)
				.add_static_function_proxy<&Thread::yield>("yield"_s)
				.add_static_function_proxy<&Thread::sleep>("sleep"_s);
			define_generic_class<Context, GCDF::none_mask>(n_Miscellaneous, "Context"_s)
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
							return JS::Handler<List<String>>::new_instance_allocate(thix.value().callback(argument.value()));
						}
					>
				>("callback"_s)
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
					) -> JS::Handler<Boolean> {
							return JS::Handler<Boolean>::new_instance_allocate(thix.value().busy());
						}
					>
				>("busy"_s)
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
				>("execute"_s);
			n_Miscellaneous
				.add_function_proxy<
					&normalized_lambda<
						[] (
						VByteListView & it
					) -> VCharacterListView {
							return self_cast<VCharacterListView>(it);
						}
					>
				>("cast_ByteListView_to_CharacterListView"_s)
				.add_function_proxy<
					&normalized_lambda<
						[] (
						VCharacterListView & it
					) -> VByteListView {
							return self_cast<VByteListView>(it);
						}
					>
				>("cast_CharacterListView_to_ByteListView"_s)
				.add_function_proxy<
					&normalized_lambda<
						[] (
						ByteArray & it
					) -> String {
							auto result = String{};
							result.bind(from_byte_view<Character>(it.view()));
							it.unbind();
							return result;
						}
					>
				>("cast_moveable_ByteArray_to_String"_s)
				.add_function_proxy<
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
				>("cast_moveable_String_to_ByteArray"_s)
				.add_function_proxy<
					&normalized_lambda<
						[] (
						String & it
					) -> VCharacterListView {
							return up_cast<VCharacterListView>(it.view());
						}
					>
				>("cast_String_to_CharacterListView"_s)
				.add_function<
					&normalized_lambda<
						[] (
						JS::Handler<VCharacterListView> & it
					) -> VStringView& {
							// NOTE : return StringView is cheap
							return down_cast<VStringView>(it.value());
						}
					>
				>("cast_CharacterListView_to_JS_String"_s);
			n_Miscellaneous.add_variable("g_version"_s, context.context().new_value(JS::Handler<Size>::new_instance_allocate(mbw<Size>(M_version))));
			n_Miscellaneous.add_variable("g_context"_s, context.context().new_value(JS::Handler<Context>::new_reference(context)));
			n_Miscellaneous.add_variable("g_byte_stream_use_big_endian"_s, context.context().new_value(JS::Handler<Boolean>::new_reference(g_byte_stream_use_big_endian)));
		}
		return;
		#if defined M_compiler_msvc
		#pragma warning(pop)
		#endif
		#if defined M_compiler_clang
		#pragma clang diagnostic pop
		#endif
	}

	#pragma endregion

}
