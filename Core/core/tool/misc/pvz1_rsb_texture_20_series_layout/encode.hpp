#pragma once

// 把图像的一行打包为一个块(block)，块宽度为定值(32像素)
// 把图像转换为N个二维块区(chunk)，区宽度为定值(32像素)

#include "core/utility/utility.hpp"

namespace TwinKleS::Core::Tool::Other::PvZ1RSBTexture20SeriesLayout {

	inline namespace Common {

		namespace Detail {

			#pragma region parameter

			inline constexpr auto k_block_width = Size{32_sz};

			inline constexpr auto k_chunk_width = Size{32_sz};

			#pragma endregion

		}

	}

	namespace Encode {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Texture::Format>)
			inline auto process (
				OByteStreamView &          data,
				Image::CBitmapView const & image
			) -> Void {
				auto image_size = image.size();
				auto block_height = image_size.width / k_block_width;
				auto chunk_height = image_size.height / k_chunk_width;
				auto data_view = data.forward_view(image_size.area() * bpp_of(format) / k_type_bit_count<Byte>);
				for (auto & block_y : SizeRange{chunk_height}) {
					for (auto & block_x : SizeRange{k_chunk_width}) {
						auto image_row = IStreamView<Image::Pixel>{image[block_y * k_chunk_width + block_x]};
						for (auto & block_row : SizeRange{block_height}) {
							auto position = (block_y * block_height + block_row) * (k_block_width * k_chunk_width) + (block_x * k_block_width);
							auto block_data = OByteStreamView{data_view.sub(position * bpp_of(format) / k_type_bit_count<Byte>, k_block_width * bpp_of(format) / k_type_bit_count<Byte>)};
							for (auto & block_column : SizeRange{k_block_width}) {
								Texture::Encode::process<format>(block_data, image_row.next());
							}
						}
					}
				}
				return;
			}

			inline auto process (
				OByteStreamView &          data,
				Image::CBitmapView const & image,
				Texture::Format const &    format
			) -> Void {
				Generalization::execute<Texture::FormatEnum>(
					format,
					[&] <auto index, auto format> (ValuePackage<index, format>) {
						process<format>(data, image);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

	namespace Decode {

		namespace Detail {

			#pragma region using

			using namespace Common::Detail;

			#pragma endregion

			#pragma region process

			template <auto format> requires
				CategoryConstraint<>
				&& (IsSameV<format, Texture::Format>)
			inline auto process (
				IByteStreamView &          data,
				Image::VBitmapView const & image
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
								Texture::Decode::process<format>(block_data, image_row.next());
							}
						}
					}
				}
				return;
			}

			inline auto process (
				IByteStreamView &          data,
				Image::VBitmapView const & image,
				Texture::Format const &    format
			) -> Void {
				Generalization::execute<Texture::FormatEnum>(
					format,
					[&] <auto index, auto format> (ValuePackage<index, format>) {
						process<format>(data, image);
					}
				);
				return;
			}

			#pragma endregion

		}

		using Detail::process;

	}

}
