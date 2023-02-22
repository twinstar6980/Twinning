#pragma once

// NOTE
// 把图像的一行打包为一个块(block)，块宽度为定值(32像素)
// 把图像转换为N个二维块区(chunk)，区宽度为定值(32像素)

#include "core/utility/utility.hpp"
#include "core/tool/image/texture/encode.hpp"

namespace TwinStar::Core::Tool::Miscellaneous::XboxTiledTexture {

	struct EncodeCommon {

	protected:

		inline static constexpr auto k_block_width = Size{32_sz};

		inline static constexpr auto k_chunk_width = Size{32_sz};

	};

	struct Encode :
		EncodeCommon {

	protected:

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Image::Texture::Format>)
			&& (Image::Texture::FormatPackage::has(format))
		static auto process_image (
			OByteStreamView &         data,
			Image::CImageView const & image
		) -> Void {
			auto image_size = image.size();
			auto block_height = image_size.width / k_block_width;
			auto chunk_height = image_size.height / k_chunk_width;
			auto data_view = data.forward_view(image_size.area() * Image::Texture::bpp_of(format) / k_type_bit_count<Byte>);
			for (auto & block_y : SizeRange{chunk_height}) {
				for (auto & block_x : SizeRange{k_chunk_width}) {
					auto image_row = IStreamView<Image::Pixel>{image[block_y * k_chunk_width + block_x]};
					for (auto & block_row : SizeRange{block_height}) {
						auto position = (block_y * block_height + block_row) * (k_block_width * k_chunk_width) + (block_x * k_block_width);
						auto block_data = OByteStreamView{data_view.sub(position * bpp_of(format) / k_type_bit_count<Byte>, k_block_width * bpp_of(format) / k_type_bit_count<Byte>)};
						for (auto & block_column : SizeRange{k_block_width}) {
							Image::Texture::Encode<format>::do_process_pixel(block_data, image_row.next());
						}
					}
				}
			}
			return;
		}

		static auto process_image (
			OByteStreamView &              data,
			Image::CImageView const &      image,
			Image::Texture::Format const & format
		) -> Void {
			Generalization::match<Image::Texture::FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					process_image<format>(data, image);
				}
			);
			return;
		}

	public:

		static auto do_process_image (
			OByteStreamView &              data_,
			Image::CImageView const &      image,
			Image::Texture::Format const & format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

	struct Decode :
		EncodeCommon {

	protected:

		template <auto format> requires
			CategoryConstraint<>
			&& (IsSameV<format, Image::Texture::Format>)
			&& (Image::Texture::FormatPackage::has(format))
		static auto process_image (
			IByteStreamView &         data,
			Image::VImageView const & image
		) -> Void {
			auto image_size = image.size();
			auto block_height = image_size.width / k_block_width;
			auto chunk_height = image_size.height / k_chunk_width;
			auto data_view = data.forward_view(image_size.area() * bpp_of(format) / k_type_bit_count<Byte>);
			for (auto & block_y : SizeRange{chunk_height}) {
				for (auto & block_x : SizeRange{k_chunk_width}) {
					auto image_row = OStreamView<Image::Pixel>{image[block_y * k_chunk_width + block_x]};
					for (auto & block_row : SizeRange{block_height}) {
						auto position = (block_y * block_height + block_row) * (k_block_width * k_chunk_width) + (block_x * k_block_width);
						auto block_data = IByteStreamView{data_view.sub(position * bpp_of(format) / k_type_bit_count<Byte>, k_block_width * bpp_of(format) / k_type_bit_count<Byte>)};
						for (auto & block_column : SizeRange{k_block_width}) {
							Image::Texture::Decode<format>::do_process_pixel(block_data, image_row.next());
						}
					}
				}
			}
			return;
		}

		static auto process_image (
			IByteStreamView &              data,
			Image::VImageView const &      image,
			Image::Texture::Format const & format
		) -> Void {
			Generalization::match<Image::Texture::FormatPackage>(
				format,
				[&] <auto index, auto format> (ValuePackage<index>, ValuePackage<format>) {
					process_image<format>(data, image);
				}
			);
			return;
		}

	public:

		static auto do_process_image (
			IByteStreamView &              data_,
			Image::VImageView const &      image,
			Image::Texture::Format const & format
		) -> Void {
			M_use_zps_of(data);
			return process_image(data, image, format);
		}

	};

}
