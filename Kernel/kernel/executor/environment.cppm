module;

#include "kernel/common.hpp"

export module twinning.kernel.executor.environment;
import twinning.kernel.utility;
import twinning.kernel.executor.context;
import twinning.kernel.tool.data.hash.fnv.common;
import twinning.kernel.tool.data.hash.fnv.hash;
import twinning.kernel.tool.data.hash.md5.common;
import twinning.kernel.tool.data.hash.md5.hash;
import twinning.kernel.tool.data.encoding.base64.common;
import twinning.kernel.tool.data.encoding.base64.encode;
import twinning.kernel.tool.data.encoding.base64.decode;
import twinning.kernel.tool.data.encryption.exor.common;
import twinning.kernel.tool.data.encryption.exor.encrypt;
import twinning.kernel.tool.data.encryption.rijndael.common;
import twinning.kernel.tool.data.encryption.rijndael.encrypt;
import twinning.kernel.tool.data.encryption.rijndael.decrypt;
import twinning.kernel.tool.data.compression.deflate.common;
import twinning.kernel.tool.data.compression.deflate.compress;
import twinning.kernel.tool.data.compression.deflate.uncompress;
import twinning.kernel.tool.data.compression.bzip2.common;
import twinning.kernel.tool.data.compression.bzip2.compress;
import twinning.kernel.tool.data.compression.bzip2.uncompress;
import twinning.kernel.tool.data.compression.lzma.common;
import twinning.kernel.tool.data.compression.lzma.compress;
import twinning.kernel.tool.data.compression.lzma.uncompress;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.tool.data.differentiation.vcdiff.encode;
import twinning.kernel.tool.data.differentiation.vcdiff.decode;
import twinning.kernel.tool.data.serialization.json.common;
import twinning.kernel.tool.data.serialization.json.write;
import twinning.kernel.tool.data.serialization.json.read;
import twinning.kernel.tool.data.serialization.xml.common;
import twinning.kernel.tool.data.serialization.xml.write;
import twinning.kernel.tool.data.serialization.xml.read;
import twinning.kernel.tool.texture.transformation.common;
import twinning.kernel.tool.texture.transformation.flip;
import twinning.kernel.tool.texture.transformation.scale;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.tool.texture.compression.etc.compress;
import twinning.kernel.tool.texture.compression.etc.uncompress;
import twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.tool.texture.compression.pvrtc.compress;
import twinning.kernel.tool.texture.compression.pvrtc.uncompress;
import twinning.kernel.tool.texture.file.png.common;
import twinning.kernel.tool.texture.file.png.write;
import twinning.kernel.tool.texture.file.png.read;
import twinning.kernel.tool.wwise.sound_bank.version;
import twinning.kernel.tool.wwise.sound_bank.definition;
import twinning.kernel.tool.wwise.sound_bank.encode;
import twinning.kernel.tool.wwise.sound_bank.decode;
import twinning.kernel.tool.marmalade.dzip.version;
import twinning.kernel.tool.marmalade.dzip.definition;
import twinning.kernel.tool.marmalade.dzip.pack;
import twinning.kernel.tool.marmalade.dzip.unpack;
import twinning.kernel.tool.popcap.zlib.version;
import twinning.kernel.tool.popcap.zlib.compress;
import twinning.kernel.tool.popcap.zlib.uncompress;
import twinning.kernel.tool.popcap.crypt_data.version;
import twinning.kernel.tool.popcap.crypt_data.encrypt;
import twinning.kernel.tool.popcap.crypt_data.decrypt;
import twinning.kernel.tool.popcap.reflection_object_notation.version;
import twinning.kernel.tool.popcap.reflection_object_notation.encode;
import twinning.kernel.tool.popcap.reflection_object_notation.decode;
import twinning.kernel.tool.popcap.u_texture.version;
import twinning.kernel.tool.popcap.u_texture.encode;
import twinning.kernel.tool.popcap.u_texture.decode;
import twinning.kernel.tool.popcap.sexy_texture.version;
import twinning.kernel.tool.popcap.sexy_texture.encode;
import twinning.kernel.tool.popcap.sexy_texture.decode;
import twinning.kernel.tool.popcap.animation.version;
import twinning.kernel.tool.popcap.animation.definition;
import twinning.kernel.tool.popcap.animation.encode;
import twinning.kernel.tool.popcap.animation.decode;
import twinning.kernel.tool.popcap.re_animation.version;
import twinning.kernel.tool.popcap.re_animation.definition;
import twinning.kernel.tool.popcap.re_animation.encode;
import twinning.kernel.tool.popcap.re_animation.decode;
import twinning.kernel.tool.popcap.particle.version;
import twinning.kernel.tool.popcap.particle.definition;
import twinning.kernel.tool.popcap.particle.encode;
import twinning.kernel.tool.popcap.particle.decode;
import twinning.kernel.tool.popcap.trail.version;
import twinning.kernel.tool.popcap.trail.definition;
import twinning.kernel.tool.popcap.trail.encode;
import twinning.kernel.tool.popcap.trail.decode;
import twinning.kernel.tool.popcap.render_effect.version;
import twinning.kernel.tool.popcap.render_effect.definition;
import twinning.kernel.tool.popcap.render_effect.encode;
import twinning.kernel.tool.popcap.render_effect.decode;
import twinning.kernel.tool.popcap.particle_effect.version;
import twinning.kernel.tool.popcap.particle_effect.definition;
import twinning.kernel.tool.popcap.particle_effect.encode;
import twinning.kernel.tool.popcap.particle_effect.decode;
import twinning.kernel.tool.popcap.character_font_widget_2.version;
import twinning.kernel.tool.popcap.character_font_widget_2.definition;
import twinning.kernel.tool.popcap.character_font_widget_2.encode;
import twinning.kernel.tool.popcap.character_font_widget_2.decode;
import twinning.kernel.tool.popcap.package.version;
import twinning.kernel.tool.popcap.package.definition;
import twinning.kernel.tool.popcap.package.pack;
import twinning.kernel.tool.popcap.package.unpack;
import twinning.kernel.tool.popcap.resource_stream_group.version;
import twinning.kernel.tool.popcap.resource_stream_group.definition;
import twinning.kernel.tool.popcap.resource_stream_group.pack;
import twinning.kernel.tool.popcap.resource_stream_group.unpack;
import twinning.kernel.tool.popcap.resource_stream_bundle.version;
import twinning.kernel.tool.popcap.resource_stream_bundle.definition;
import twinning.kernel.tool.popcap.resource_stream_bundle.manifest;
import twinning.kernel.tool.popcap.resource_stream_bundle.pack;
import twinning.kernel.tool.popcap.resource_stream_bundle.unpack;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.version;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.encode;
import twinning.kernel.tool.popcap.resource_stream_bundle_patch.decode;
import twinning.kernel.tool.miscellaneous.xbox_tiled_texture.common;
import twinning.kernel.tool.miscellaneous.xbox_tiled_texture.encode;
import twinning.kernel.tool.miscellaneous.xbox_tiled_texture.decode;
import twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.common;
import twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.encode;
import twinning.kernel.tool.miscellaneous.pvz2_cn_alpha_palette_texture.decode;
import twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.common;
import twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.encrypt;
import twinning.kernel.tool.miscellaneous.pvz2_cn_crypt_data.decrypt;

export namespace Twinning::Kernel::Executor::Environment {

	#pragma region define generic class

	enum class GenericClassDefinitionFlag : ZIntegerU8 {
		// generic operation
		default_constructor = 1 << 0,
		copy_constructor    = 1 << 1,
		// value operation
		value_constructor = 1 << 2,
		value_getter      = 1 << 3,
		value_setter      = 1 << 4,
		// mask
		none_mask    = 0b000'00,
		all_mask     = 0b111'11,
		generic_mask = default_constructor | copy_constructor,
		value_mask   = value_constructor | value_getter | value_setter,
		default_mask = generic_mask | value_mask,
	};

	// ----------------

	inline constexpr auto operator | (
		GenericClassDefinitionFlag const & thix,
		GenericClassDefinitionFlag const & that
	) -> GenericClassDefinitionFlag {
		return static_cast<GenericClassDefinitionFlag>(static_cast<std::underlying_type_t<GenericClassDefinitionFlag>>(thix) | static_cast<std::underlying_type_t<GenericClassDefinitionFlag>>(that));
	}

	inline constexpr auto operator & (
		GenericClassDefinitionFlag const & thix,
		GenericClassDefinitionFlag const & that
	) -> GenericClassDefinitionFlag {
		return static_cast<GenericClassDefinitionFlag>(static_cast<std::underlying_type_t<GenericClassDefinitionFlag>>(thix) & static_cast<std::underlying_type_t<GenericClassDefinitionFlag>>(that));
	}

	inline constexpr auto operator * (
		GenericClassDefinitionFlag const & thix,
		GenericClassDefinitionFlag const & that
	) -> Boolean {
		return mbox<Boolean>(static_cast<std::underlying_type_t<GenericClassDefinitionFlag>>(thix) & static_cast<std::underlying_type_t<GenericClassDefinitionFlag>>(that));
	}

	// ----------------

	template <typename Class, auto flag = GenericClassDefinitionFlag::default_mask> requires
		CategoryConstraint<IsPureInstance<Class>>
		&& (IsSameOf<flag, GenericClassDefinitionFlag>)
	inline auto define_generic_class (
		JavaScript::NativeSpaceBuilder & space,
		String const &                   name
	) -> JavaScript::NativeClassBuilder<Class> {
		auto builder = space.add_class<Class>(name);
		builder.template set_constructor<
			&normalized_lambda<
				[] (
			) -> JavaScript::NativeValueHandler<Class> {
					throw UnsupportedException{};
				}
			>
		>();
		if constexpr (flag * GenericClassDefinitionFlag::default_constructor) {
			// NOTE: EXPLAIN: static default(): T;
			builder.template add_constructor_allocate_proxy<>("default"_s);
		}
		if constexpr (flag * GenericClassDefinitionFlag::copy_constructor) {
			// NOTE: EXPLAIN: static copy(it: T): T;
			builder.template add_constructor_allocate_proxy<Class const &>("copy"_s);
		}
		if constexpr (flag * GenericClassDefinitionFlag::value_constructor) {
			// NOTE: EXPLAIN: static value(it: typeof T.Value) : T;
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
		if constexpr (flag * GenericClassDefinitionFlag::value_getter || flag * GenericClassDefinitionFlag::value_setter) {
			static_assert(flag * GenericClassDefinitionFlag::value_getter);
			// NOTE: EXPLAIN: get value(): typeof T.Value;
			constexpr auto & getter = normalized_lambda<
				[] (
				JavaScript::NativeValueHandler<Class> & thix
			) -> Class & {
					// NOTE: EXPLAIN: return reference is cheap
					return thix.value();
				}
			>;
			if constexpr (!(flag * GenericClassDefinitionFlag::value_setter)) {
				builder.template add_getter<&getter>("value"_s);
			}
			else {
				// NOTE: EXPLAIN: set value(it: typeof T.Value);
				constexpr auto & setter = normalized_lambda<
					[] (
					JavaScript::NativeValueHandler<Class> & thix,
					Class &                                 value
				) -> Void {
						// NOTE: EXPLAIN: some type has not copy assignment
						restruct(thix.value(), value);
						return;
					}
				>;
				builder.template add_getter_setter<&getter, &setter>("value"_s);
			}
		}
		return builder;
	}

	template <typename Version, typename VersionPackage, typename Class> requires
		CategoryConstraint<IsPureInstance<Version> && IsPureInstance<VersionPackage> && IsPureInstance<Class>>
	inline auto define_variant_class_version_method (
		JavaScript::NativeClassBuilder<Class> & builder
	) -> JavaScript::NativeClassBuilder<Class> & {
		// NOTE: EXPLAIN: static json(json: JSON.Value, version: Version): T;
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
							json.to(it, mbox<Size>(index));
						}
					);
					return it;
				}
			>
		>;
		// NOTE: EXPLAIN: get_json(version: Version): JSON.Value;
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
							json.from(thix, mbox<Size>(index));
						}
					);
					return json;
				}
			>
		>;
		// NOTE: EXPLAIN: set_json(version: Version, value: JSON.Value);
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
							json.to(thix, mbox<Size>(index));
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

	#pragma region proxy function with promotion

	// NOTE: EXPLAIN: promotion these type to simplify interface
	// - *StringView          -> String                    -> String
	// - *ByteListView        -> VariableByteListView      -> ByteListView
	// - *ByteStreamView      -> AccessByteStreamView      -> ByteStreamView
	// - *CharacterListView   -> VariableCharacterListView -> CharacterListView
	// - *CharacterStreamView -> AccessCharacterStreamView -> CharacterStreamView
	// - Image::*ImageView    -> Image::VariableImageView  -> Image::ImageView

	namespace Detail {

		template <typename Type> requires
			NoneConstraint
		using AsPromotion = AsSwitch<
			IsSame<Type, ConstantStringView, VariableStringView>,
			String,
			AsSwitch<
				IsSame<Type, ConstantByteListView>,
				VariableByteListView,
				AsSwitch<
					IsSame<Type, InputByteStreamView, OutputByteStreamView>,
					AccessByteStreamView,
					AsSwitch<
						IsSame<Type, ConstantCharacterListView>,
						VariableCharacterListView,
						AsSwitch<
							IsSame<Type, InputCharacterStreamView, OutputCharacterStreamView>,
							AccessCharacterStreamView,
							AsSwitch<
								IsSame<Type, Image::ConstantImageView>,
								Image::VariableImageView,
								Type
							>
						>
					>
				>
			>
		>;

		template <auto function, auto ... index> requires
			NoneConstraint
		inline constexpr auto make_proxy_function_with_promotion (
			ValuePackage<index ...>
		) -> auto {
			if constexpr ((IsSame<AsPromotion<AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>>, AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>> && ...)) {
				return function;
			}
			else {
				return &proxy_global_function<function, AsPromotion<AsPure<typename CallableTraitOf<function>::Argument::template Element<index>>> & ...>;
			}
		}

	}

	// ----------------

	template <auto function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(function)>)
	inline constexpr auto & proxy_global_function_with_promotion = *Detail::make_proxy_function_with_promotion<function>(AsValuePackageOfIndex<CallableTraitOf<function>::Argument::size>{});

	template <typename Class, auto function> requires
		CategoryConstraint<>
		&& (IsMemberFunction<decltype(function)>)
		&& (IsDerivedFrom<Class, typename CallableTraitOf<function>::Class>)
	inline constexpr auto & proxy_member_function_with_promotion = proxy_global_function_with_promotion<normalized_member_function<function, Class>>;

	#pragma endregion

	#pragma region inject

	inline auto inject (
		Context & context
	) -> Void {
		auto s_Twinning = JavaScript::NativeSpaceBuilder{k_null_optional, "Twinning"_s, as_left(context.context().global_object())};
		auto s_Kernel = s_Twinning.add_space("Kernel"_s);
		// Boolean
		define_generic_class<Boolean>(s_Kernel, "Boolean"_s);
		// Integer
		define_generic_class<IntegerU32>(s_Kernel, "IntegerU32"_s);
		// Size
		define_generic_class<Size>(s_Kernel, "Size"_s);
		define_generic_class<Optional<Size>>(s_Kernel, "SizeOptional"_s);
		// Byte
		define_generic_class<Byte>(s_Kernel, "Byte"_s);
		// String
		define_generic_class<String>(s_Kernel, "String"_s);
		define_generic_class<Optional<String>>(s_Kernel, "StringOptional"_s);
		define_generic_class<List<String>>(s_Kernel, "StringList"_s);
		// Path
		define_generic_class<Path>(s_Kernel, "Path"_s);
		define_generic_class<Optional<Path>>(s_Kernel, "PathOptional"_s);
		define_generic_class<List<Path>>(s_Kernel, "PathList"_s);
		// ByteArray
		define_generic_class<ByteArray>(s_Kernel, "ByteArray"_s)
			.add_constructor_allocate_proxy<Size const &>("allocate"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<ByteArray, &ByteArray::allocate>>("allocate"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<ByteArray, &ByteArray::reset>>("reset"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<ByteArray, &ByteArray::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<ByteArray, AsVariableMemberFunction<ByteArray, ByteArray::View>{&ByteArray::view}>>("view"_s)
			.add_member_function<
				&normalized_lambda<
					[] (
					JavaScript::NativeValueHandler<ByteArray> & that
				) -> ByteArray && {
						return as_moveable(that.value());
					}
				>
			>("release"_s);
		// ByteListView
		define_generic_class<VariableByteListView>(s_Kernel, "ByteListView"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableByteListView, &VariableByteListView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableByteListView, AsConstantMemberFunction<VariableByteListView, VariableByteListView, Size const &, Size const &>{&VariableByteListView::sub}>>("sub"_s);
		// ByteStreamView
		define_generic_class<AccessByteStreamView, GenericClassDefinitionFlag::generic_mask>(s_Kernel, "ByteStreamView"_s)
			.add_constructor_allocate_proxy<VariableByteListView const &>("watch"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessByteStreamView, &AccessByteStreamView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessByteStreamView, &AccessByteStreamView::position>>("position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessByteStreamView, &AccessByteStreamView::set_position>>("set_position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessByteStreamView, &AccessByteStreamView::view>>("view"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessByteStreamView, &AccessByteStreamView::stream_view>>("stream_view"_s);
		// CharacterListView
		define_generic_class<VariableCharacterListView, GenericClassDefinitionFlag::generic_mask>(s_Kernel, "CharacterListView"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableCharacterListView, &VariableCharacterListView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableCharacterListView, AsConstantMemberFunction<VariableCharacterListView, VariableCharacterListView, Size const &, Size const &>{&VariableCharacterListView::sub}>>("sub"_s);
		// CharacterStreamView
		define_generic_class<AccessCharacterStreamView, GenericClassDefinitionFlag::generic_mask>(s_Kernel, "CharacterStreamView"_s)
			.add_constructor_allocate_proxy<VariableCharacterListView const &>("watch"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessCharacterStreamView, &AccessCharacterStreamView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessCharacterStreamView, &AccessCharacterStreamView::position>>("position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessCharacterStreamView, &AccessCharacterStreamView::set_position>>("set_position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessCharacterStreamView, &AccessCharacterStreamView::view>>("view"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessCharacterStreamView, &AccessCharacterStreamView::stream_view>>("stream_view"_s);
		// JSON
		{
			auto s_JSON = s_Kernel.add_space("JSON"_s);
			define_generic_class<JSON::Value>(s_JSON, "Value"_s);
		}
		// XML
		{
			auto s_XML = s_Kernel.add_space("XML"_s);
			define_generic_class<XML::Node>(s_XML, "Node"_s);
		}
		// Image
		{
			auto s_Image = s_Kernel.add_space("Image"_s);
			define_generic_class<Image::ImageSize>(s_Image, "ImageSize"_s);
			define_generic_class<Image::ImagePosition>(s_Image, "ImagePosition"_s);
			define_generic_class<Image::Color>(s_Image, "Color"_s);
			define_generic_class<List<Image::Color>>(s_Image, "ColorList"_s);
			define_generic_class<Image::Pixel>(s_Image, "Pixel"_s);
			define_generic_class<Image::VariableImageView, GenericClassDefinitionFlag::generic_mask>(s_Image, "ImageView"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::size>>("size"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::fill>>("fill"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::draw>>("draw"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::sub>>("sub"_s);
			define_generic_class<Image::Image, GenericClassDefinitionFlag::generic_mask>(s_Image, "Image"_s)
				.add_constructor_allocate_proxy<Image::ImageSize const &>("allocate"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, &Image::Image::allocate>>("allocate"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, &Image::Image::reset>>("reset"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, &Image::Image::size>>("size"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, AsVariableMemberFunction<Image::Image, Image::VariableImageView>{&Image::Image::view}>>("view"_s);
		}
		// Storage
		{
			auto s_Storage = s_Kernel.add_space("Storage"_s);
			s_Storage
				// exist
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist>>("exist"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist_file>>("exist_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist_directory>>("exist_directory"_s)
				// basic
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::copy>>("copy"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::rename>>("rename"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::remove>>("remove"_s)
				// link
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_link>>("create_link"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::parse_link>>("parse_link"_s)
				// hard link
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_hard_link>>("create_hard_link"_s)
				// file
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_file>>("create_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::size_file>>("size_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::resize_file>>("resize_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::read_file>>("read_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::write_file>>("write_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::read_file_stream>>("read_file_stream"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::write_file_stream>>("write_file_stream"_s)
				// directory
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_directory>>("create_directory"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::list_directory>>("list_directory"_s);
		}
		// Process
		{
			auto s_Process = s_Kernel.add_space("Process"_s);
			s_Process
				// workspace
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::get_workspace>>("get_workspace"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::set_workspace>>("set_workspace"_s)
				// environment
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::get_environment>>("get_environment"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::set_environment>>("set_environment"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::list_environment>>("list_environment"_s)
				// process
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::run_process>>("run_process"_s)
				// command
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::execute_command>>("execute_command"_s);
		}
		// Tool
		{
			auto s_Tool = s_Kernel.add_space("Tool"_s);
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
							.add_function_proxy<&proxy_global_function_with_promotion<Tool::Data::Hash::FNV::Hash::process>>("process"_s);
					}
					{
						auto s_MD5 = s_Hash.add_space("MD5"_s);
						s_MD5.add_space("Hash"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Hash::MD5::Hash::process>>("process"_s);
					}
				}
				{
					auto s_Encoding = s_Data.add_space("Encoding"_s);
					{
						auto s_Base64 = s_Encoding.add_space("Base64"_s);
						s_Base64.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base64::Encode::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base64::Encode::estimate>>("estimate"_s);
						s_Base64.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base64::Decode::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base64::Decode::estimate>>("estimate"_s);
					}
				}
				{
					auto s_Encryption = s_Data.add_space("Encryption"_s);
					{
						auto s_EXOR = s_Encryption.add_space("EXOR"_s);
						s_EXOR.add_space("Encrypt"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encryption::EXOR::Encrypt::process>>("process"_s);
					}
					{
						auto s_Rijndael = s_Encryption.add_space("Rijndael"_s);
						define_generic_class<Tool::Data::Encryption::Rijndael::Mode>(s_Rijndael, "Mode"_s);
						s_Rijndael.add_space("Encrypt"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encryption::Rijndael::Encrypt::process>>("process"_s);
						s_Rijndael.add_space("Decrypt"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encryption::Rijndael::Decrypt::process>>("process"_s);
					}
				}
				{
					auto s_Compression = s_Data.add_space("Compression"_s);
					{
						auto s_Deflate = s_Compression.add_space("Deflate"_s);
						define_generic_class<Tool::Data::Compression::Deflate::Strategy>(s_Deflate, "Strategy"_s);
						define_generic_class<Tool::Data::Compression::Deflate::Wrapper>(s_Deflate, "Wrapper"_s);
						s_Deflate.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Deflate::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Deflate::Compress::estimate>>("estimate"_s);
						s_Deflate.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Deflate::Uncompress::process>>("process"_s);
					}
					{
						auto s_BZip2 = s_Compression.add_space("BZip2"_s);
						s_BZip2.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::BZip2::Compress::process>>("process"_s);
						s_BZip2.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::BZip2::Uncompress::process>>("process"_s);
					}
					{
						auto s_Lzma = s_Compression.add_space("Lzma"_s);
						s_Lzma.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma::Compress::process>>("process"_s);
						s_Lzma.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma::Uncompress::process>>("process"_s);
					}
				}
				{
					auto s_Differentiation = s_Data.add_space("Differentiation"_s);
					{
						auto s_VCDiff = s_Differentiation.add_space("VCDiff"_s);
						s_VCDiff.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Differentiation::VCDiff::Encode::process>>("process"_s);
						s_VCDiff.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Differentiation::VCDiff::Decode::process>>("process"_s);
					}
				}
				{
					auto s_Serialization = s_Data.add_space("Serialization"_s);
					{
						auto s_JSON = s_Serialization.add_space("JSON"_s);
						s_JSON.add_space("Write"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::JSON::Write::process>>("process"_s);
						s_JSON.add_space("Read"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::JSON::Read::process>>("process"_s);
					}
					{
						auto s_XML = s_Serialization.add_space("XML"_s);
						s_XML.add_space("Write"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::XML::Write::process>>("process"_s);
						s_XML.add_space("Read"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::XML::Read::process>>("process"_s);
					}
				}
			}
			// Texture
			{
				auto s_Texture = s_Tool.add_space("Texture"_s);
				{
					auto s_Transformation = s_Texture.add_space("Transformation"_s);
					s_Transformation.add_space("Flip"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Flip::process>>("process"_s);
					s_Transformation.add_space("Scale"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Scale::process>>("process"_s);
				}
				{
					auto s_Encoding = s_Texture.add_space("Encoding"_s);
					define_generic_class<Tool::Texture::Encoding::Format>(s_Encoding, "Format"_s);
					s_Encoding.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Encoding::Encode::process>>("process"_s);
					s_Encoding.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Encoding::Decode::process>>("process"_s);
				}
				{
					auto s_Compression = s_Texture.add_space("Compression"_s);
					{
						auto s_ETC = s_Compression.add_space("ETC"_s);
						define_generic_class<Tool::Texture::Compression::ETC::Format>(s_ETC, "Format"_s);
						s_ETC.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::ETC::Compress::process>>("process"_s);
						s_ETC.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::ETC::Uncompress::process>>("process"_s);
					}
					{
						auto s_PVRTC = s_Compression.add_space("PVRTC"_s);
						define_generic_class<Tool::Texture::Compression::PVRTC::Format>(s_PVRTC, "Format"_s);
						s_PVRTC.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::PVRTC::Compress::process>>("process"_s);
						s_PVRTC.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::PVRTC::Uncompress::process>>("process"_s);
					}
				}
				{
					auto s_File = s_Texture.add_space("File"_s);
					{
						auto s_PNG = s_File.add_space("PNG"_s);
						s_PNG.add_space("Write"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::File::PNG::Write::process>>("process"_s);
						s_PNG.add_space("Read"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::File::PNG::Read::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::File::PNG::Read::estimate>>("estimate"_s);
					}
				}
			}
			// Wwise
			{
				auto s_Wwise = s_Tool.add_space("Wwise"_s);
				{
					using Tool::Wwise::SoundBank::Version;
					using Tool::Wwise::SoundBank::VersionPackage;
					using Tool::Wwise::SoundBank::Definition;
					using SoundBankDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<2_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<3_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<4_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<5_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<6_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<7_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<8_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<9_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<10_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<11_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<12_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<13_ixz>>::SoundBank,
						typename Definition<VersionPackage::element<14_ixz>>::SoundBank
					>;
					auto s_SoundBank = s_Wwise.add_space("SoundBank"_s);
					define_generic_class<Version>(s_SoundBank, "Version"_s);
					{
						auto s_Definition = s_SoundBank.add_space("Definition"_s);
						auto c_SoundBank = define_generic_class<SoundBankDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "SoundBank"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_SoundBank);
					}
					s_SoundBank.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &      data,
							SoundBankDefinition const & definition,
							Path const &                embedded_media_directory,
							Version const &             version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::SoundBank::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>(), embedded_media_directory);
									}
								);
							}
						>>>("process"_s);
					s_SoundBank.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  data,
							SoundBankDefinition &  definition,
							Optional<Path> const & embedded_media_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Wwise::SoundBank::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>(), embedded_media_directory);
									}
								);
							}
						>>>("process"_s);
				}
			}
			// Marmalade
			{
				auto s_Marmalade = s_Tool.add_space("Marmalade"_s);
				{
					using Tool::Marmalade::DZip::Version;
					using Tool::Marmalade::DZip::VersionPackage;
					using Tool::Marmalade::DZip::Definition;
					using PackageDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Package
					>;
					auto s_DZip = s_Marmalade.add_space("DZip"_s);
					define_generic_class<Version>(s_DZip, "Version"_s);
					{
						auto s_Definition = s_DZip.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_DZip.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &    data,
							PackageDefinition const & definition,
							Path const &              resource_directory,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Marmalade::DZip::Pack<version>::process(data, definition.template get_of_index<mbox<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process"_s);
					s_DZip.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  data,
							PackageDefinition &    definition,
							Optional<Path> const & resource_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::Marmalade::DZip::Unpack<version>::process(data, definition.template set_of_index<mbox<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process"_s);
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
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &                              raw,
							OutputByteStreamView &                             ripe,
							Size const &                                       level,
							Size const &                                       window_bits,
							Size const &                                       memory_level,
							Tool::Data::Compression::Deflate::Strategy const & strategy,
							Version const &                                    version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Compress<version>::process(raw, ripe, level, window_bits, memory_level, strategy);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
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
										Tool::PopCap::ZLib::Compress<version>::estimate(raw_size, ripe_size_bound, window_bits, memory_level);
									}
								);
							}
						>>>("estimate"_s);
					s_ZLib.add_space("Uncompress"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  ripe,
							OutputByteStreamView & raw,
							Size const &           window_bits,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Uncompress<version>::process(ripe, raw, window_bits);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							ConstantByteListView const & ripe,
							Size &                       raw_size,
							Version const &              version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ZLib::Uncompress<version>::estimate(ripe, raw_size);
									}
								);
							}
						>>>("estimate"_s);
				}
				{
					using Tool::PopCap::CryptData::Version;
					using Tool::PopCap::CryptData::VersionPackage;
					auto s_CryptData = s_PopCap.add_space("CryptData"_s);
					define_generic_class<Version>(s_CryptData, "Version"_s);
					s_CryptData.add_space("Encrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  plain,
							OutputByteStreamView & cipher,
							Size const &           limit,
							String const &         key,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Encrypt<version>::process(plain, cipher, limit, key);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							Size const &    plain_size,
							Size &          cipher_size,
							Size const &    limit,
							Version const & version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Encrypt<version>::estimate(plain_size, cipher_size, limit);
									}
								);
							}
						>>>("estimate"_s);
					s_CryptData.add_space("Decrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  cipher,
							OutputByteStreamView & plain,
							Size const &           limit,
							String const &         key,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Decrypt<version>::process(cipher, plain, limit, key);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							ConstantByteListView const & cipher,
							Size &                       plain_size,
							Size const &                 limit,
							Version const &              version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CryptData::Decrypt<version>::estimate(cipher, plain_size, limit);
									}
								);
							}
						>>>("estimate"_s);
				}
				{
					using Tool::PopCap::ReflectionObjectNotation::Version;
					using Tool::PopCap::ReflectionObjectNotation::VersionPackage;
					auto s_ReflectionObjectNotation = s_PopCap.add_space("ReflectionObjectNotation"_s);
					define_generic_class<Version>(s_ReflectionObjectNotation, "Version"_s);
					s_ReflectionObjectNotation.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView & data,
							JSON::Value const &    definition,
							Boolean const &        enable_string_index,
							Boolean const &        enable_reference,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReflectionObjectNotation::Encode<version>::process(data, definition, enable_string_index, enable_reference);
									}
								);
							}
						>>>("process"_s);
					s_ReflectionObjectNotation.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							JSON::Value &         definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReflectionObjectNotation::Decode<version>::process(data, definition);
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::UTexture::Version;
					using Tool::PopCap::UTexture::VersionPackage;
					auto s_UTexture = s_PopCap.add_space("UTexture"_s);
					define_generic_class<Version>(s_UTexture, "Version"_s);
					s_UTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &                  data,
							Image::ConstantImageView const &        image,
							Tool::Texture::Encoding::Format const & format,
							Version const &                         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Encode<version>::process(data, image, format);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							Size &                                  data_size_bound,
							Image::ImageSize const &                image_size,
							Tool::Texture::Encoding::Format const & format,
							Version const &                         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Encode<version>::estimate(data_size_bound, image_size, format);
									}
								);
							}
						>>>("estimate"_s);
					s_UTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &            data,
							Image::VariableImageView const & image,
							Version const &                  version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Decode<version>::process(data, image);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							ConstantByteListView & data,
							Image::ImageSize &     image_size,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::UTexture::Decode<version>::estimate(data, image_size);
									}
								);
							}
						>>>("estimate"_s);
				}
				{
					using Tool::PopCap::SexyTexture::Version;
					using Tool::PopCap::SexyTexture::VersionPackage;
					auto s_SexyTexture = s_PopCap.add_space("SexyTexture"_s);
					define_generic_class<Version>(s_SexyTexture, "Version"_s);
					s_SexyTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &                  data,
							Image::ConstantImageView const &        image,
							Tool::Texture::Encoding::Format const & format,
							Boolean const &                         compress_texture_data,
							Version const &                         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Encode<version>::process(data, image, format, compress_texture_data);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							Size &                                  data_size_bound,
							Image::ImageSize const &                image_size,
							Tool::Texture::Encoding::Format const & format,
							Boolean const &                         compress_texture_data,
							Version const &                         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Encode<version>::estimate(data_size_bound, image_size, format, compress_texture_data);
									}
								);
							}
						>>>("estimate"_s);
					s_SexyTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &            data,
							Image::VariableImageView const & image,
							Version const &                  version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Decode<version>::process(data, image);
									}
								);
							}
						>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							ConstantByteListView & data,
							Image::ImageSize &     image_size,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::SexyTexture::Decode<version>::estimate(data, image_size);
									}
								);
							}
						>>>("estimate"_s);
				}
				{
					using Tool::PopCap::Animation::Version;
					using Tool::PopCap::Animation::VersionPackage;
					using Tool::PopCap::Animation::Definition;
					using AnimationDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Animation,
						typename Definition<VersionPackage::element<2_ixz>>::Animation,
						typename Definition<VersionPackage::element<3_ixz>>::Animation,
						typename Definition<VersionPackage::element<4_ixz>>::Animation,
						typename Definition<VersionPackage::element<5_ixz>>::Animation,
						typename Definition<VersionPackage::element<6_ixz>>::Animation
					>;
					auto s_Animation = s_PopCap.add_space("Animation"_s);
					define_generic_class<Version>(s_Animation, "Version"_s);
					{
						auto s_Definition = s_Animation.add_space("Definition"_s);
						auto c_Animation = define_generic_class<AnimationDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					s_Animation.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &      data,
							AnimationDefinition const & definition,
							Version const &             version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Animation::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_Animation.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							AnimationDefinition & definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Animation::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::ReAnimation::Version;
					using Tool::PopCap::ReAnimation::VersionPackage;
					using Tool::PopCap::ReAnimation::Definition;
					using AnimationDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Animation,
						typename Definition<VersionPackage::element<2_ixz>>::Animation,
						typename Definition<VersionPackage::element<3_ixz>>::Animation,
						typename Definition<VersionPackage::element<4_ixz>>::Animation
					>;
					auto s_ReAnimation = s_PopCap.add_space("ReAnimation"_s);
					define_generic_class<Version>(s_ReAnimation, "Version"_s);
					{
						auto s_Definition = s_ReAnimation.add_space("Definition"_s);
						auto c_Animation = define_generic_class<AnimationDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					s_ReAnimation.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &      data,
							AnimationDefinition const & definition,
							Version const &             version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReAnimation::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_ReAnimation.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							AnimationDefinition & definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ReAnimation::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::Particle::Version;
					using Tool::PopCap::Particle::VersionPackage;
					using Tool::PopCap::Particle::Definition;
					using ParticleDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Particle,
						typename Definition<VersionPackage::element<2_ixz>>::Particle,
						typename Definition<VersionPackage::element<3_ixz>>::Particle,
						typename Definition<VersionPackage::element<4_ixz>>::Particle
					>;
					auto s_Particle = s_PopCap.add_space("Particle"_s);
					define_generic_class<Version>(s_Particle, "Version"_s);
					{
						auto s_Definition = s_Particle.add_space("Definition"_s);
						auto c_Particle = define_generic_class<ParticleDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Particle"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Particle);
					}
					s_Particle.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &     data,
							ParticleDefinition const & definition,
							Version const &            version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Particle::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_Particle.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							ParticleDefinition &  definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Particle::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::Trail::Version;
					using Tool::PopCap::Trail::VersionPackage;
					using Tool::PopCap::Trail::Definition;
					using TrailDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Trail,
						typename Definition<VersionPackage::element<2_ixz>>::Trail,
						typename Definition<VersionPackage::element<3_ixz>>::Trail,
						typename Definition<VersionPackage::element<4_ixz>>::Trail
					>;
					auto s_Trail = s_PopCap.add_space("Trail"_s);
					define_generic_class<Version>(s_Trail, "Version"_s);
					{
						auto s_Definition = s_Trail.add_space("Definition"_s);
						auto c_Trail = define_generic_class<TrailDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Trail"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Trail);
					}
					s_Trail.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &  data,
							TrailDefinition const & definition,
							Version const &         version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Trail::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_Trail.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							TrailDefinition &     definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Trail::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::RenderEffect::Version;
					using Tool::PopCap::RenderEffect::VersionPackage;
					using Tool::PopCap::RenderEffect::Definition;
					using EffectDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Effect,
						typename Definition<VersionPackage::element<2_ixz>>::Effect,
						typename Definition<VersionPackage::element<3_ixz>>::Effect
					>;
					auto s_RenderEffect = s_PopCap.add_space("RenderEffect"_s);
					define_generic_class<Version>(s_RenderEffect, "Version"_s);
					{
						auto s_Definition = s_RenderEffect.add_space("Definition"_s);
						auto c_Effect = define_generic_class<EffectDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Effect"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Effect);
					}
					s_RenderEffect.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &   data,
							EffectDefinition const & definition,
							Version const &          version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::RenderEffect::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_RenderEffect.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							EffectDefinition &    definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::RenderEffect::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::ParticleEffect::Version;
					using Tool::PopCap::ParticleEffect::VersionPackage;
					using Tool::PopCap::ParticleEffect::Definition;
					using EffectDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Effect
					>;
					auto s_ParticleEffect = s_PopCap.add_space("ParticleEffect"_s);
					define_generic_class<Version>(s_ParticleEffect, "Version"_s);
					{
						auto s_Definition = s_ParticleEffect.add_space("Definition"_s);
						auto c_Effect = define_generic_class<EffectDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Effect"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Effect);
					}
					s_ParticleEffect.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &   data,
							EffectDefinition const & definition,
							Version const &          version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ParticleEffect::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_ParticleEffect.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView & data,
							EffectDefinition &    definition,
							Version const &       version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ParticleEffect::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::CharacterFontWidget2::Version;
					using Tool::PopCap::CharacterFontWidget2::VersionPackage;
					using Tool::PopCap::CharacterFontWidget2::Definition;
					using FontWidgetDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::FontWidget
					>;
					auto s_CharacterFontWidget2 = s_PopCap.add_space("CharacterFontWidget2"_s);
					define_generic_class<Version>(s_CharacterFontWidget2, "Version"_s);
					{
						auto s_Definition = s_CharacterFontWidget2.add_space("Definition"_s);
						auto c_FontWidget = define_generic_class<FontWidgetDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "FontWidget"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_FontWidget);
					}
					s_CharacterFontWidget2.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &       data,
							FontWidgetDefinition const & definition,
							Version const &              version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CharacterFontWidget2::Encode<version>::process(data, definition.template get_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
					s_CharacterFontWidget2.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  data,
							FontWidgetDefinition & definition,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::CharacterFontWidget2::Decode<version>::process(data, definition.template set_of_index<mbox<Size>(index)>());
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::Package::Version;
					using Tool::PopCap::Package::VersionPackage;
					using Tool::PopCap::Package::Definition;
					using PackageDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Package,
						typename Definition<VersionPackage::element<2_ixz>>::Package
					>;
					auto s_Package = s_PopCap.add_space("Package"_s);
					define_generic_class<Version>(s_Package, "Version"_s);
					{
						auto s_Definition = s_Package.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_Package.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &    data,
							PackageDefinition const & definition,
							Path const &              resource_directory,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Package::Pack<version>::process(data, definition.template get_of_index<mbox<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process"_s);
					s_Package.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  data,
							PackageDefinition &    definition,
							Optional<Path> const & resource_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::Package::Unpack<version>::process(data, definition.template set_of_index<mbox<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::ResourceStreamGroup::Version;
					using Tool::PopCap::ResourceStreamGroup::VersionPackage;
					using Tool::PopCap::ResourceStreamGroup::Definition;
					using PackageDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Package,
						typename Definition<VersionPackage::element<2_ixz>>::Package,
						typename Definition<VersionPackage::element<3_ixz>>::Package
					>;
					auto s_ResourceStreamGroup = s_PopCap.add_space("ResourceStreamGroup"_s);
					define_generic_class<Version>(s_ResourceStreamGroup, "Version"_s);
					{
						auto s_Definition = s_ResourceStreamGroup.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_ResourceStreamGroup.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &    data,
							PackageDefinition const & definition,
							Path const &              resource_directory,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamGroup::Pack<version>::process(data, definition.template get_of_index<mbox<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process"_s);
					s_ResourceStreamGroup.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  data,
							PackageDefinition &    definition,
							Optional<Path> const & resource_directory,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamGroup::Unpack<version>::process(data, definition.template set_of_index<mbox<Size>(index)>(), resource_directory);
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::ResourceStreamBundle::Version;
					using Tool::PopCap::ResourceStreamBundle::VersionPackage;
					using Tool::PopCap::ResourceStreamBundle::Definition;
					using Tool::PopCap::ResourceStreamBundle::Manifest;
					using PackageDefinition = Variant<
						typename Definition<VersionPackage::element<1_ixz>>::Package,
						typename Definition<VersionPackage::element<2_ixz>>::Package,
						typename Definition<VersionPackage::element<3_ixz>>::Package,
						typename Definition<VersionPackage::element<4_ixz>>::Package,
						typename Definition<VersionPackage::element<5_ixz>>::Package,
						typename Definition<VersionPackage::element<6_ixz>>::Package
					>;
					using PackageManifestOptional = Variant<
						Optional<typename Manifest<VersionPackage::element<1_ixz>>::Package>,
						Optional<typename Manifest<VersionPackage::element<2_ixz>>::Package>,
						Optional<typename Manifest<VersionPackage::element<3_ixz>>::Package>,
						Optional<typename Manifest<VersionPackage::element<4_ixz>>::Package>,
						Optional<typename Manifest<VersionPackage::element<5_ixz>>::Package>,
						Optional<typename Manifest<VersionPackage::element<6_ixz>>::Package>
					>;
					auto s_ResourceStreamBundle = s_PopCap.add_space("ResourceStreamBundle"_s);
					define_generic_class<Version>(s_ResourceStreamBundle, "Version"_s);
					{
						auto s_Definition = s_ResourceStreamBundle.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::generic_mask>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					{
						auto s_Manifest = s_ResourceStreamBundle.add_space("Manifest"_s);
						auto c_PackageOptional = define_generic_class<PackageManifestOptional, GenericClassDefinitionFlag::generic_mask>(s_Manifest, "PackageOptional"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_PackageOptional);
					}
					s_ResourceStreamBundle.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							OutputByteStreamView &          data,
							PackageDefinition const &       definition,
							PackageManifestOptional const & manifest,
							Path const &                    resource_directory,
							Optional<Path> const &          packet_file,
							Optional<Path> const &          new_packet_file,
							Version const &                 version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundle::Pack<version>::process(data, definition.template get_of_index<mbox<Size>(index)>(), manifest.template get_of_index<mbox<Size>(index)>(), resource_directory, packet_file, new_packet_file);
									}
								);
							}
						>>>("process"_s);
					s_ResourceStreamBundle.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &     data,
							PackageDefinition &       definition,
							PackageManifestOptional & manifest,
							Optional<Path> const &    resource_directory,
							Optional<Path> const &    packet_file,
							Version const &           version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundle::Unpack<version>::process(data, definition.template set_of_index<mbox<Size>(index)>(), manifest.template set_of_index<mbox<Size>(index)>(), resource_directory, packet_file);
									}
								);
							}
						>>>("process"_s);
				}
				{
					using Tool::PopCap::ResourceStreamBundlePatch::Version;
					using Tool::PopCap::ResourceStreamBundlePatch::VersionPackage;
					auto s_ResourceStreamBundlePatch = s_PopCap.add_space("ResourceStreamBundlePatch"_s);
					define_generic_class<Version>(s_ResourceStreamBundlePatch, "Version"_s);
					s_ResourceStreamBundlePatch.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  before,
							InputByteStreamView &  after,
							OutputByteStreamView & patch,
							Boolean const &        use_raw_packet,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundlePatch::Encode<version>::process(before, after, patch, use_raw_packet);
									}
								);
							}
						>>>("process"_s);
					s_ResourceStreamBundlePatch.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&normalized_lambda<
							[] (
							InputByteStreamView &  before,
							OutputByteStreamView & after,
							InputByteStreamView &  patch,
							Boolean const &        use_raw_packet,
							Version const &        version
						) -> Void {
								Generalization::match<VersionPackage>(
									version,
									[&] <auto index, auto version> (ValuePackage<index>, ValuePackage<version>) {
										Tool::PopCap::ResourceStreamBundlePatch::Decode<version>::process(before, after, patch, use_raw_packet);
									}
								);
							}
						>>>("process"_s);
				}
			}
			// Miscellaneous
			{
				auto s_Miscellaneous = s_Tool.add_space("Miscellaneous"_s);
				{
					auto s_XboxTiledTexture = s_Miscellaneous.add_space("XboxTiledTexture"_s);
					s_XboxTiledTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::XboxTiledTexture::Encode::process>>("process"_s);
					s_XboxTiledTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::XboxTiledTexture::Decode::process>>("process"_s);
				}
				{
					auto s_PvZ2CNAlphaPaletteTexture = s_Miscellaneous.add_space("PvZ2CNAlphaPaletteTexture"_s);
					s_PvZ2CNAlphaPaletteTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::PvZ2CNAlphaPaletteTexture::Encode::process>>("process"_s);
					s_PvZ2CNAlphaPaletteTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::PvZ2CNAlphaPaletteTexture::Decode::process>>("process"_s);
				}
				{
					auto s_PvZ2CNCryptData = s_Miscellaneous.add_space("PvZ2CNCryptData"_s);
					s_PvZ2CNCryptData.add_space("Encrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::PvZ2CNCryptData::Encrypt::process>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::PvZ2CNCryptData::Encrypt::estimate>>("estimate"_s);
					s_PvZ2CNCryptData.add_space("Decrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::PvZ2CNCryptData::Decrypt::process>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::PvZ2CNCryptData::Decrypt::estimate>>("estimate"_s);
				}
			}
		}
		// Miscellaneous
		{
			auto s_Miscellaneous = s_Kernel.add_space("Miscellaneous"_s);
			define_generic_class<Thread::Thread, GenericClassDefinitionFlag::default_constructor>(s_Miscellaneous, "Thread"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Thread::Thread, &Thread::Thread::joinable>>("joinable"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Thread::Thread, &Thread::Thread::join>>("join"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Thread::Thread, &Thread::Thread::detach>>("detach"_s)
				.add_static_function_proxy<&proxy_global_function_with_promotion<&Thread::Thread::yield>>("yield"_s)
				.add_static_function_proxy<&proxy_global_function_with_promotion<&Thread::Thread::sleep>>("sleep"_s);
			define_generic_class<Context, GenericClassDefinitionFlag::none_mask>(s_Miscellaneous, "Context"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> &                   thix,
						JavaScript::NativeValueHandler<VariableCharacterListView> & script,
						JavaScript::NativeValueHandler<String> &                    name,
						JavaScript::NativeValueHandler<Boolean> &                   is_module
					) -> JavaScript::Value {
							return thix.value().evaluate(down_cast<VariableStringView>(script.value()), name.value(), is_module.value());
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
						JavaScript::NativeValueHandler<Context> &        thix,
						JavaScript::NativeValueHandler<Thread::Thread> & thread,
						JavaScript::Value &                              executor
					) -> Void {
							return thix.value().execute(thread.value(), executor);
						}
					>
				>("execute"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> & thix
					) -> JavaScript::NativeValueHandler<Optional<Path>> {
							return JavaScript::NativeValueHandler<Optional<Path>>::new_reference(thix.value().query_module_home());
						}
					>
				>("query_module_home"_s)
				.add_member_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<Context> & thix
					) -> JavaScript::NativeValueHandler<Boolean> {
							return JavaScript::NativeValueHandler<Boolean>::new_reference(thix.value().query_byte_stream_use_big_endian());
						}
					>
				>("query_byte_stream_use_big_endian"_s);
			s_Miscellaneous
				.add_function_proxy<
					&normalized_lambda<
						[] (
						VariableByteListView & it
					) -> VariableCharacterListView {
							return self_cast<VariableCharacterListView>(it);
						}
					>
				>("cast_ByteListView_to_CharacterListView"_s)
				.add_function_proxy<
					&normalized_lambda<
						[] (
						VariableCharacterListView & it
					) -> VariableByteListView {
							return self_cast<VariableByteListView>(it);
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
					) -> VariableCharacterListView {
							return up_cast<VariableCharacterListView>(it.view());
						}
					>
				>("cast_String_to_CharacterListView"_s)
				.add_function<
					&normalized_lambda<
						[] (
						JavaScript::NativeValueHandler<VariableCharacterListView> & it
					) -> VariableStringView & {
							// NOTE: EXPLAIN: return StringView is cheap
							return down_cast<VariableStringView>(it.value());
						}
					>
				>("cast_CharacterListView_to_JS_String"_s);
			s_Miscellaneous.add_variable("g_context"_s, context.context().new_value(JavaScript::NativeValueHandler<Context>::new_reference(context)));
			s_Miscellaneous.add_variable("g_version"_s, context.context().new_value(JavaScript::NativeValueHandler<String>::new_instance_allocate(make_string(M_version))));
			s_Miscellaneous.add_variable("g_system"_s, context.context().new_value(JavaScript::NativeValueHandler<String>::new_instance_allocate(make_string(M_system))));
			s_Miscellaneous.add_variable("g_architecture"_s, context.context().new_value(JavaScript::NativeValueHandler<String>::new_instance_allocate(make_string(M_architecture))));
		}
		return;
	}

	#pragma endregion

}
