module;

#include "kernel/common.hpp"

export module twinning.kernel.tool.texture.encoding.common;
import twinning.kernel.utility;

export namespace Twinning::Kernel::Tool::Texture::Encoding {

	M_enumeration(
		M_wrap(Format),
		M_wrap(
			a_8,
			rgb_332,
			rgb_565,
			rgba_5551,
			rgba_4444,
			rgba_8888,
			argb_1555,
			argb_4444,
			argb_8888,
			l_8,
			la_44,
			la_88,
			al_44,
			al_88,
			rgb_888_o,
			rgb_888_r,
			rgba_8888_o,
			rgba_8888_r,
			argb_8888_o,
			argb_8888_r,
		),
	);

	using FormatPackage = ValuePackage<
		Format::Constant::a_8(),
		Format::Constant::rgb_332(),
		Format::Constant::rgb_565(),
		Format::Constant::rgba_5551(),
		Format::Constant::rgba_4444(),
		Format::Constant::rgba_8888(),
		Format::Constant::argb_1555(),
		Format::Constant::argb_4444(),
		Format::Constant::argb_8888(),
		Format::Constant::l_8(),
		Format::Constant::la_44(),
		Format::Constant::la_88(),
		Format::Constant::al_44(),
		Format::Constant::al_88(),
		Format::Constant::rgb_888_o(),
		Format::Constant::rgb_888_r(),
		Format::Constant::rgba_8888_o(),
		Format::Constant::rgba_8888_r(),
		Format::Constant::argb_8888_o(),
		Format::Constant::argb_8888_r()
	>;

	// ----------------

	struct Common {

		inline static constexpr auto get_pixel_byte_count(
			Format const & format
		) -> Size {
			auto bit_count = Size{};
			switch (format.value) {
				case Format::Constant::a_8().value: {
					bit_count = 8_sz;
					break;
				}
				case Format::Constant::rgb_332().value: {
					bit_count = 8_sz;
					break;
				}
				case Format::Constant::rgb_565().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::rgba_5551().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::rgba_4444().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::rgba_8888().value: {
					bit_count = 32_sz;
					break;
				}
				case Format::Constant::argb_1555().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::argb_4444().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::argb_8888().value: {
					bit_count = 32_sz;
					break;
				}
				case Format::Constant::l_8().value: {
					bit_count = 8_sz;
					break;
				}
				case Format::Constant::la_44().value: {
					bit_count = 8_sz;
					break;
				}
				case Format::Constant::la_88().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::al_44().value: {
					bit_count = 8_sz;
					break;
				}
				case Format::Constant::al_88().value: {
					bit_count = 16_sz;
					break;
				}
				case Format::Constant::rgb_888_o().value: {
					bit_count = 24_sz;
					break;
				}
				case Format::Constant::rgb_888_r().value: {
					bit_count = 24_sz;
					break;
				}
				case Format::Constant::rgba_8888_o().value: {
					bit_count = 32_sz;
					break;
				}
				case Format::Constant::rgba_8888_r().value: {
					bit_count = 32_sz;
					break;
				}
				case Format::Constant::argb_8888_o().value: {
					bit_count = 32_sz;
					break;
				}
				case Format::Constant::argb_8888_r().value: {
					bit_count = 32_sz;
					break;
				}
				default: throw UnreachableException{};
			}
			return bit_count / k_type_bit_count<Byte>;
		}

	};

}
