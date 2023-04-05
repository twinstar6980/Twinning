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
		JavaScript::NativeSpaceBuilder & space,
		String const &                   name
	) -> JavaScript::NativeClassBuilder<Class> {
		auto builder = space.add_class<Class>(name);
		builder.template set_constructor<
			&normalized_lambda<
				[] (
			) -> JavaScript::NativeValueHandler<Class> {
					throw UnnamedException{mss("java-script style constructor is not allowed"_sf())};
				}
			>
		>();
		if constexpr (flag * GCDF::default_constructor) {
			builder.template add_constructor_allocate_proxy<>("default"_s);
		}
		if constexpr (flag * GCDF::copy_constructor) {
			builder.template add_constructor_allocate_proxy<Class const &>("copy"_s);
		}
		if constexpr (flag * GCDF::value_constructor) {
			builder.template add_constructor<
				&normalized_lambda<
					[] (
					Class & that
				) -> JavaScript::NativeValueHandler<Class> {
						return JavaScript::NativeValueHandler<Class>::new_instance_allocate(that);
					}
				>
			>("value"_s);
		}
		if constexpr (flag * GCDF::value_getter || flag * GCDF::value_setter) {
			static_assert(flag * GCDF::value_getter);
			// NOTE : return reference is cheap
			constexpr auto & getter = normalized_lambda<
				[] (
				JavaScript::NativeValueHandler<Class> & thix
			) -> Class& {
					return thix.value();
				}
			>;
			if constexpr (!(flag * GCDF::value_setter)) {
				builder.template add_getter<&getter>("value"_s);
			} else {
				constexpr auto & setter = normalized_lambda<
					[] (
					JavaScript::NativeValueHandler<Class> & thix,
					Class &                                 value
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
		JavaScript::NativeClassBuilder<Class> & builder
	) -> JavaScript::NativeClassBuilder<Class>& {
		constexpr auto & json_constructor = JavaScript::proxy_native_function_by_handler<
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
		constexpr auto & json_getter = JavaScript::proxy_native_function_by_handler<
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
		constexpr auto & json_setter = JavaScript::proxy_native_function_by_handler<
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
		builder.template add_constructor<&json_constructor>("json"_s);
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
		auto s_TwinStar = JavaScript::NativeSpaceBuilder{k_null_optional, "TwinStar"_s, as_lvalue(context.context().global_object())};
		auto s_Core = s_TwinStar.add_space("Core"_s);
		// Boolean
		define_generic_class<Boolean>(s_Core, "Boolean"_s);
		// Number
		define_generic_class<IntegerU32>(s_Core, "IntegerU32"_s);
		// Size
		define_generic_class<Size>(s_Core, "Size"_s);
		define_generic_class<Optional<Size>>(s_Core, "SizeOptional"_s);
		// Byte
		define_generic_class<Byte>(s_Core, "Byte"_s);
		// String
		define_generic_class<String>(s_Core, "String"_s);
		define_generic_class<Optional<String>>(s_Core, "StringOptional"_s);
		define_generic_class<List<String>>(s_Core, "StringList"_s);
		// Path
		define_generic_class<Path>(s_Core, "Path"_s);
		define_generic_class<Optional<Path>>(s_Core, "PathOptional"_s);
		define_generic_class<List<Path>>(s_Core, "PathList"_s);
		// ByteArray
		define_generic_class<ByteArray>(s_Core, "ByteArray"_s)
			.add_constructor_allocate_proxy<Size const &>("allocate"_s)
			.add_member_function_proxy<&ByteArray::allocate>("allocate"_s)
			.add_member_function_proxy<&ByteArray::reset>("reset"_s)
			.add_member_function_proxy<&ByteArray::size>("size"_s)
			.add_member_function_proxy<AsVMemberFunction<ByteArray, ByteArray::View>{&ByteArray::view}>("view"_s)
			.add_member_function<
				&normalized_lambda<
					[] (
					JavaScript::NativeValueHandler<ByteArray> & that
				) -> ByteArray&& {
						return as_moveable(that.value());
					}
				>
			>("release"_s);
		// ByteListView
		define_generic_class<VByteListView>(s_Core, "ByteListView"_s)
			.add_member_function_proxy<&VByteListView::size>("size"_s)
			.add_member_function_proxy<AsCMemberFunction<VByteListView, VByteListView, Size const &, Size const &>{&VByteListView::sub}>("sub"_s);
		// ByteStreamView
		define_generic_class<IOByteStreamView, GCDF::generic_mask>(s_Core, "ByteStreamView"_s)
			.add_constructor_allocate_proxy<VByteListView const &>("watch"_s)
			.add_member_function_proxy<&IOByteStreamView::size>("size"_s)
			.add_member_function_proxy<&IOByteStreamView::position>("position"_s)
			.add_member_function_proxy<&IOByteStreamView::set_position>("set_position"_s)
			.add_member_function_proxy<&IOByteStreamView::view>("view"_s)
			.add_member_function_proxy<&IOByteStreamView::stream_view>("stream_view"_s);
		// CharacterListView
		define_generic_class<VCharacterListView, GCDF::generic_mask>(s_Core, "CharacterListView"_s)
			.add_member_function_proxy<&VCharacterListView::size>("size"_s)
			.add_member_function_proxy<AsCMemberFunction<VCharacterListView, VCharacterListView, Size const &, Size const &>{&VCharacterListView::sub}>("sub"_s);
		// CharacterStreamView
		define_generic_class<IOCharacterStreamView, GCDF::generic_mask>(s_Core, "CharacterStreamView"_s)
			.add_constructor_allocate_proxy<VCharacterListView const &>("watch"_s)
			.add_member_function_proxy<&IOCharacterStreamView::size>("size"_s)
			.add_member_function_proxy<&IOCharacterStreamView::position>("position"_s)
			.add_member_function_proxy<&IOCharacterStreamView::set_position>("set_position"_s)
			.add_member_function_proxy<&IOCharacterStreamView::view>("view"_s)
			.add_member_function_proxy<&IOCharacterStreamView::stream_view>("stream_view"_s);
		// JSON
		{
			auto s_JSON = s_Core.add_space("JSON"_s);
			define_generic_class<JSON::Value>(s_JSON, "Value"_s);
		}
		// XML
		{
			auto s_XML = s_Core.add_space("XML"_s);
			define_generic_class<XML::Node>(s_XML, "Node"_s);
		}
		// Image
		{
			auto s_Image = s_Core.add_space("Image"_s);
			define_generic_class<Image::ImageSize>(s_Image, "ImageSize"_s);
			define_generic_class<Image::ImagePosition>(s_Image, "ImagePosition"_s);
			define_generic_class<Image::Color>(s_Image, "Color"_s);
			define_generic_class<List<Image::Color>>(s_Image, "ColorList"_s);
			define_generic_class<Image::Pixel>(s_Image, "Pixel"_s);
			define_generic_class<Image::VImageView, GCDF::generic_mask>(s_Image, "ImageView"_s)
				.add_member_function_proxy<&Image::VImageView::size>("size"_s)
				.add_member_function_proxy<&Image::VImageView::fill>("fill"_s)
				.add_member_function_proxy<&Image::VImageView::draw>("draw"_s)
				.add_member_function_proxy<&Image::VImageView::sub>("sub"_s);
			define_generic_class<Image::Image, GCDF::generic_mask>(s_Image, "Image"_s)
				.add_constructor_allocate_proxy<Image::ImageSize const &>("allocate"_s)
				.add_member_function_proxy<&Image::Image::allocate>("allocate"_s)
				.add_member_function_proxy<&Image::Image::reset>("reset"_s)
				.add_member_function_proxy<&Image::Image::size>("size"_s)
				.add_member_function_proxy<AsVMemberFunction<Image::Image, Image::VImageView>{&Image::Image::view}>("view"_s);
		}
		// FileSystem
		{
			auto s_FileSystem = s_Core.add_space("FileSystem"_s);
			s_FileSystem
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
				.add_function_proxy<&stp<&FileSystem::list_directory>>("list_directory"_s);
		}
		// Process
		{
			auto s_Process = s_Core.add_space("Process"_s);
			s_Process
				// working directory
				.add_function_proxy<&stp<&Process::get_working_directory>>("get_working_directory"_s)
				.add_function_proxy<&stp<&Process::set_working_directory>>("set_working_directory"_s)
				// environment variable
				.add_function_proxy<&stp<&Process::get_environment_variable>>("get_environment_variable"_s)
				.add_function_proxy<&stp<&Process::set_environment_variable>>("set_environment_variable"_s)
				.add_function_proxy<&stp<&Process::list_environment_variable>>("list_environment_variable"_s)
				// child process
				.add_function_proxy<&stp<&Process::spawn_process>>("spawn_process"_s)
				// system command
				.add_function_proxy<&stp<&Process::system_command>>("system_command"_s);
		}
		// Tool
		{
			auto s_Tool = s_Core.add_space("Tool"_s);
			// Data
			{
				auto s_Data = s_Tool.add_space("Data"_s);
				{
					auto s_Hash = s_Data.add_space("Hash"_s);
					{
						auto s_FNV = s_Hash.add_space("FNV"_s);
						define_generic_class<Tool::Data::Hash::FNV::Mode>(s_FNV, "Mode"_s);
						define_generic_class<Tool::Data::Hash::FNV::BitCount>(s_FNV, "BitCount"_s);
						s_FNV.add_space("Hash"_s)
							.add_function_proxy<&stp<Tool::Data::Hash::FNV::Hash::do_process_whole>>("process_whole"_s);
					}
					{
						auto s_MD5 = s_Hash.add_space("MD5"_s);
						s_MD5.add_space("Hash"_s)
							.add_function_proxy<&stp<&Tool::Data::Hash::MD5::Hash::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto s_Encoding = s_Data.add_space("Encoding"_s);
					{
						auto s_Base64 = s_Encoding.add_space("Base64"_s);
						s_Base64.add_space("Encode"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Encode::do_compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Encode::do_process_whole>>("process_whole"_s);
						s_Base64.add_space("Decode"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Decode::do_compute_size>>("compute_size"_s)
							.add_function_proxy<&stp<&Tool::Data::Encoding::Base64::Decode::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto s_Encryption = s_Data.add_space("Encryption"_s);
					{
						auto s_XOR = s_Encryption.add_space("XOR"_s);
						s_XOR.add_space("Encrypt"_s)
							.add_function_proxy<&stp<&Tool::Data::Encryption::XOR::Encrypt::do_process_whole>>("process_whole"_s);
					}
					{
						auto s_Rijndael = s_Encryption.add_space("Rijndael"_s);
						define_generic_class<Tool::Data::Encryption::Rijndael::Mode>(s_Rijndael, "Mode"_s);
						s_Rijndael.add_space("Encrypt"_s)
							.add_function_proxy<&stp<&Tool::Data::Encryption::Rijndael::Encrypt::do_process_whole>>("process_whole"_s);
						s_Rijndael.add_space("Decrypt"_s)
							.add_function_proxy<&stp<&Tool::Data::Encryption::Rijndael::Decrypt::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto s_Compression = s_Data.add_space("Compression"_s);
					{
						auto s_Deflate = s_Compression.add_space("Deflate"_s);
						define_generic_class<Tool::Data::Compression::Deflate::Strategy>(s_Deflate, "Strategy"_s);
						define_generic_class<Tool::Data::Compression::Deflate::Wrapper>(s_Deflate, "Wrapper"_s);
						s_Deflate.add_space("Compress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Deflate::Compress::do_compute_size_bound>>("compute_size_bound"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Deflate::Compress::do_process_whole>>("process_whole"_s);
						s_Deflate.add_space("Uncompress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Deflate::Uncompress::do_process_whole>>("process_whole"_s);
					}
					{
						auto s_BZip2 = s_Compression.add_space("BZip2"_s);
						s_BZip2.add_space("Compress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::BZip2::Compress::do_process_whole>>("process_whole"_s);
						s_BZip2.add_space("Uncompress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::BZip2::Uncompress::do_process_whole>>("process_whole"_s);
					}
					{
						auto s_Lzma = s_Compression.add_space("Lzma"_s);
						s_Lzma.add_space("Compress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Lzma::Compress::do_process_whole>>("process_whole"_s);
						s_Lzma.add_space("Uncompress"_s)
							.add_function_proxy<&stp<&Tool::Data::Compression::Lzma::Uncompress::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto s_Differentiation = s_Data.add_space("Differentiation"_s);
					{
						auto s_VCDiff = s_Differentiation.add_space("VCDiff"_s);
						s_VCDiff.add_space("Encode"_s)
							.add_function_proxy<&stp<&Tool::Data::Differentiation::VCDiff::Encode::do_process_whole>>("process_whole"_s);
						s_VCDiff.add_space("Decode"_s)
							.add_function_proxy<&stp<&Tool::Data::Differentiation::VCDiff::Decode::do_process_whole>>("process_whole"_s);
					}
				}
				{
					auto s_Serialization = s_Data.add_space("Serialization"_s);
					{
						auto s_JSON = s_Serialization.add_space("JSON"_s);
						s_JSON.add_space("Write"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::JSON::Write::do_process_whole>>("process_whole"_s);
						s_JSON.add_space("Read"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::JSON::Read::do_process_whole>>("process_whole"_s);
					}
					{
						auto s_XML = s_Serialization.add_space("XML"_s);
						s_XML.add_space("Write"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::XML::Write::do_process_whole>>("process_whole"_s);
						s_XML.add_space("Read"_s)
							.add_function_proxy<&stp<&Tool::Data::Serialization::XML::Read::do_process_whole>>("process_whole"_s);
					}
				}
			}
			// Image
			{
				auto s_Image = s_Tool.add_space("Image"_s);
				{
					auto s_Transformation = s_Image.add_space("Transformation"_s);
					s_Transformation.add_space("Flip"_s)
						.add_function_proxy<&stp<&Tool::Image::Transformation::Flip::do_process_image>>("process_image"_s);
					s_Transformation.add_space("Scale"_s)
						.add_function_proxy<&stp<&Tool::Image::Transformation::Scale::do_process_image>>("process_image"_s);
				}
				{
					auto s_Texture = s_Image.add_space("Texture"_s);
					define_generic_class<Tool::Image::Texture::Format>(s_Texture, "Format"_s);
					s_Texture.add_space("Encode"_s)
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
					s_Texture.add_space("Decode"_s)
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
						auto s_Compression = s_Texture.add_space("Compression"_s);
						{
							auto s_ETC1 = s_Compression.add_space("ETC1"_s);
							s_ETC1.add_space("Compress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC1::Compress::do_process_image>>("process_image"_s);
							s_ETC1.add_space("Uncompress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC1::Uncompress::do_process_image>>("process_image"_s);
						}
						{
							auto s_ETC2 = s_Compression.add_space("ETC2"_s);
							s_ETC2.add_space("Compress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC2::Compress::do_process_image>>("process_image"_s);
							s_ETC2.add_space("Uncompress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::ETC2::Uncompress::do_process_image>>("process_image"_s);
						}
						{
							auto s_PVRTC4 = s_Compression.add_space("PVRTC4"_s);
							s_PVRTC4.add_space("Compress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::PVRTC4::Compress::do_process_image>>("process_image"_s);
							s_PVRTC4.add_space("Uncompress"_s)
								.add_function_proxy<&stp<&Tool::Image::Texture::Compression::PVRTC4::Uncompress::do_process_image>>("process_image"_s);
						}
					}
				}
				{
					auto s_File = s_Image.add_space("File"_s);
					{
						auto s_PNG = s_File.add_space("PNG"_s);
						s_PNG.add_space("Write"_s)
							.add_function_proxy<&stp<&Tool::Image::File::PNG::Write::do_process_image>>("process_image"_s);
						s_PNG.add_space("Read"_s)
							.add_function_proxy<&stp<&Tool::Image::File::PNG::Read::do_compute_image_size>>("compute_image_size"_s)
							.add_function_proxy<&stp<&Tool::Image::File::PNG::Read::do_process_image>>("process_image"_s);
					}
				}
			}
			// Wwise
			{
				auto s_Wwise = s_Tool.add_space("Wwise"_s);
				{
					using Tool::Wwise::Media::Version;
					using Tool::Wwise::Media::VersionPackage;
					auto s_Media = s_Wwise.add_space("Media"_s);
					define_generic_class<Version>(s_Media, "Version"_s);
					s_Media.add_space("Decode"_s)
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
						typename Manifest<VersionPackage::element<3_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<4_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<5_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<6_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<7_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<8_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<9_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<10_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<11_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<12_ixz>>::SoundBank,
						typename Manifest<VersionPackage::element<13_ixz>>::SoundBank
					>;
					auto s_SoundBank = s_Wwise.add_space("SoundBank"_s);
					define_generic_class<Version>(s_SoundBank, "Version"_s);
					{
						auto s_Manifest = s_SoundBank.add_space("Manifest"_s);
						auto c_SoundBank = define_generic_class<SoundBankManifest, GCDF::generic_mask>(s_Manifest, "SoundBank"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_SoundBank);
					}
					s_SoundBank.add_space("Encode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							OByteStreamView &         sound_bank_data,
							SoundBankManifest const & sound_bank_manifest,
							Path const &              embedded_media_directory,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::SoundBank::Encode<version>::do_process_sound_bank(sound_bank_data, sound_bank_manifest.template get_of_index<mbw<Size>(index)>(), embedded_media_directory);
									}
								);
							}
						>>>("process_sound_bank"_s);
					s_SoundBank.add_space("Decode"_s)
						.add_function_proxy<&stp<&normalized_lambda<
							[] (
							IByteStreamView &      sound_bank_data,
							SoundBankManifest &    sound_bank_manifest,
							Optional<Path> const & embedded_media_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::SoundBank::Decode<version>::do_process_sound_bank(sound_bank_data, sound_bank_manifest.template set_of_index<mbw<Size>(index)>(), embedded_media_directory);
									}
								);
							}
						>>>("process_sound_bank"_s);
				}
			}
			// Marmalade
			{
				auto s_Marmalade = s_Tool.add_space("Marmalade"_s);
				{
					using Tool::Marmalade::DZip::Version;
					using Tool::Marmalade::DZip::VersionPackage;
					using Tool::Marmalade::DZip::Manifest;
					using PackageManifest = Variant<
						typename Manifest<VersionPackage::element<1_ixz>>::Package
					>;
					auto s_DZip = s_Marmalade.add_space("DZip"_s);
					define_generic_class<Version>(s_DZip, "Version"_s);
					{
						auto s_Manifest = s_DZip.add_space("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(s_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_DZip.add_space("Pack"_s)
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
					s_DZip.add_space("Unpack"_s)
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
				auto s_PopCap = s_Tool.add_space("PopCap"_s);
				{
					using Tool::PopCap::ZLib::Version;
					using Tool::PopCap::ZLib::VersionPackage;
					auto s_ZLib = s_PopCap.add_space("ZLib"_s);
					define_generic_class<Version>(s_ZLib, "Version"_s);
					s_ZLib.add_space("Compress"_s)
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
					s_ZLib.add_space("Uncompress"_s)
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
					auto s_CryptData = s_PopCap.add_space("CryptData"_s);
					define_generic_class<Version>(s_CryptData, "Version"_s);
					s_CryptData.add_space("Encrypt"_s)
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
					s_CryptData.add_space("Decrypt"_s)
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
					auto s_ReflectionObjectNotation = s_PopCap.add_space("ReflectionObjectNotation"_s);
					define_generic_class<Version>(s_ReflectionObjectNotation, "Version"_s);
					s_ReflectionObjectNotation.add_space("Encode"_s)
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
					s_ReflectionObjectNotation.add_space("Decode"_s)
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
					s_ReflectionObjectNotation.add_space("Encrypt"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Encrypt::do_compute_size>>("compute_size"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Encrypt::do_process_whole>>("process_whole"_s);
					s_ReflectionObjectNotation.add_space("Decrypt"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Decrypt::do_compute_size>>("compute_size"_s)
						.add_function_proxy<&stp<&Tool::PopCap::ReflectionObjectNotation::Decrypt::do_process_whole>>("process_whole"_s);
				}
				{
					using Tool::PopCap::UTexture::Version;
					using Tool::PopCap::UTexture::VersionPackage;
					auto s_UTexture = s_PopCap.add_space("UTexture"_s);
					define_generic_class<Version>(s_UTexture, "Version"_s);
					s_UTexture.add_space("Encode"_s)
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
					s_UTexture.add_space("Decode"_s)
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
					auto s_SexyTexture = s_PopCap.add_space("SexyTexture"_s);
					define_generic_class<Version>(s_SexyTexture, "Version"_s);
					s_SexyTexture.add_space("Encode"_s)
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
					s_SexyTexture.add_space("Decode"_s)
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
					auto s_Animation = s_PopCap.add_space("Animation"_s);
					define_generic_class<Version>(s_Animation, "Version"_s);
					{
						auto s_Manifest = s_Animation.add_space("Manifest"_s);
						auto c_Animation = define_generic_class<AnimationManifest, GCDF::generic_mask>(s_Manifest, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					s_Animation.add_space("Encode"_s)
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
					s_Animation.add_space("Decode"_s)
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
					auto s_ReAnimation = s_PopCap.add_space("ReAnimation"_s);
					define_generic_class<Version>(s_ReAnimation, "Version"_s);
					{
						auto s_Manifest = s_ReAnimation.add_space("Manifest"_s);
						auto c_Animation = define_generic_class<AnimationManifest, GCDF::generic_mask>(s_Manifest, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					s_ReAnimation.add_space("Encode"_s)
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
					s_ReAnimation.add_space("Decode"_s)
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
					auto s_Particle = s_PopCap.add_space("Particle"_s);
					define_generic_class<Version>(s_Particle, "Version"_s);
					{
						auto s_Manifest = s_Particle.add_space("Manifest"_s);
						auto c_Particle = define_generic_class<ParticleManifest, GCDF::generic_mask>(s_Manifest, "Particle"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Particle);
					}
					s_Particle.add_space("Encode"_s)
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
					s_Particle.add_space("Decode"_s)
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
					auto s_Trail = s_PopCap.add_space("Trail"_s);
					define_generic_class<Version>(s_Trail, "Version"_s);
					{
						auto s_Manifest = s_Trail.add_space("Manifest"_s);
						auto c_Trail = define_generic_class<TrailManifest, GCDF::generic_mask>(s_Manifest, "Trail"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Trail);
					}
					s_Trail.add_space("Encode"_s)
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
					s_Trail.add_space("Decode"_s)
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
					auto s_Effect = s_PopCap.add_space("Effect"_s);
					define_generic_class<Version>(s_Effect, "Version"_s);
					{
						auto s_Manifest = s_Effect.add_space("Manifest"_s);
						auto c_Effect = define_generic_class<EffectManifest, GCDF::generic_mask>(s_Manifest, "Effect"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Effect);
					}
					s_Effect.add_space("Encode"_s)
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
					s_Effect.add_space("Decode"_s)
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
					auto s_CharacterFontWidget2 = s_PopCap.add_space("CharacterFontWidget2"_s);
					define_generic_class<Version>(s_CharacterFontWidget2, "Version"_s);
					{
						auto s_Manifest = s_CharacterFontWidget2.add_space("Manifest"_s);
						auto c_FontWidget = define_generic_class<FontWidgetManifest, GCDF::generic_mask>(s_Manifest, "FontWidget"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_FontWidget);
					}
					s_CharacterFontWidget2.add_space("Encode"_s)
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
					s_CharacterFontWidget2.add_space("Decode"_s)
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
					auto s_Package = s_PopCap.add_space("Package"_s);
					define_generic_class<Version>(s_Package, "Version"_s);
					{
						auto s_Manifest = s_Package.add_space("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(s_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_Package.add_space("Pack"_s)
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
					s_Package.add_space("Unpack"_s)
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
					auto s_ResourceStreamGroup = s_PopCap.add_space("ResourceStreamGroup"_s);
					define_generic_class<Version>(s_ResourceStreamGroup, "Version"_s);
					{
						auto s_Manifest = s_ResourceStreamGroup.add_space("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(s_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_ResourceStreamGroup.add_space("Pack"_s)
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
					s_ResourceStreamGroup.add_space("Unpack"_s)
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
					auto s_ResourceStreamBundle = s_PopCap.add_space("ResourceStreamBundle"_s);
					define_generic_class<Version>(s_ResourceStreamBundle, "Version"_s);
					{
						auto s_Manifest = s_ResourceStreamBundle.add_space("Manifest"_s);
						auto c_Package = define_generic_class<PackageManifest, GCDF::generic_mask>(s_Manifest, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					{
						auto s_Description = s_ResourceStreamBundle.add_space("Description"_s);
						auto c_PackageOptional = define_generic_class<PackageDescriptionOptional, GCDF::generic_mask>(s_Description, "PackageOptional"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_PackageOptional);
					}
					s_ResourceStreamBundle.add_space("Pack"_s)
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
					s_ResourceStreamBundle.add_space("Unpack"_s)
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
					auto s_ResourceStreamBundlePatch = s_PopCap.add_space("ResourceStreamBundlePatch"_s);
					define_generic_class<Version>(s_ResourceStreamBundlePatch, "Version"_s);
					s_ResourceStreamBundlePatch.add_space("Encode"_s)
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
					s_ResourceStreamBundlePatch.add_space("Decode"_s)
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
				auto s_Miscellaneous = s_Tool.add_space("Miscellaneous"_s);
				{
					auto s_XboxTiledTexture = s_Miscellaneous.add_space("XboxTiledTexture"_s);
					s_XboxTiledTexture.add_space("Encode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::XboxTiledTexture::Encode::do_process_image>>("process_image"_s);
					s_XboxTiledTexture.add_space("Decode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::XboxTiledTexture::Decode::do_process_image>>("process_image"_s);
				}
				{
					auto s_PvZ2ChineseAndroidAlphaPaletteTexture = s_Miscellaneous.add_space("PvZ2ChineseAndroidAlphaPaletteTexture"_s);
					s_PvZ2ChineseAndroidAlphaPaletteTexture.add_space("Encode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::PvZ2ChineseAndroidAlphaPaletteTexture::Encode::do_process_image>>("process_image"_s);
					s_PvZ2ChineseAndroidAlphaPaletteTexture.add_space("Decode"_s)
						.add_function_proxy<&stp<&Tool::Miscellaneous::PvZ2ChineseAndroidAlphaPaletteTexture::Decode::do_process_image>>("process_image"_s);
				}
			}
		}
		// Miscellaneous
		{
			auto s_Miscellaneous = s_Core.add_space("Miscellaneous"_s);
			define_generic_class<Thread, GCDF::default_constructor>(s_Miscellaneous, "Thread"_s)
				.add_member_function_proxy<&Thread::joinable>("joinable"_s)
				.add_member_function_proxy<&Thread::join>("join"_s)
				.add_member_function_proxy<&Thread::detach>("detach"_s)
				.add_static_function_proxy<&Thread::yield>("yield"_s)
				.add_static_function_proxy<&Thread::sleep>("sleep"_s);
			define_generic_class<Context, GCDF::none_mask>(s_Miscellaneous, "Context"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> &            thix,
						JavaScript::NativeValueHandler<VCharacterListView> & script,
						JavaScript::NativeValueHandler<String> &             name
					) -> JavaScript::Value {
							return thix.value().evaluate(down_cast<VStringView>(script.value()), name.value());
						}
					>
				>("evaluate"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> &      thix,
						JavaScript::NativeValueHandler<List<String>> & argument
					) -> JavaScript::NativeValueHandler<List<String>> {
							return JavaScript::NativeValueHandler<List<String>>::new_instance_allocate(thix.value().callback(argument.value()));
						}
					>
				>("callback"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> & thix
					) -> JavaScript::NativeValueHandler<Context> {
							return JavaScript::NativeValueHandler<Context>::new_instance_allocate(thix.value().spawn());
						}
					>
				>("spawn"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> & thix
					) -> JavaScript::NativeValueHandler<Boolean> {
							return JavaScript::NativeValueHandler<Boolean>::new_instance_allocate(thix.value().busy());
						}
					>
				>("busy"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> & thix,
						JavaScript::NativeValueHandler<Thread> &  thread,
						JavaScript::Value &                       executor
					) -> Void {
							return thix.value().execute(thread.value(), executor);
						}
					>
				>("execute"_s);
			s_Miscellaneous
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
						JavaScript::NativeValueHandler<VCharacterListView> & it
					) -> VStringView& {
							// NOTE : return StringView is cheap
							return down_cast<VStringView>(it.value());
						}
					>
				>("cast_CharacterListView_to_JS_String"_s);
			s_Miscellaneous.add_variable("g_version"_s, context.context().new_value(JavaScript::NativeValueHandler<Size>::new_instance_allocate(mbw<Size>(M_version))));
			s_Miscellaneous.add_variable("g_context"_s, context.context().new_value(JavaScript::NativeValueHandler<Context>::new_reference(context)));
			// TODO : can not access the value at other thread
			s_Miscellaneous.add_variable("g_byte_stream_use_big_endian"_s, context.context().new_value(JavaScript::NativeValueHandler<Boolean>::new_reference(g_byte_stream_use_big_endian)));
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
