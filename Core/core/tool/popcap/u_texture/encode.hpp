#pragma once

#include "core/utility/utility.hpp"
#include "core/tool/popcap/u_texture/version.hpp"
#include "core/tool/data/compression/deflate.hpp"
#include "core/tool/image/texture/encode.hpp"

namespace TwinStar::Core::Tool::PopCap::UTexture {

	template <auto version> requires (check_version(version, {}))
	struct EncodeCommon {

	protected:

		using MagicIdentifier = IntegerU16;

		inline static constexpr auto k_magic_identifier = MagicIdentifier{0x0A75_iu16};

		// ----------------

		struct FormatFlag {
			inline static constexpr auto rgba_8888_o = Integer{1_i};
			inline static constexpr auto rgba_4444 = Integer{2_i};
			inline static constexpr auto rgba_5551 = Integer{3_i};
			inline static constexpr auto rgb_565 = Integer{4_i};
		};

	};

	template <auto version> requires (check_version(version, {}))
	struct Encode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::FormatFlag;

		// ----------------

		static auto exchange_unit_integer (
			OByteStreamView & data,
			Integer const &   value
		) -> Void {
			data.write(cbw<IntegerU16>(value));
			return;
		}

		// ----------------

		static auto compute_data_size_bound (
			Size &                         data_size_bound,
			Image::ImageSize const &       image_size,
			Image::Texture::Format const & format
		) -> Void {
			data_size_bound = k_none_size;
			data_size_bound += bs_static_size<MagicIdentifier>();
			data_size_bound += bs_static_size<IntegerU16>();
			data_size_bound += bs_static_size<IntegerU16>();
			data_size_bound += bs_static_size<IntegerU16>();
			auto texture_data_size = image_size.area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_size_bound = Size{};
			if constexpr (check_version(version, {false})) {
				texture_data_size_bound = texture_data_size;
			}
			if constexpr (check_version(version, {true})) {
				Data::Compression::Deflate::Compress::do_compute_size_bound(texture_data_size, texture_data_size_bound, 15_sz, 9_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
			}
			data_size_bound += texture_data_size_bound;
			return;
		}

		// ----------------

		static auto process_image (
			OByteStreamView &              data,
			Image::CImageView const &      image,
			Image::Texture::Format const & format
		) -> Void {
			data.write_constant(k_magic_identifier);
			auto image_format = Integer{};
			switch (format.value) {
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
				case Image::Texture::Format::Constant::rgb_565().value : {
					image_format = FormatFlag::rgb_565;
					break;
				}
				default : {
					assert_fail(R"(format == /* valid */)");
				}
			}
			exchange_unit_integer(data, cbw<Integer>(image.size().width));
			exchange_unit_integer(data, cbw<Integer>(image.size().height));
			exchange_unit_integer(data, image_format);
			auto texture_data_size = image.size().area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_view = VByteListView{};
			auto texture_data_container = ByteArray{};
			if constexpr (check_version(version, {false})) {
				texture_data_view = data.forward_view(texture_data_size);
			}
			if constexpr (check_version(version, {true})) {
				texture_data_container.allocate(texture_data_size);
				texture_data_view = texture_data_container.as_view();
			}
			Generalization::match<Image::Texture::FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					Image::Texture::Encode<format>::do_process_image(as_lvalue(OByteStreamView{texture_data_view}), image);
				}
			);
			if constexpr (check_version(version, {true})) {
				auto texture_data_stream = IByteStreamView{texture_data_container};
				Data::Compression::Deflate::Compress::do_process_whole(texture_data_stream, data, 9_sz, 15_sz, 9_sz, Data::Compression::Deflate::Strategy::Constant::default_mode(), Data::Compression::Deflate::Wrapper::Constant::zlib());
			}
			return;
		}

	public:

		static auto do_compute_data_size_bound (
			Size &                         data_size_bound,
			Image::ImageSize const &       image_size,
			Image::Texture::Format const & format
		) -> Void {
			restruct(data_size_bound);
			return compute_data_size_bound(data_size_bound, image_size, format);
		}

		// ----------------

		static auto do_process_image (
			OByteStreamView &              data_,
			Image::CImageView const &      image,
			Image::Texture::Format const & format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

	template <auto version> requires (check_version(version, {}))
	struct Decode :
		EncodeCommon<version> {

	protected:

		using Common = EncodeCommon<version>;

		using typename Common::MagicIdentifier;

		using Common::k_magic_identifier;

		using typename Common::FormatFlag;

		// ----------------

		static auto exchange_unit_integer (
			IByteStreamView & data,
			Integer &         value
		) -> Void {
			value = cbw<Integer>(data.read_of<IntegerU16>());
			return;
		}

		// ----------------

		static auto compute_image_size (
			CByteListView const & data,
			Image::ImageSize &    image_size
		) -> Void {
			auto data_stream = IByteStreamView{data};
			data_stream.read_constant(k_magic_identifier);
			auto image_size_width = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			auto image_size_height = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			auto image_format = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data_stream, it); })));
			image_size = Image::ImageSize{cbw<Size>(image_size_width), cbw<Size>(image_size_height)};
			return;
		}

		// ----------------

		static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			data.read_constant(k_magic_identifier);
			auto image_size_width = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			auto image_size_height = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			auto image_format = cbw<Size>(M_apply(M_wrap(Integer{}), M_wrap({ exchange_unit_integer(data, it); })));
			assert_test(image.size() == Image::ImageSize{cbw<Size>(image_size_width), cbw<Size>(image_size_height)});
			auto format = Image::Texture::Format{};
			auto opacity = Boolean{};
			switch (image_format.value) {
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
				case FormatFlag::rgb_565.value : {
					format = Image::Texture::Format::Constant::rgb_565();
					opacity = k_true;
					break;
				}
				default : {
					assert_fail(R"(image_format == /* valid */)");
				}
			}
			auto texture_data_size = image.size().area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>;
			auto texture_data_view = CByteListView{};
			auto texture_data_container = ByteArray{};
			if constexpr (check_version(version, {false})) {
				texture_data_view = data.forward_view(texture_data_size);
			}
			if constexpr (check_version(version, {true})) {
				texture_data_container.allocate(texture_data_size);
				texture_data_view = texture_data_container.as_view();
			}
			if constexpr (check_version(version, {true})) {
				auto texture_data_stream = OByteStreamView{texture_data_container};
				Data::Compression::Deflate::Uncompress::do_process_whole(data, texture_data_stream, 15_sz, Data::Compression::Deflate::Wrapper::Constant::zlib());
				assert_test(texture_data_stream.full());
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
