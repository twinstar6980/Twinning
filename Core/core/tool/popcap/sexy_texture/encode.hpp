#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/sexy_texture/version.hpp"
#include "core/tool/data/compression/deflate.hpp"
#include "core/tool/image/texture/encode.hpp"

namespace TwinStar::Core::Tool::PopCap::SexyTexture {

	template <auto version> requires (check_version(version, {}))
	struct EncodeCommon {

	protected:

		using MagicIdentifier = IntegerU64;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0x0058455459584553_iu64};

		// ----------------

		using VersionNumber = IntegerU32;

		// ----------------

		struct FormatFlag {
			inline static constexpr auto lut_8 = Integer{1_i};
			inline static constexpr auto argb_8888 = Integer{2_i};
			inline static constexpr auto argb_4444 = Integer{3_i};
			inline static constexpr auto argb_1555 = Integer{4_i};
			inline static constexpr auto rgb_565 = Integer{5_i};
			inline static constexpr auto rgba_8888_o = Integer{6_i};
			inline static constexpr auto rgba_4444 = Integer{7_i};
			inline static constexpr auto rgba_5551 = Integer{8_i};
			inline static constexpr auto xrgb_8888 = Integer{9_i};
			inline static constexpr auto la_88 = Integer{10_i};
		};

	};

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::FormatFlag;

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			OByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.write_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU32>(value));
			return;
		}

		// ----------------

		static auto compute_data_size_bound (
			Size &                         data_size_bound,
			Image::ImageSize const &       image_size,
			Image::Texture::Format const & format,
			Boolean const &                compress_texture_data
		) -> Void {
			data_size_bound = k_none_size;
			data_size_bound += bs_static_size<MagicIdentifier>();
			data_size_bound += bs_static_size<VersionNumber>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			data_size_bound += bs_static_size<IntegerU32>();
			auto texture_data_size = image_size.area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_size_bound = Size{};
			if (!compress_texture_data) {
				texture_data_size_bound = texture_data_size;
			} else {
				Data::Compression::Deflate::Compress::do_compute_size_bound(texture_data_size, texture_data_size_bound, 15_sz, 9_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
			}
			data_size_bound += texture_data_size_bound;
			return;
		}

		// ----------------

		static auto process_image (
			OByteStreamView &              data,
			Image::CImageView const &      image,
			Image::Texture::Format const & format,
			Boolean const &                compress_texture_data
		) -> Void {
			data.write_constant(k_magic_identifier);
			data.write_constant(cbw<VersionNumber>(version.number));
			auto image_format = Integer{};
			switch (format.value) {
				case Image::Texture::Format::Constant::argb_8888().value : {
					image_format = FormatFlag::argb_8888;
					break;
				}
				case Image::Texture::Format::Constant::argb_4444().value : {
					image_format = FormatFlag::argb_4444;
					break;
				}
				case Image::Texture::Format::Constant::argb_1555().value : {
					image_format = FormatFlag::argb_1555;
					break;
				}
				case Image::Texture::Format::Constant::rgb_565().value : {
					image_format = FormatFlag::rgb_565;
					break;
				}
				case Image::Texture::Format::Constant::rgba_8888_o().value : {
					image_format = FormatFlag::rgba_8888_o;
					break;
				}
				case Image::Texture::Format::Constant::rgba_4444().value : {
					image_format = FormatFlag::rgba_4444;
					break;
				}
				case Image::Texture::Format::Constant::rgba_5551().value : {
					image_format = FormatFlag::rgba_5551;
					break;
				}
				/*case Image::Texture::Format::Constant::xrgb_8888().value : {
					image_format = FormatFlag::xrgb_8888;
					break;
				}*/
				case Image::Texture::Format::Constant::la_88().value : {
					image_format = FormatFlag::la_88;
					break;
				}
				default : {
					assert_fail(R"(format == /* valid */)");
				}
			}
			exchange_unit_integer(data, cbw<Integer>(image.size().width));
			exchange_unit_integer(data, cbw<Integer>(image.size().height));
			exchange_unit_integer(data, image_format);
			exchange_unit_integer(data, cbw<Integer>(compress_texture_data));
			exchange_unit_constant(data, 0x00000001_iu32);
			auto compress_texture_data_size = Size{};
			auto compress_texture_data_size_stream = OByteStreamView{data.forward_view(bs_static_size<IntegerU32>())};
			exchange_unit_constant(data, 0x00000000_iu32);
			exchange_unit_constant(data, 0x00000000_iu32);
			exchange_unit_constant(data, 0x00000000_iu32);
			auto texture_data_size = image.size().area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_view = VByteListView{};
			auto texture_data_container = ByteArray{};
			if (!compress_texture_data) {
				texture_data_view = data.forward_view(texture_data_size);
				compress_texture_data_size = k_none_size;
			} else {
				texture_data_container.allocate(texture_data_size);
				texture_data_view = texture_data_container.as_view();
			}
			Generalization::match<Image::Texture::FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					Image::Texture::Encode<format>::do_process_image(as_lvalue(OByteStreamView{texture_data_view}), image);
				}
			);
			if (compress_texture_data) {
				auto texture_data_stream = IByteStreamView{texture_data_container};
				auto ripe_texture_data_stream = OByteStreamView{data.reserve_view()};
				Data::Compression::Deflate::Compress::do_process_whole(texture_data_stream, ripe_texture_data_stream, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
				compress_texture_data_size = ripe_texture_data_stream.position();
				data.forward(ripe_texture_data_stream.position());
			}
			exchange_unit_integer(compress_texture_data_size_stream, cbw<Integer>(compress_texture_data_size));
			return;
		}

	public:

		static auto do_compute_data_size_bound (
			Size &                         data_size_bound,
			Image::ImageSize const &       image_size,
			Image::Texture::Format const & format,
			Boolean const &                compress_texture_data
		) -> Void {
			restruct(data_size_bound);
			return compute_data_size_bound(data_size_bound, image_size, format, compress_texture_data);
		}

		// ----------------

		static auto do_process_image (
			OByteStreamView &              data_,
			Image::CImageView const &      image,
			Image::Texture::Format const & format,
			Boolean const &                compress_data
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format, compress_data);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::VersionNumber;

		using typename Common::FormatFlag;

		// ----------------

		template <typename RawValue> requires
			CategoryConstraint<IsPureInstance<RawValue>>
			&& (IsBaseWrapper<RawValue>)
		static auto exchange_unit_constant (
			IByteStreamView & data,
			RawValue const &  value
		) -> Void {
			data.read_constant(value);
			return;
		}

		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU32>());
			return;
		}

		// ----------------

		static auto compute_image_size (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			auto data_stream = IByteStreamView{data};
			data_stream.read_constant(k_magic_identifier);
			data_stream.read_constant(cbw<VersionNumber>(version.number));
			auto image_size_width = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			auto image_size_height = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			auto image_format = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			auto compress_texture_data = cbw<Boolean>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			exchange_unit_constant(data_stream, 0x00000001_iu32);
			auto compress_texture_data_size = cbw<Boolean>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			exchange_unit_constant(data_stream, 0x00000000_iu32);
			exchange_unit_constant(data_stream, 0x00000000_iu32);
			exchange_unit_constant(data_stream, 0x00000000_iu32);
			image_size = Image::ImageSize{cbw<Size>(image_size_width), cbw<Size>(image_size_height)};
			return;
		}

		// ----------------

		static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			data.read_constant(k_magic_identifier);
			data.read_constant(cbw<VersionNumber>(version.number));
			auto image_size_width = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			auto image_size_height = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			auto image_format = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			auto compress_texture_data = cbw<Boolean>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			exchange_unit_constant(data, 0x00000001_iu32);
			auto compress_texture_data_size = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			exchange_unit_constant(data, 0x00000000_iu32);
			exchange_unit_constant(data, 0x00000000_iu32);
			exchange_unit_constant(data, 0x00000000_iu32);
			assert_test(image.size() == Image::ImageSize{cbw<Size>(image_size_width), cbw<Size>(image_size_height)});
			auto format = Image::Texture::Format{};
			auto opacity = Boolean{};
			switch (image_format.value) {
				case FormatFlag::argb_8888.value : {
					format = Image::Texture::Format::Constant::argb_8888();
					opacity = k_false;
					break;
				}
				case FormatFlag::argb_4444.value : {
					format = Image::Texture::Format::Constant::argb_4444();
					opacity = k_false;
					break;
				}
				case FormatFlag::argb_1555.value : {
					format = Image::Texture::Format::Constant::argb_1555();
					opacity = k_false;
					break;
				}
				case FormatFlag::rgb_565.value : {
					format = Image::Texture::Format::Constant::rgb_565();
					opacity = k_true;
					break;
				}
				case FormatFlag::rgba_8888_o.value : {
					format = Image::Texture::Format::Constant::rgba_8888_o();
					opacity = k_false;
					break;
				}
				case FormatFlag::rgba_4444.value : {
					format = Image::Texture::Format::Constant::rgba_4444();
					opacity = k_false;
					break;
				}
				case FormatFlag::rgba_5551.value : {
					format = Image::Texture::Format::Constant::rgba_5551();
					opacity = k_false;
					break;
				}
				case FormatFlag::xrgb_8888.value : {
					format = Image::Texture::Format::Constant::argb_8888();
					opacity = k_true;
					break;
				}
				case FormatFlag::la_88.value : {
					format = Image::Texture::Format::Constant::la_88();
					opacity = k_false;
					break;
				}
				default : {
					assert_fail(R"(image_format == /* valid */)");
				}
			}
			auto texture_data_size = image.size().area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_view = CByteListView{};
			auto texture_data_container = ByteArray{};
			if (!compress_texture_data) {
				texture_data_view = data.forward_view(texture_data_size);
				assert_test(compress_texture_data_size == k_none_size);
			} else {
				texture_data_container.allocate(texture_data_size);
				texture_data_view = texture_data_container.as_view();
			}
			if (compress_texture_data) {
				auto texture_data_stream = OByteStreamView{texture_data_container};
				auto ripe_texture_data_stream = IByteStreamView{data.reserve_view()};
				Data::Compression::Deflate::Uncompress::do_process_whole(ripe_texture_data_stream, texture_data_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				assert_test(ripe_texture_data_stream.position() == compress_texture_data_size);
				assert_test(texture_data_stream.full());
				data.forward(ripe_texture_data_stream.position());
			}
			Generalization::match<Image::Texture::FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					Image::Texture::Decode<format>::do_process_image(as_lvalue(IByteStreamView{texture_data_view}), image);
				}
			);
			if (opacity) {
				for (auto & row : image.data()) {
					for (auto & pixel : row) {
						pixel.alpha = Image::k_color_maximum;
					}
				}
			}
			return;
		}

	public:

		static auto do_compute_image_size (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			restruct(image_size);
			return compute_image_size(data, image_size);
		}

		// ----------------

		static auto do_process_image (
			IByteStreamView &         data_,
			Image::VImageView const & image
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image);
		}

	};

}
