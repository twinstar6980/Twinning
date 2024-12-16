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
			rgba_8888_o,
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
		Format::Constant::rgba_8888_o()
	>;

	// TODO
	inline constexpr auto bpp_of (
		Format const & format
	) -> Size {
		auto result = Size{};
		switch (format.value) {
			case Format::Constant::a_8().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::rgb_332().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::rgb_565().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_5551().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_4444().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgba_8888().value : {
				result = 32_sz;
				break;
			}
			case Format::Constant::argb_1555().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_4444().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::argb_8888().value : {
				result = 32_sz;
				break;
			}
			case Format::Constant::l_8().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::la_44().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::la_88().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::al_44().value : {
				result = 8_sz;
				break;
			}
			case Format::Constant::al_88().value : {
				result = 16_sz;
				break;
			}
			case Format::Constant::rgb_888_o().value : {
				result = 24_sz;
				break;
			}
			case Format::Constant::rgba_8888_o().value : {
				result = 32_sz;
				break;
			}
		}
		return result;
	}

	// ----------------

	struct Common {

	};

}
