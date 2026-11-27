module;

#include "kernel/common.hpp"

export module twinning.kernel.executor.environment;
import twinning.kernel.utility;
import twinning.kernel.executor.context;
import twinning.kernel.tool.data.hashing.fnv.common;
import twinning.kernel.tool.data.hashing.fnv.hash;
import twinning.kernel.tool.data.hashing.crc.common;
import twinning.kernel.tool.data.hashing.crc.hash;
import twinning.kernel.tool.data.hashing.adler.common;
import twinning.kernel.tool.data.hashing.adler.hash;
import twinning.kernel.tool.data.hashing.md.common;
import twinning.kernel.tool.data.hashing.md.hash;
import twinning.kernel.tool.data.hashing.sha.common;
import twinning.kernel.tool.data.hashing.sha.hash;
import twinning.kernel.tool.data.encoding.base.common;
import twinning.kernel.tool.data.encoding.base.encode;
import twinning.kernel.tool.data.encoding.base.decode;
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
import twinning.kernel.tool.data.compression.lzma1.common;
import twinning.kernel.tool.data.compression.lzma1.compress;
import twinning.kernel.tool.data.compression.lzma1.uncompress;
import twinning.kernel.tool.data.compression.lzma2.common;
import twinning.kernel.tool.data.compression.lzma2.compress;
import twinning.kernel.tool.data.compression.lzma2.uncompress;
import twinning.kernel.tool.data.compression.lz4.common;
import twinning.kernel.tool.data.compression.lz4.compress;
import twinning.kernel.tool.data.compression.lz4.uncompress;
import twinning.kernel.tool.data.compression.lzo.common;
import twinning.kernel.tool.data.compression.lzo.compress;
import twinning.kernel.tool.data.compression.lzo.uncompress;
import twinning.kernel.tool.data.compression.brotli.common;
import twinning.kernel.tool.data.compression.brotli.compress;
import twinning.kernel.tool.data.compression.brotli.uncompress;
import twinning.kernel.tool.data.compression.snappy.common;
import twinning.kernel.tool.data.compression.snappy.compress;
import twinning.kernel.tool.data.compression.snappy.uncompress;
import twinning.kernel.tool.data.compression.zstd.common;
import twinning.kernel.tool.data.compression.zstd.compress;
import twinning.kernel.tool.data.compression.zstd.uncompress;
import twinning.kernel.tool.data.differentiation.vcdiff.common;
import twinning.kernel.tool.data.differentiation.vcdiff.encode;
import twinning.kernel.tool.data.differentiation.vcdiff.decode;
import twinning.kernel.tool.data.serialization.json.common;
import twinning.kernel.tool.data.serialization.json.encode;
import twinning.kernel.tool.data.serialization.json.decode;
import twinning.kernel.tool.data.serialization.xml.common;
import twinning.kernel.tool.data.serialization.xml.encode;
import twinning.kernel.tool.data.serialization.xml.decode;
import twinning.kernel.tool.texture.transformation.filling.common;
import twinning.kernel.tool.texture.transformation.filling.encode;
import twinning.kernel.tool.texture.transformation.flipping.common;
import twinning.kernel.tool.texture.transformation.flipping.encode;
import twinning.kernel.tool.texture.transformation.rotating.common;
import twinning.kernel.tool.texture.transformation.rotating.encode;
import twinning.kernel.tool.texture.transformation.scaling.common;
import twinning.kernel.tool.texture.transformation.scaling.encode;
import twinning.kernel.tool.texture.transformation.tiling.common;
import twinning.kernel.tool.texture.transformation.tiling.encode;
import twinning.kernel.tool.texture.transformation.tiling.decode;
import twinning.kernel.tool.texture.transformation.interleaving.common;
import twinning.kernel.tool.texture.transformation.interleaving.encode;
import twinning.kernel.tool.texture.transformation.interleaving.decode;
import twinning.kernel.tool.texture.transformation.premultiplying.common;
import twinning.kernel.tool.texture.transformation.premultiplying.encode;
import twinning.kernel.tool.texture.transformation.premultiplying.decode;
import twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.tool.texture.encoding.encode;
import twinning.kernel.tool.texture.encoding.decode;
import twinning.kernel.tool.texture.compression.dxtc.common;
import twinning.kernel.tool.texture.compression.dxtc.compress;
import twinning.kernel.tool.texture.compression.dxtc.uncompress;
import twinning.kernel.tool.texture.compression.pvrtc.common;
import twinning.kernel.tool.texture.compression.pvrtc.compress;
import twinning.kernel.tool.texture.compression.pvrtc.uncompress;
import twinning.kernel.tool.texture.compression.etc.common;
import twinning.kernel.tool.texture.compression.etc.compress;
import twinning.kernel.tool.texture.compression.etc.uncompress;
import twinning.kernel.tool.texture.compression.astc.common;
import twinning.kernel.tool.texture.compression.astc.compress;
import twinning.kernel.tool.texture.compression.astc.uncompress;
import twinning.kernel.tool.texture.conversion.png.common;
import twinning.kernel.tool.texture.conversion.png.encode;
import twinning.kernel.tool.texture.conversion.png.decode;
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
import twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.common;
import twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.encrypt;
import twinning.kernel.tool.miscellaneous.pvz2cn_crypt_data.decrypt;
import twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.common;
import twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.encode;
import twinning.kernel.tool.miscellaneous.pvz2cn_alpha_palette_texture.decode;

export namespace Twinning::Kernel::Executor::Environment {

	#pragma region define generic class

	M_enumeration_specific(
		M_wrap(GenericClassDefinitionFlag),
		M_wrap(ZIntegerU8),
		M_wrap(
			// generic operation
			default_constructor,
			copy_constructor,
			// value operation
			value_constructor,
			value_getter,
			value_setter,
			// mask
			none_mask,
			all_mask,
			generic_mask,
			value_mask,
			default_mask,
		),
		M_wrap(
			// @formatter:off
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
			// @formatter:on
		),
	);

	// ----------------

	template <typename TClass, auto t_flag = GenericClassDefinitionFlag::Constant::default_mask()> requires
		CategoryConstraint<IsPureInstance<TClass>>
		&& (IsSameOf<t_flag, GenericClassDefinitionFlag>)
	inline auto define_generic_class(
		Script::JavaScript::NativeSpaceBuilder & space,
		String const &                           name
	) -> Script::JavaScript::NativeClassBuilder<TClass> {
		auto builder = space.add_class<TClass>(name);
		builder.template set_constructor<
			&normalized_lambda<
				[](
			) -> Script::JavaScript::NativeValueHandler<TClass> {
					throw UnsupportedException{};
				}
			>
		>();
		constexpr auto check = [](GenericClassDefinitionFlag const & value) -> Boolean {
			return make_box<Boolean>((t_flag.underlying() & value.underlying()) != make_box<GenericClassDefinitionFlag::Underlying>(0));
		};
		if constexpr (check(GenericClassDefinitionFlag::Constant::default_constructor())) {
			// NOTE: EXPLAIN: static default(): T;
			builder.template add_constructor_allocate_proxy<>("default"_s);
		}
		if constexpr (check(GenericClassDefinitionFlag::Constant::copy_constructor())) {
			// NOTE: EXPLAIN: static copy(it: T): T;
			builder.template add_constructor_allocate_proxy<TClass const &>("copy"_s);
		}
		if constexpr (check(GenericClassDefinitionFlag::Constant::value_constructor())) {
			// NOTE: EXPLAIN: static value(it: typeof T.Value): T;
			builder.template add_constructor<
				&normalized_lambda<
					[](
					TClass & that
				) -> Script::JavaScript::NativeValueHandler<TClass> {
						return Script::JavaScript::NativeValueHandler<TClass>::new_instance_allocate(that);
					}
				>
			>("value"_s);
		}
		if constexpr (check(GenericClassDefinitionFlag::Constant::value_getter()) || check(GenericClassDefinitionFlag::Constant::value_setter())) {
			static_assert(check(GenericClassDefinitionFlag::Constant::value_getter()));
			// NOTE: EXPLAIN: get value(): typeof T.Value;
			constexpr auto & getter = normalized_lambda<
				[](
				Script::JavaScript::NativeValueHandler<TClass> & thix
			) -> TClass & {
					return thix.value();
				}
			>;
			if constexpr (!check(GenericClassDefinitionFlag::Constant::value_setter())) {
				builder.template add_getter<&getter>("value"_s);
			}
			else {
				// NOTE: EXPLAIN: set value(it: typeof T.Value);
				constexpr auto & setter = normalized_lambda<
					[](
					Script::JavaScript::NativeValueHandler<TClass> & thix,
					TClass &                                         value
				) -> Void {
						value = thix.value();
						return;
					}
				>;
				builder.template add_getter_setter<&getter, &setter>("value"_s);
			}
		}
		return builder;
	}

	template <typename TVersion, typename TVersionPackage, typename TClass> requires
		CategoryConstraint<IsPureInstance<TVersion> && IsPureInstance<TVersionPackage> && IsPureInstance<TClass>>
	inline auto define_variant_class_version_method(
		Script::JavaScript::NativeClassBuilder<TClass> & builder
	) -> Script::JavaScript::NativeClassBuilder<TClass> & {
		// NOTE: EXPLAIN: static json(json: Notation.Json.Value, version: Version): T;
		constexpr auto & json_constructor = Script::JavaScript::proxy_native_function_by_handler<
			&normalized_lambda<
				[](
				Notation::Json::Value const & json,
				TVersion const &              version
			) -> TClass {
					auto it = TClass{};
					Generalization::match<TVersionPackage>(
						version,
						[&]<auto t_index, auto t_version>(ValuePackage<t_index>, ValuePackage<t_version>) {
							json.to(it, make_box<Size>(t_index));
						}
					);
					return it;
				}
			>
		>;
		// NOTE: EXPLAIN: get_json(version: Version): Notation.Json.Value;
		constexpr auto & json_getter = Script::JavaScript::proxy_native_function_by_handler<
			&normalized_lambda<
				[](
				TClass &         thix,
				TVersion const & version
			) -> Notation::Json::Value {
					auto json = Notation::Json::Value{};
					Generalization::match<TVersionPackage>(
						version,
						[&]<auto t_index, auto t_version>(ValuePackage<t_index>, ValuePackage<t_version>) {
							json.from(thix, make_box<Size>(t_index));
						}
					);
					return json;
				}
			>
		>;
		// NOTE: EXPLAIN: set_json(version: Version, value: Notation.Json.Value);
		constexpr auto & json_setter = Script::JavaScript::proxy_native_function_by_handler<
			&normalized_lambda<
				[](
				TClass &                      thix,
				TVersion const &              version,
				Notation::Json::Value const & json
			) -> Void {
					Generalization::match<TVersionPackage>(
						version,
						[&]<auto t_index, auto t_version>(ValuePackage<t_index>, ValuePackage<t_version>) {
							json.to(thix, make_box<Size>(t_index));
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
	// - *BinaryView       -> VariableBinaryView       -> BinaryView
	// - *BinaryStreamView -> AccessBinaryStreamView   -> BinaryStreamView
	// - *StringView       -> VariableStringView       -> StringView
	// - *StringStreamView -> AccessStringStreamView   -> StringStreamView
	// - Image::*ImageView -> Image::VariableImageView -> Image::ImageView

	namespace Detail {

		template <typename TType> requires
			NoneConstraint
		using AsPromotion = decltype([]<typename = None> {
			if constexpr (IsSame<TType, VariableBinaryView, ConstantBinaryView>) {
				return declare<VariableBinaryView>();
			}
			else if constexpr (IsSame<TType, InputBinaryStreamView, OutputBinaryStreamView, AccessBinaryStreamView>) {
				return declare<AccessBinaryStreamView>();
			}
			else if constexpr (IsSame<TType, VariableStringView, ConstantStringView>) {
				return declare<VariableStringView>();
			}
			else if constexpr (IsSame<TType, InputStringStreamView, OutputStringStreamView, AccessStringStreamView>) {
				return declare<AccessStringStreamView>();
			}
			else if constexpr (IsSame<TType, Image::VariableImageView, Image::ConstantImageView>) {
				return declare<Image::VariableImageView>();
			}
			else {
				return declare<TType>();
			}
		}());

		template <auto t_function, auto ... t_index> requires
			NoneConstraint
		inline constexpr auto make_proxy_function_with_promotion(
			ValuePackage<t_index ...>
		) -> auto {
			if constexpr ((IsSame<AsPromotion<AsPure<typename CallableTraitOf<t_function>::Argument::template Element<t_index>>>, AsPure<typename CallableTraitOf<t_function>::Argument::template Element<t_index>>> && ...)) {
				return t_function;
			}
			else {
				return &proxy_global_function<t_function, AsPromotion<AsPure<typename CallableTraitOf<t_function>::Argument::template Element<t_index>>> & ...>;
			}
		}

	}

	// ----------------

	template <auto t_function> requires
		CategoryConstraint<>
		&& (IsGlobalFunction<decltype(t_function)>)
	inline constexpr auto & proxy_global_function_with_promotion = *Detail::make_proxy_function_with_promotion<t_function>(AsValuePackageOfIndex<CallableTraitOf<t_function>::Argument::size>{});

	template <typename TClass, auto t_function> requires
		CategoryConstraint<>
		&& (IsMemberFunction<decltype(t_function)>)
		&& (IsDerivedFrom<TClass, typename CallableTraitOf<t_function>::Class>)
	inline constexpr auto & proxy_member_function_with_promotion = proxy_global_function_with_promotion<normalized_member_function<t_function, TClass>>;

	#pragma endregion

	#pragma region proxy function by versioned

	M_enumeration(
		M_wrap(VersionedOperation),
		M_wrap(
			process,
			estimate,
		),
	);

	namespace Detail {

		template <typename TVersion, typename TVersionPackage, template <auto> typename TMOperationClass, auto t_operation_type, typename TVersionedArgument, typename ... TArgument> requires
			NoneConstraint
		inline auto make_proxy_function_by_versioned(
			TArgument & ...  argument,
			TVersion const & version
		) -> Void {
			Generalization::match<TVersionPackage>(
				version,
				[&]<auto t_index, auto t_version>(ValuePackage<t_index>, ValuePackage<t_version>) {
					auto extract_argument = [&]<auto t_argument_index>(ValuePackage<t_argument_index>) -> auto & {
						using CurrentArgument = AsSelect<t_argument_index, TArgument ...>;
						auto & current_argument = argument...[t_argument_index];
						if constexpr (TVersionedArgument::template has<AsPure<CurrentArgument>>()) {
							if constexpr (IsConstant<CurrentArgument>) {
								return current_argument.template get_of_index<make_box<Size>(t_index)>();
							}
							if constexpr (IsVariable<CurrentArgument>) {
								return current_argument.template set_of_index<make_box<Size>(t_index)>();
							}
						}
						else {
							return current_argument;
						}
					};
					[&] <auto ... t_argument_index>(ValuePackage<t_argument_index ...>) -> Void {
						if constexpr (t_operation_type == VersionedOperation::Constant::process()) {
							TMOperationClass<t_version>::process(extract_argument(ValuePackage<t_argument_index>{}) ...);
						}
						if constexpr (t_operation_type == VersionedOperation::Constant::estimate()) {
							TMOperationClass<t_version>::estimate(extract_argument(ValuePackage<t_argument_index>{}) ...);
						}
					}(AsValuePackageOfIndex<sizeof...(TArgument)>{});
				}
			);
			return;
		}

	}

	// ----------------

	template <typename TVersion, typename TVersionPackage, template <auto> typename TMOperationClass, auto t_operation_type, typename TVersionedArgument, typename ... TArgument> requires
		CategoryConstraint<IsPureInstance<TVersionPackage> && IsInstance<TArgument ...>>
		&& (IsValuePackage<TVersionPackage>)
		&& (IsSameOf<t_operation_type, VersionedOperation>)
		&& (IsTypePackage<TVersionedArgument>)
	inline constexpr auto & proxy_function_by_versioned = Detail::make_proxy_function_by_versioned<TVersion, TVersionPackage, TMOperationClass, t_operation_type, TVersionedArgument, TArgument ...>;

	#pragma endregion

	#pragma region inject

	inline auto inject(
		Context & context
	) -> Void {
		// @formatter:off
		// ReSharper disable CppInconsistentNaming CppTooWideScope
		auto s_Twinning = Script::JavaScript::NativeSpaceBuilder{make_optional_null<String>(), "Twinning"_s, as_left(context.context().global_object())};
		auto s_Kernel = s_Twinning.add_space("Kernel"_s);
		// Boolean
		define_generic_class<Boolean>(s_Kernel, "Boolean"_s);
		// Integer
		define_generic_class<Integer>(s_Kernel, "Integer"_s);
		define_generic_class<IntegerU32>(s_Kernel, "IntegerU32"_s);
		// Size
		define_generic_class<Size>(s_Kernel, "Size"_s);
		define_generic_class<Optional<Size>>(s_Kernel, "SizeOptional"_s);
		// Byte
		define_generic_class<Byte>(s_Kernel, "Byte"_s);
		// String
		define_generic_class<String>(s_Kernel, "String"_s);
		define_generic_class<List<String>>(s_Kernel, "StringList"_s);
		define_generic_class<Map<String, String>>(s_Kernel, "StringMap"_s);
		// Path
		define_generic_class<Path>(s_Kernel, "Path"_s);
		define_generic_class<Optional<Path>>(s_Kernel, "PathOptional"_s);
		define_generic_class<List<Path>>(s_Kernel, "PathList"_s);
		// Binary
		define_generic_class<Binary>(s_Kernel, "Binary"_s)
			.add_constructor_allocate_proxy<Size const &>("allocate"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<Binary, &Binary::allocate_full>>("allocate"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<Binary, &Binary::reset>>("reset"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<Binary, &Binary::size>>("size"_s).add_member_function_proxy<
					&normalized_lambda<
						[](
						Binary & target
					) -> VariableBinaryView & {
							return as_variable(target.as_view());
						}
					>
				>("view"_s)
			.add_member_function<
				&normalized_lambda<
					[](
					Script::JavaScript::NativeValueHandler<Binary> & that
				) -> Binary && {
						return as_moveable(that.value());
					}
				>
			>("release"_s);
		// BinaryView
		define_generic_class<VariableBinaryView>(s_Kernel, "BinaryView"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableBinaryView, &VariableBinaryView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableBinaryView, AsConstantMemberFunction<VariableBinaryView, VariableBinaryView, Size const &, Size const &>{&VariableBinaryView::sub}>>("sub"_s);
		// BinaryStreamView
		define_generic_class<AccessBinaryStreamView, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Kernel, "BinaryStreamView"_s)
			.add_constructor_allocate_proxy<VariableBinaryView const &>("watch"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessBinaryStreamView, &AccessBinaryStreamView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessBinaryStreamView, &AccessBinaryStreamView::position>>("position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessBinaryStreamView, &AccessBinaryStreamView::set_position>>("set_position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessBinaryStreamView, &AccessBinaryStreamView::view>>("view"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessBinaryStreamView, &AccessBinaryStreamView::split_stream>>("split_stream"_s);
		// StringView
		define_generic_class<VariableStringView, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Kernel, "StringView"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableStringView, &VariableStringView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<VariableStringView, AsConstantMemberFunction<VariableStringView, VariableStringView, Size const &, Size const &>{&VariableStringView::sub}>>("sub"_s);
		// StringStreamView
		define_generic_class<AccessStringStreamView, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Kernel, "StringStreamView"_s)
			.add_constructor_allocate_proxy<VariableStringView const &>("watch"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessStringStreamView, &AccessStringStreamView::size>>("size"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessStringStreamView, &AccessStringStreamView::position>>("position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessStringStreamView, &AccessStringStreamView::set_position>>("set_position"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessStringStreamView, &AccessStringStreamView::view>>("view"_s)
			.add_member_function_proxy<&proxy_member_function_with_promotion<AccessStringStreamView, &AccessStringStreamView::split_stream>>("split_stream"_s);
		// Notation
		{
			auto s_Notation = s_Kernel.add_space("Notation"_s);
			// Json
			{
				auto s_Json = s_Notation.add_space("Json"_s);
				define_generic_class<Notation::Json::Value>(s_Json, "Value"_s);
			}
			// Xml
			{
				auto s_Xml = s_Notation.add_space("Xml"_s);
				define_generic_class<Notation::Xml::Node>(s_Xml, "Node"_s);
			}
		}
		// Image
		{
			auto s_Image = s_Kernel.add_space("Image"_s);
			define_generic_class<Image::ImageSize>(s_Image, "ImageSize"_s);
			define_generic_class<Image::ImagePosition>(s_Image, "ImagePosition"_s);
			define_generic_class<Image::Color>(s_Image, "Color"_s);
			define_generic_class<Optional<Image::Color>>(s_Image, "ColorOptional"_s);
			define_generic_class<List<Image::Color>>(s_Image, "ColorList"_s);
			define_generic_class<Image::Pixel>(s_Image, "Pixel"_s);
			define_generic_class<Image::VariableImageView, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Image, "ImageView"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::size>>("size"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::fill>>("fill"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::draw>>("draw"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::VariableImageView, &Image::VariableImageView::sub>>("sub"_s);
			define_generic_class<Image::Image, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Image, "Image"_s)
				.add_constructor_allocate_proxy<Image::ImageSize const &>("allocate"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, &Image::Image::allocate>>("allocate"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, &Image::Image::reset>>("reset"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Image::Image, &Image::Image::size>>("size"_s)
				.add_member_function_proxy<
					&normalized_lambda<
						[](
						Image::Image & target
					) -> Image::VariableImageView & {
							return as_variable(target.as_view());
						}
					>
				>("view"_s);
		}
		// Storage
		{
			auto s_Storage = s_Kernel.add_space("Storage"_s);
			s_Storage
				// basic
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist>>("exist"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::copy>>("copy"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::rename>>("rename"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::remove>>("remove"_s)
				// link
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist_link>>("exist_link"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_link>>("create_link"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::resolve_link>>("resolve_link"_s)
				// file
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist_file>>("exist_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_file>>("create_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::size_file>>("size_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::resize_file>>("resize_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::read_file>>("read_file"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::write_file>>("write_file"_s)
				// directory
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::exist_directory>>("exist_directory"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::create_directory>>("create_directory"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Storage::list_directory>>("list_directory"_s);
		}
		// Process
		{
			auto s_Process = s_Kernel.add_space("Process"_s);
			s_Process
				// context
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::get_workspace>>("get_workspace"_s)
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::get_environment>>("get_environment"_s)
				// child
				.add_function_proxy<&proxy_global_function_with_promotion<&Process::run_child>>("run_child"_s);
		}
		// Tool
		{
			auto s_Tool = s_Kernel.add_space("Tool"_s);
			// Data
			{
				auto s_Data = s_Tool.add_space("Data"_s);
				{
					auto s_Hashing = s_Data.add_space("Hashing"_s);
					{
						auto s_Fnv = s_Hashing.add_space("Fnv"_s);
						define_generic_class<Tool::Data::Hashing::Fnv::Mode>(s_Fnv, "Mode"_s);
						s_Fnv.add_space("Hash"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<Tool::Data::Hashing::Fnv::Hash::process>>("process"_s);
					}
					{
						auto s_Crc = s_Hashing.add_space("Crc"_s);
						define_generic_class<Tool::Data::Hashing::Crc::Mode>(s_Crc, "Mode"_s);
						s_Crc.add_space("Hash"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<Tool::Data::Hashing::Crc::Hash::process>>("process"_s);
					}
					{
						auto s_Adler = s_Hashing.add_space("Adler"_s);
						define_generic_class<Tool::Data::Hashing::Adler::Mode>(s_Adler, "Mode"_s);
						s_Adler.add_space("Hash"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<Tool::Data::Hashing::Adler::Hash::process>>("process"_s);
					}
					{
						auto s_Md = s_Hashing.add_space("Md"_s);
						define_generic_class<Tool::Data::Hashing::Md::Mode>(s_Md, "Mode"_s);
						s_Md.add_space("Hash"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Hashing::Md::Hash::process>>("process"_s);
					}
					{
						auto s_Sha = s_Hashing.add_space("Sha"_s);
						define_generic_class<Tool::Data::Hashing::Sha::Mode>(s_Sha, "Mode"_s);
						s_Sha.add_space("Hash"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Hashing::Sha::Hash::process>>("process"_s);
					}
				}
				{
					auto s_Encoding = s_Data.add_space("Encoding"_s);
					{
						auto s_Base = s_Encoding.add_space("Base"_s);
						define_generic_class<Tool::Data::Encoding::Base::Mode>(s_Base, "Mode"_s);
						s_Base.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base::Encode::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base::Encode::estimate>>("estimate"_s);
						s_Base.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base::Decode::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encoding::Base::Decode::estimate>>("estimate"_s);
					}
				}
				{
					auto s_Encryption = s_Data.add_space("Encryption"_s);
					{
						auto s_Exor = s_Encryption.add_space("Exor"_s);
						s_Exor.add_space("Encrypt"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Encryption::Exor::Encrypt::process>>("process"_s);
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
						define_generic_class<Tool::Data::Compression::Deflate::Container>(s_Deflate, "Container"_s);
						define_generic_class<Tool::Data::Compression::Deflate::StrategyMode>(s_Deflate, "StrategyMode"_s);
						s_Deflate.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Deflate::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Deflate::Compress::estimate>>("estimate"_s);
						s_Deflate.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Deflate::Uncompress::process>>("process"_s);
					}
					{
						auto s_Bzip2 = s_Compression.add_space("Bzip2"_s);
						define_generic_class<Tool::Data::Compression::Bzip2::Container>(s_Bzip2, "Container"_s);
						s_Bzip2.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Bzip2::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Bzip2::Compress::estimate>>("estimate"_s);
						s_Bzip2.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Bzip2::Uncompress::process>>("process"_s);
					}
					{
						auto s_Lzma1 = s_Compression.add_space("Lzma1"_s);
						define_generic_class<Tool::Data::Compression::Lzma1::Container>(s_Lzma1, "Container"_s);
						s_Lzma1.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma1::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma1::Compress::estimate>>("estimate"_s);
						s_Lzma1.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma1::Uncompress::process>>("process"_s);
					}
					{
						auto s_Lzma2 = s_Compression.add_space("Lzma2"_s);
						define_generic_class<Tool::Data::Compression::Lzma2::Container>(s_Lzma2, "Container"_s);
						s_Lzma2.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma2::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma2::Compress::estimate>>("estimate"_s);
						s_Lzma2.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzma2::Uncompress::process>>("process"_s);
					}
					{
						auto s_Lz4 = s_Compression.add_space("Lz4"_s);
						define_generic_class<Tool::Data::Compression::Lz4::Container>(s_Lz4, "Container"_s);
						s_Lz4.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lz4::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lz4::Compress::estimate>>("estimate"_s);
						s_Lz4.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lz4::Uncompress::process>>("process"_s);
					}
					{
						auto s_Lzo = s_Compression.add_space("Lzo"_s);
						define_generic_class<Tool::Data::Compression::Lzo::Container>(s_Lzo, "Container"_s);
						s_Lzo.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzo::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzo::Compress::estimate>>("estimate"_s);
						s_Lzo.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Lzo::Uncompress::process>>("process"_s);
					}
					{
						auto s_Brotli = s_Compression.add_space("Brotli"_s);
						define_generic_class<Tool::Data::Compression::Brotli::Container>(s_Brotli, "Container"_s);
						define_generic_class<Tool::Data::Compression::Brotli::Mode>(s_Brotli, "Mode"_s);
						s_Brotli.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Brotli::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Brotli::Compress::estimate>>("estimate"_s);
						s_Brotli.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Brotli::Uncompress::process>>("process"_s);
					}
					{
						auto s_Snappy = s_Compression.add_space("Snappy"_s);
						define_generic_class<Tool::Data::Compression::Snappy::Container>(s_Snappy, "Container"_s);
						s_Snappy.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Snappy::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Snappy::Compress::estimate>>("estimate"_s);
						s_Snappy.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Snappy::Uncompress::process>>("process"_s);
					}
					{
						auto s_Zstd = s_Compression.add_space("Zstd"_s);
						define_generic_class<Tool::Data::Compression::Zstd::Container>(s_Zstd, "Container"_s);
						s_Zstd.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Zstd::Compress::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Zstd::Compress::estimate>>("estimate"_s);
						s_Zstd.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Compression::Zstd::Uncompress::process>>("process"_s);
					}
				}
				{
					auto s_Differentiation = s_Data.add_space("Differentiation"_s);
					{
						auto s_Vcdiff = s_Differentiation.add_space("Vcdiff"_s);
						s_Vcdiff.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Differentiation::Vcdiff::Encode::process>>("process"_s);
						s_Vcdiff.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Differentiation::Vcdiff::Decode::process>>("process"_s);
					}
				}
				{
					auto s_Serialization = s_Data.add_space("Serialization"_s);
					{
						auto s_Json = s_Serialization.add_space("Json"_s);
						s_Json.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::Json::Encode::process>>("process"_s);
						s_Json.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::Json::Decode::process>>("process"_s);
					}
					{
						auto s_Xml = s_Serialization.add_space("Xml"_s);
						s_Xml.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::Xml::Encode::process>>("process"_s);
						s_Xml.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Data::Serialization::Xml::Decode::process>>("process"_s);
					}
				}
			}
			// Texture
			{
				auto s_Texture = s_Tool.add_space("Texture"_s);
				{
					auto s_Transformation = s_Texture.add_space("Transformation"_s);
					{
						auto s_Filling = s_Transformation.add_space("Filling"_s);
						s_Filling.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Filling::Encode::process>>("process"_s);
					}
					{
						auto s_Flipping = s_Transformation.add_space("Flipping"_s);
						s_Flipping.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Flipping::Encode::process>>("process"_s);
					}
					{
						auto s_Rotating = s_Transformation.add_space("Rotating"_s);
						s_Rotating.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Rotating::Encode::process>>("process"_s);
					}
					{
						auto s_Scaling = s_Transformation.add_space("Scaling"_s);
						s_Scaling.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Scaling::Encode::process>>("process"_s);
					}
					{
						auto s_Tiling = s_Transformation.add_space("Tiling"_s);
						s_Tiling.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Tiling::Encode::process>>("process"_s);
						s_Tiling.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Tiling::Decode::process>>("process"_s);
					}
					{
						auto s_Interleaving = s_Transformation.add_space("Interleaving"_s);
						s_Interleaving.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Interleaving::Encode::process>>("process"_s);
						s_Interleaving.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Interleaving::Decode::process>>("process"_s);
					}
					{
						auto s_Premultiplying = s_Transformation.add_space("Premultiplying"_s);
						s_Premultiplying.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Premultiplying::Encode::process>>("process"_s);
						s_Premultiplying.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Transformation::Premultiplying::Decode::process>>("process"_s);
					}
				}
				{
					auto s_Encoding = s_Texture.add_space("Encoding"_s);
					define_generic_class<Tool::Texture::Encoding::Channel>(s_Encoding, "Channel"_s);
					define_generic_class<Tool::Texture::Encoding::Format>(s_Encoding, "Format"_s);
					s_Encoding.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Encoding::Encode::process>>("process"_s);
					s_Encoding.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Encoding::Decode::process>>("process"_s);
				}
				{
					auto s_Compression = s_Texture.add_space("Compression"_s);
					{
						auto s_Dxtc = s_Compression.add_space("Dxtc"_s);
						define_generic_class<Tool::Texture::Compression::Dxtc::Generation>(s_Dxtc, "Generation"_s);
						s_Dxtc.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Dxtc::Compress::process>>("process"_s);
						s_Dxtc.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Dxtc::Uncompress::process>>("process"_s);
					}
					{
						auto s_Pvrtc = s_Compression.add_space("Pvrtc"_s);
						define_generic_class<Tool::Texture::Compression::Pvrtc::Generation>(s_Pvrtc, "Generation"_s);
						s_Pvrtc.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Pvrtc::Compress::process>>("process"_s);
						s_Pvrtc.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Pvrtc::Uncompress::process>>("process"_s);
					}
					{
						auto s_Etc = s_Compression.add_space("Etc"_s);
						define_generic_class<Tool::Texture::Compression::Etc::Generation>(s_Etc, "Generation"_s);
						s_Etc.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Etc::Compress::process>>("process"_s);
						s_Etc.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Etc::Uncompress::process>>("process"_s);
					}
					{
						auto s_Astc = s_Compression.add_space("Astc"_s);
						define_generic_class<Tool::Texture::Compression::Astc::Generation>(s_Astc, "Generation"_s);
						s_Astc.add_space("Compress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Astc::Compress::process>>("process"_s);
						s_Astc.add_space("Uncompress"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Compression::Astc::Uncompress::process>>("process"_s);
					}
				}
				{
					auto s_Conversion = s_Texture.add_space("Conversion"_s);
					{
						auto s_Png = s_Conversion.add_space("Png"_s);
						s_Png.add_space("Encode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Conversion::Png::Encode::process>>("process"_s);
						s_Png.add_space("Decode"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Conversion::Png::Decode::process>>("process"_s)
							.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Texture::Conversion::Png::Decode::estimate>>("estimate"_s);
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
						Definition<VersionPackage::element<1_ixz>>::SoundBank,
						Definition<VersionPackage::element<2_ixz>>::SoundBank,
						Definition<VersionPackage::element<3_ixz>>::SoundBank,
						Definition<VersionPackage::element<4_ixz>>::SoundBank,
						Definition<VersionPackage::element<5_ixz>>::SoundBank,
						Definition<VersionPackage::element<6_ixz>>::SoundBank,
						Definition<VersionPackage::element<7_ixz>>::SoundBank,
						Definition<VersionPackage::element<8_ixz>>::SoundBank,
						Definition<VersionPackage::element<9_ixz>>::SoundBank,
						Definition<VersionPackage::element<10_ixz>>::SoundBank,
						Definition<VersionPackage::element<11_ixz>>::SoundBank,
						Definition<VersionPackage::element<12_ixz>>::SoundBank,
						Definition<VersionPackage::element<13_ixz>>::SoundBank,
						Definition<VersionPackage::element<14_ixz>>::SoundBank
					>;
					auto s_SoundBank = s_Wwise.add_space("SoundBank"_s);
					define_generic_class<Version>(s_SoundBank, "Version"_s);
					{
						auto s_Definition = s_SoundBank.add_space("Definition"_s);
						auto c_SoundBank = define_generic_class<SoundBankDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "SoundBank"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_SoundBank);
					}
					s_SoundBank.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Wwise::SoundBank::Encode, VersionedOperation::Constant::process(), TypePackage<SoundBankDefinition>, OutputBinaryStreamView, SoundBankDefinition const, Path const>>>("process"_s);
					s_SoundBank.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Wwise::SoundBank::Decode, VersionedOperation::Constant::process(), TypePackage<SoundBankDefinition>, InputBinaryStreamView, SoundBankDefinition, Optional<Path> const>>>("process"_s);
				}
			}
			// Marmalade
			{
				auto s_Marmalade = s_Tool.add_space("Marmalade"_s);
				{
					using Tool::Marmalade::Dzip::Version;
					using Tool::Marmalade::Dzip::VersionPackage;
					using Tool::Marmalade::Dzip::Definition;
					using PackageDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Package
					>;
					auto s_Dzip = s_Marmalade.add_space("Dzip"_s);
					define_generic_class<Version>(s_Dzip, "Version"_s);
					{
						auto s_Definition = s_Dzip.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_Dzip.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Marmalade::Dzip::Pack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition>, OutputBinaryStreamView, PackageDefinition const, Path const>>>("process"_s);
					s_Dzip.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Marmalade::Dzip::Unpack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition>, InputBinaryStreamView, PackageDefinition, Optional<Path> const>>>("process"_s);
				}
			}
			// Popcap
			{
				auto s_Popcap = s_Tool.add_space("Popcap"_s);
				{
					using Tool::Popcap::Zlib::Version;
					using Tool::Popcap::Zlib::VersionPackage;
					auto s_Zlib = s_Popcap.add_space("Zlib"_s);
					define_generic_class<Version>(s_Zlib, "Version"_s);
					s_Zlib.add_space("Compress"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Zlib::Compress, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, OutputBinaryStreamView, Integer const, Integer const, Integer const, Tool::Data::Compression::Deflate::StrategyMode const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Zlib::Compress, VersionedOperation::Constant::estimate(), TypePackage<>, Size const, Size, Integer const, Integer const>>>("estimate"_s);
					s_Zlib.add_space("Uncompress"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Zlib::Uncompress, VersionedOperation::Constant::process(), TypePackage<>, OutputBinaryStreamView, InputBinaryStreamView, Integer const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Zlib::Uncompress, VersionedOperation::Constant::estimate(), TypePackage<>, Size, InputBinaryStreamView>>>("estimate"_s);
				}
				{
					using Tool::Popcap::CryptData::Version;
					using Tool::Popcap::CryptData::VersionPackage;
					auto s_CryptData = s_Popcap.add_space("CryptData"_s);
					define_generic_class<Version>(s_CryptData, "Version"_s);
					s_CryptData.add_space("Encrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::CryptData::Encrypt, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, OutputBinaryStreamView, Size const, String const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::CryptData::Encrypt, VersionedOperation::Constant::estimate(), TypePackage<>, Size const, Size, Size const>>>("estimate"_s);
					s_CryptData.add_space("Decrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::CryptData::Decrypt, VersionedOperation::Constant::process(), TypePackage<>, OutputBinaryStreamView, InputBinaryStreamView, Size const, String const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::CryptData::Decrypt, VersionedOperation::Constant::estimate(), TypePackage<>, Size, InputBinaryStreamView, Size const>>>("estimate"_s);
				}
				{
					using Tool::Popcap::ReflectionObjectNotation::Version;
					using Tool::Popcap::ReflectionObjectNotation::VersionPackage;
					auto s_ReflectionObjectNotation = s_Popcap.add_space("ReflectionObjectNotation"_s);
					define_generic_class<Version>(s_ReflectionObjectNotation, "Version"_s);
					s_ReflectionObjectNotation.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ReflectionObjectNotation::Encode, VersionedOperation::Constant::process(), TypePackage<>, OutputBinaryStreamView, Notation::Json::Value const, Boolean const, Boolean const>>>("process"_s);
					s_ReflectionObjectNotation.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ReflectionObjectNotation::Decode, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, Notation::Json::Value>>>("process"_s);
				}
				{
					using Tool::Popcap::UTexture::Version;
					using Tool::Popcap::UTexture::VersionPackage;
					auto s_UTexture = s_Popcap.add_space("UTexture"_s);
					define_generic_class<Version>(s_UTexture, "Version"_s);
					s_UTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::UTexture::Encode, VersionedOperation::Constant::process(), TypePackage<>, OutputBinaryStreamView, Image::ConstantImageView const, String const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::UTexture::Encode, VersionedOperation::Constant::estimate(), TypePackage<>, Size, Image::ImageSize const, String const>>>("estimate"_s);
					s_UTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::UTexture::Decode, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, Image::VariableImageView const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::UTexture::Decode, VersionedOperation::Constant::estimate(), TypePackage<>, InputBinaryStreamView, Image::ImageSize>>>("estimate"_s);
				}
				{
					using Tool::Popcap::SexyTexture::Version;
					using Tool::Popcap::SexyTexture::VersionPackage;
					auto s_SexyTexture = s_Popcap.add_space("SexyTexture"_s);
					define_generic_class<Version>(s_SexyTexture, "Version"_s);
					s_SexyTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::SexyTexture::Encode, VersionedOperation::Constant::process(), TypePackage<>, OutputBinaryStreamView, Image::ConstantImageView const, String const, Boolean const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::SexyTexture::Encode, VersionedOperation::Constant::estimate(), TypePackage<>, Size, Image::ImageSize const, String const, Boolean const>>>("estimate"_s);
					s_SexyTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::SexyTexture::Decode, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, Image::VariableImageView const>>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::SexyTexture::Decode, VersionedOperation::Constant::estimate(), TypePackage<>, InputBinaryStreamView, Image::ImageSize>>>("estimate"_s);
				}
				{
					using Tool::Popcap::Animation::Version;
					using Tool::Popcap::Animation::VersionPackage;
					using Tool::Popcap::Animation::Definition;
					using AnimationDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Animation,
						Definition<VersionPackage::element<2_ixz>>::Animation,
						Definition<VersionPackage::element<3_ixz>>::Animation,
						Definition<VersionPackage::element<4_ixz>>::Animation,
						Definition<VersionPackage::element<5_ixz>>::Animation,
						Definition<VersionPackage::element<6_ixz>>::Animation
					>;
					auto s_Animation = s_Popcap.add_space("Animation"_s);
					define_generic_class<Version>(s_Animation, "Version"_s);
					{
						auto s_Definition = s_Animation.add_space("Definition"_s);
						auto c_Animation = define_generic_class<AnimationDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					s_Animation.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Animation::Encode, VersionedOperation::Constant::process(), TypePackage<AnimationDefinition>, OutputBinaryStreamView, AnimationDefinition const>>>("process"_s);
					s_Animation.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Animation::Decode, VersionedOperation::Constant::process(), TypePackage<AnimationDefinition>, InputBinaryStreamView, AnimationDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::ReAnimation::Version;
					using Tool::Popcap::ReAnimation::VersionPackage;
					using Tool::Popcap::ReAnimation::Definition;
					using AnimationDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Animation,
						Definition<VersionPackage::element<2_ixz>>::Animation,
						Definition<VersionPackage::element<3_ixz>>::Animation,
						Definition<VersionPackage::element<4_ixz>>::Animation
					>;
					auto s_ReAnimation = s_Popcap.add_space("ReAnimation"_s);
					define_generic_class<Version>(s_ReAnimation, "Version"_s);
					{
						auto s_Definition = s_ReAnimation.add_space("Definition"_s);
						auto c_Animation = define_generic_class<AnimationDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Animation"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Animation);
					}
					s_ReAnimation.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ReAnimation::Encode, VersionedOperation::Constant::process(), TypePackage<AnimationDefinition>, OutputBinaryStreamView, AnimationDefinition const>>>("process"_s);
					s_ReAnimation.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ReAnimation::Decode, VersionedOperation::Constant::process(), TypePackage<AnimationDefinition>, InputBinaryStreamView, AnimationDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::Particle::Version;
					using Tool::Popcap::Particle::VersionPackage;
					using Tool::Popcap::Particle::Definition;
					using ParticleDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Particle,
						Definition<VersionPackage::element<2_ixz>>::Particle,
						Definition<VersionPackage::element<3_ixz>>::Particle,
						Definition<VersionPackage::element<4_ixz>>::Particle
					>;
					auto s_Particle = s_Popcap.add_space("Particle"_s);
					define_generic_class<Version>(s_Particle, "Version"_s);
					{
						auto s_Definition = s_Particle.add_space("Definition"_s);
						auto c_Particle = define_generic_class<ParticleDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Particle"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Particle);
					}
					s_Particle.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Particle::Encode, VersionedOperation::Constant::process(), TypePackage<ParticleDefinition>, OutputBinaryStreamView, ParticleDefinition const>>>("process"_s);
					s_Particle.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Particle::Decode, VersionedOperation::Constant::process(), TypePackage<ParticleDefinition>, InputBinaryStreamView, ParticleDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::Trail::Version;
					using Tool::Popcap::Trail::VersionPackage;
					using Tool::Popcap::Trail::Definition;
					using TrailDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Trail,
						Definition<VersionPackage::element<2_ixz>>::Trail,
						Definition<VersionPackage::element<3_ixz>>::Trail,
						Definition<VersionPackage::element<4_ixz>>::Trail
					>;
					auto s_Trail = s_Popcap.add_space("Trail"_s);
					define_generic_class<Version>(s_Trail, "Version"_s);
					{
						auto s_Definition = s_Trail.add_space("Definition"_s);
						auto c_Trail = define_generic_class<TrailDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Trail"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Trail);
					}
					s_Trail.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Trail::Encode, VersionedOperation::Constant::process(), TypePackage<TrailDefinition>, OutputBinaryStreamView, TrailDefinition const>>>("process"_s);
					s_Trail.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Trail::Decode, VersionedOperation::Constant::process(), TypePackage<TrailDefinition>, InputBinaryStreamView, TrailDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::RenderEffect::Version;
					using Tool::Popcap::RenderEffect::VersionPackage;
					using Tool::Popcap::RenderEffect::Definition;
					using EffectDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Effect,
						Definition<VersionPackage::element<2_ixz>>::Effect,
						Definition<VersionPackage::element<3_ixz>>::Effect
					>;
					auto s_RenderEffect = s_Popcap.add_space("RenderEffect"_s);
					define_generic_class<Version>(s_RenderEffect, "Version"_s);
					{
						auto s_Definition = s_RenderEffect.add_space("Definition"_s);
						auto c_Effect = define_generic_class<EffectDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Effect"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Effect);
					}
					s_RenderEffect.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::RenderEffect::Encode, VersionedOperation::Constant::process(), TypePackage<EffectDefinition>, OutputBinaryStreamView, EffectDefinition const>>>("process"_s);
					s_RenderEffect.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::RenderEffect::Decode, VersionedOperation::Constant::process(), TypePackage<EffectDefinition>, InputBinaryStreamView, EffectDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::ParticleEffect::Version;
					using Tool::Popcap::ParticleEffect::VersionPackage;
					using Tool::Popcap::ParticleEffect::Definition;
					using EffectDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Effect
					>;
					auto s_ParticleEffect = s_Popcap.add_space("ParticleEffect"_s);
					define_generic_class<Version>(s_ParticleEffect, "Version"_s);
					{
						auto s_Definition = s_ParticleEffect.add_space("Definition"_s);
						auto c_Effect = define_generic_class<EffectDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Effect"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Effect);
					}
					s_ParticleEffect.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ParticleEffect::Encode, VersionedOperation::Constant::process(), TypePackage<EffectDefinition>, OutputBinaryStreamView, EffectDefinition const>>>("process"_s);
					s_ParticleEffect.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ParticleEffect::Decode, VersionedOperation::Constant::process(), TypePackage<EffectDefinition>, InputBinaryStreamView, EffectDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::CharacterFontWidget2::Version;
					using Tool::Popcap::CharacterFontWidget2::VersionPackage;
					using Tool::Popcap::CharacterFontWidget2::Definition;
					using FontWidgetDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::FontWidget
					>;
					auto s_CharacterFontWidget2 = s_Popcap.add_space("CharacterFontWidget2"_s);
					define_generic_class<Version>(s_CharacterFontWidget2, "Version"_s);
					{
						auto s_Definition = s_CharacterFontWidget2.add_space("Definition"_s);
						auto c_FontWidget = define_generic_class<FontWidgetDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "FontWidget"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_FontWidget);
					}
					s_CharacterFontWidget2.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::CharacterFontWidget2::Encode, VersionedOperation::Constant::process(), TypePackage<FontWidgetDefinition>, OutputBinaryStreamView, FontWidgetDefinition const>>>("process"_s);
					s_CharacterFontWidget2.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::CharacterFontWidget2::Decode, VersionedOperation::Constant::process(), TypePackage<FontWidgetDefinition>, InputBinaryStreamView, FontWidgetDefinition>>>("process"_s);
				}
				{
					using Tool::Popcap::Package::Version;
					using Tool::Popcap::Package::VersionPackage;
					using Tool::Popcap::Package::Definition;
					using PackageDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Package,
						Definition<VersionPackage::element<2_ixz>>::Package
					>;
					auto s_Package = s_Popcap.add_space("Package"_s);
					define_generic_class<Version>(s_Package, "Version"_s);
					{
						auto s_Definition = s_Package.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_Package.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Package::Pack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition>, OutputBinaryStreamView, PackageDefinition const, Path const>>>("process"_s);
					s_Package.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::Package::Unpack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition>, InputBinaryStreamView, PackageDefinition, Optional<Path> const>>>("process"_s);
				}
				{
					using Tool::Popcap::ResourceStreamGroup::Version;
					using Tool::Popcap::ResourceStreamGroup::VersionPackage;
					using Tool::Popcap::ResourceStreamGroup::Definition;
					using PackageDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Package,
						Definition<VersionPackage::element<2_ixz>>::Package,
						Definition<VersionPackage::element<3_ixz>>::Package
					>;
					auto s_ResourceStreamGroup = s_Popcap.add_space("ResourceStreamGroup"_s);
					define_generic_class<Version>(s_ResourceStreamGroup, "Version"_s);
					{
						auto s_Definition = s_ResourceStreamGroup.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					s_ResourceStreamGroup.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ResourceStreamGroup::Pack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition>, OutputBinaryStreamView, PackageDefinition const, Path const>>>("process"_s);
					s_ResourceStreamGroup.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ResourceStreamGroup::Unpack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition>, InputBinaryStreamView, PackageDefinition, Optional<Path> const>>>("process"_s);
				}
				{
					using Tool::Popcap::ResourceStreamBundle::Version;
					using Tool::Popcap::ResourceStreamBundle::VersionPackage;
					using Tool::Popcap::ResourceStreamBundle::Definition;
					using Tool::Popcap::ResourceStreamBundle::Manifest;
					using PackageDefinition = Variant<
						Definition<VersionPackage::element<1_ixz>>::Package,
						Definition<VersionPackage::element<2_ixz>>::Package,
						Definition<VersionPackage::element<3_ixz>>::Package,
						Definition<VersionPackage::element<4_ixz>>::Package,
						Definition<VersionPackage::element<5_ixz>>::Package,
						Definition<VersionPackage::element<6_ixz>>::Package
					>;
					using PackageManifestOptional = Variant<
						Optional<Manifest<VersionPackage::element<1_ixz>>::Package>,
						Optional<Manifest<VersionPackage::element<2_ixz>>::Package>,
						Optional<Manifest<VersionPackage::element<3_ixz>>::Package>,
						Optional<Manifest<VersionPackage::element<4_ixz>>::Package>,
						Optional<Manifest<VersionPackage::element<5_ixz>>::Package>,
						Optional<Manifest<VersionPackage::element<6_ixz>>::Package>
					>;
					auto s_ResourceStreamBundle = s_Popcap.add_space("ResourceStreamBundle"_s);
					define_generic_class<Version>(s_ResourceStreamBundle, "Version"_s);
					{
						auto s_Definition = s_ResourceStreamBundle.add_space("Definition"_s);
						auto c_Package = define_generic_class<PackageDefinition, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Definition, "Package"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_Package);
					}
					{
						auto s_Manifest = s_ResourceStreamBundle.add_space("Manifest"_s);
						auto c_PackageOptional = define_generic_class<PackageManifestOptional, GenericClassDefinitionFlag::Constant::generic_mask()>(s_Manifest, "PackageOptional"_s);
						define_variant_class_version_method<Version, VersionPackage>(c_PackageOptional);
					}
					s_ResourceStreamBundle.add_space("Pack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ResourceStreamBundle::Pack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition, PackageManifestOptional>, OutputBinaryStreamView, PackageDefinition const, PackageManifestOptional const, Path const, Optional<Path> const, Optional<Path> const>>>("process"_s);
					s_ResourceStreamBundle.add_space("Unpack"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ResourceStreamBundle::Unpack, VersionedOperation::Constant::process(), TypePackage<PackageDefinition, PackageManifestOptional>, InputBinaryStreamView, PackageDefinition, PackageManifestOptional, Optional<Path> const, Optional<Path> const>>>("process"_s);
				}
				{
					using Tool::Popcap::ResourceStreamBundlePatch::Version;
					using Tool::Popcap::ResourceStreamBundlePatch::VersionPackage;
					auto s_ResourceStreamBundlePatch = s_Popcap.add_space("ResourceStreamBundlePatch"_s);
					define_generic_class<Version>(s_ResourceStreamBundlePatch, "Version"_s);
					s_ResourceStreamBundlePatch.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ResourceStreamBundlePatch::Encode, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, InputBinaryStreamView, OutputBinaryStreamView, Boolean const>>>("process"_s);
					s_ResourceStreamBundlePatch.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&proxy_function_by_versioned<Version, VersionPackage, Tool::Popcap::ResourceStreamBundlePatch::Decode, VersionedOperation::Constant::process(), TypePackage<>, InputBinaryStreamView, OutputBinaryStreamView, InputBinaryStreamView, Boolean const>>>("process"_s);
				}
			}
			// Miscellaneous
			{
				auto s_Miscellaneous = s_Tool.add_space("Miscellaneous"_s);
				{
					auto s_Pvz2cnCryptData = s_Miscellaneous.add_space("Pvz2cnCryptData"_s);
					s_Pvz2cnCryptData.add_space("Encrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::Pvz2cnCryptData::Encrypt::process>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::Pvz2cnCryptData::Encrypt::estimate>>("estimate"_s);
					s_Pvz2cnCryptData.add_space("Decrypt"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::Pvz2cnCryptData::Decrypt::process>>("process"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::Pvz2cnCryptData::Decrypt::estimate>>("estimate"_s);
				}
				{
					auto s_Pvz2cnAlphaPaletteTexture = s_Miscellaneous.add_space("Pvz2cnAlphaPaletteTexture"_s);
					s_Pvz2cnAlphaPaletteTexture.add_space("Encode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::Pvz2cnAlphaPaletteTexture::Encode::process>>("process"_s);
					s_Pvz2cnAlphaPaletteTexture.add_space("Decode"_s)
						.add_function_proxy<&proxy_global_function_with_promotion<&Tool::Miscellaneous::Pvz2cnAlphaPaletteTexture::Decode::process>>("process"_s);
				}
			}
		}
		// Miscellaneous
		{
			auto s_Miscellaneous = s_Kernel.add_space("Miscellaneous"_s);
			define_generic_class<Thread::Thread, GenericClassDefinitionFlag::Constant::default_constructor()>(s_Miscellaneous, "Thread"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Thread::Thread, &Thread::Thread::joinable>>("joinable"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Thread::Thread, &Thread::Thread::join>>("join"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Thread::Thread, &Thread::Thread::detach>>("detach"_s)
				.add_static_function_proxy<&proxy_global_function_with_promotion<&Thread::Thread::yield>>("yield"_s)
				.add_static_function_proxy<&proxy_global_function_with_promotion<&Thread::Thread::sleep>>("sleep"_s);
			define_generic_class<Context, GenericClassDefinitionFlag::Constant::none_mask()>(s_Miscellaneous, "Context"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Context, &Context::evaluate>>("evaluate"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Context, &Context::callback>>("callback"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Context, &Context::spawn>>("spawn"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Context, &Context::busy>>("busy"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Context, &Context::execute>>("execute"_s)
				.add_member_function_proxy<&proxy_member_function_with_promotion<Context, &Context::query_module_home>>("query_module_home"_s);
			s_Miscellaneous
				.add_function_proxy<
					&normalized_lambda<
						[](
						VariableBinaryView & target
					) -> VariableStringView {
							return unsafe_cast<VariableStringView>(target);
						}
					>
				>("cast_BinaryView_to_StringView"_s)
				.add_function_proxy<
					&normalized_lambda<
						[](
						VariableStringView & target
					) -> VariableBinaryView {
							return unsafe_cast<VariableBinaryView>(target);
						}
					>
				>("cast_StringView_to_BinaryView"_s)
				.add_function_proxy<
					&normalized_lambda<
						[](
						Binary & target
					) -> String {
							auto result = String{};
							result.bind(cast_pointer<Character>(target.data()), target.size(), target.capacity());
							target.unbind();
							return result;
						}
					>
				>("cast_moveable_Binary_to_String"_s)
				.add_function_proxy<
					&normalized_lambda<
						[](
						String & target
					) -> Binary {
							auto result = Binary{};
							result.bind(cast_pointer<Byte>(target.data()), target.size(), target.capacity());
							target.unbind();
							return result;
						}
					>
				>("cast_moveable_String_to_Binary"_s)
				.add_function_proxy<
					&normalized_lambda<
						[](
						String & target
					) -> VariableStringView {
							return target.as_view();
						}
					>
				>("cast_String_to_StringView"_s)
				.add_function<
					&normalized_lambda<
						[](
						Script::JavaScript::NativeValueHandler<VariableStringView> & target
					) -> VariableStringView & {
							// NOTE: EXPLAIN: return StringView is cheap
							return target.value();
						}
					>
				>("cast_StringView_to_RawString"_s);
			s_Miscellaneous.add_variable("g_context"_s, context.context().new_value(Script::JavaScript::NativeValueHandler<Context>::new_reference(context)));
			s_Miscellaneous.add_variable("g_version"_s, context.context().new_value(Script::JavaScript::NativeValueHandler<String>::new_instance_allocate(from_std_string(M_version))));
			s_Miscellaneous.add_variable("g_system"_s, context.context().new_value(Script::JavaScript::NativeValueHandler<String>::new_instance_allocate(from_std_string(M_system))));
			s_Miscellaneous.add_variable("g_architecture"_s, context.context().new_value(Script::JavaScript::NativeValueHandler<String>::new_instance_allocate(from_std_string(M_architecture))));
		}
		// ReSharper restore CppInconsistentNaming
		// @formatter:on
		return;
	}

	#pragma endregion

}
