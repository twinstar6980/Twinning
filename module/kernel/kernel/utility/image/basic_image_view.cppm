module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.basic_image_view;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.image.size;
import twinning.kernel.utility.image.basic_color;
import twinning.kernel.utility.image.basic_pixel;
import twinning.kernel.utility.range.algorithm;
import twinning.kernel.utility.range.integer_range;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TPixel, auto t_constant> requires
		CategoryConstraint<IsPureInstance<TPixel>>
		&& (IsSameOf<t_constant, Boolean>)
	class BasicImageView {

	private:

		using ConstantView = BasicImageView<TPixel, k_true>;

		using ArrayView = ArrayView<TPixel, t_constant>;

	public:

		using Pixel = TPixel;

		inline static constexpr auto constant = Boolean{t_constant};

		using QualifyPixel = AsConstantIf<Pixel, constant.value>;

	protected:

		ArrayView m_data;

		Size m_data_capacity_dummy;

		ImagePosition m_position;

		ImageSize m_size;

		ImageSize m_area;

	public:

		#pragma region constructor

		constexpr ~BasicImageView(
		) = default;

		// ----------------

		constexpr BasicImageView(
		) :
			m_data{},
			m_position{},
			m_size{},
			m_area{} {
			return;
		}

		constexpr BasicImageView(
			BasicImageView const & that
		) = default;

		constexpr BasicImageView(
			BasicImageView && that
		) = default;

		#pragma endregion

		#pragma region operator

		constexpr auto operator =(
			BasicImageView const & that
		) -> BasicImageView & = default;

		constexpr auto operator =(
			BasicImageView && that
		) -> BasicImageView & = default;

		// ----------------

		constexpr auto operator [](
			Size const & x,
			Size const & y
		) const -> QualifyPixel & {
			return thiz.at(x, y);
		}

		// ----------------

		implicit operator ConstantView &() requires
			(!constant.value) {
			return unsafe_cast<ConstantView>(thiz);
		}

		implicit operator ConstantView const &() const requires
			(!constant.value) {
			return unsafe_cast<ConstantView>(thiz);
		}

		#pragma endregion

		#pragma region set

		constexpr auto reset(
		) -> Void {
			thiz.m_data.reset();
			thiz.m_position = ImagePosition{};
			thiz.m_size = ImageSize{};
			thiz.m_area = ImageSize{};
			return;
		}

		constexpr auto set(
			BasicImageView const & view
		) -> Void {
			thiz.m_data = view.m_data;
			thiz.m_position = view.m_position;
			thiz.m_size = view.m_size;
			thiz.m_area = view.m_area;
			return;
		}

		#pragma endregion

		#pragma region access

		constexpr auto data(
		) const -> ArrayView const & {
			return thiz.m_data;
		}

		constexpr auto position(
		) const -> ImagePosition {
			return thiz.m_position;
		}

		constexpr auto size(
		) const -> ImageSize {
			return thiz.m_size;
		}

		constexpr auto area(
		) const -> ImageSize {
			return thiz.m_area;
		}

		#pragma endregion

		#pragma region query

		constexpr auto at(
			Size const & x,
			Size const & y
		) const -> QualifyPixel & {
			assert_test(y < thiz.m_size.height);
			assert_test(x < thiz.m_size.width);
			return thiz.m_data[(thiz.m_position.y + y) * thiz.m_area.width + (thiz.m_position.x + x)];
		}

		// ----------------

		constexpr auto row(
			Size const & y
		) const -> ArrayView {
			assert_test(y < thiz.m_size.height);
			return thiz.m_data.sub((thiz.m_position.y + y) * thiz.m_area.width + (thiz.m_position.x), thiz.m_size.width);
		}

		#pragma endregion

		#pragma region modify

		constexpr auto fill(
			Pixel const & pixel
		) const -> Void requires
			(!constant.value) {
			for (auto & y : SizeRange{thiz.m_size.height}) {
				for (auto & x : SizeRange{thiz.m_size.width}) {
					thiz[x, y] = pixel;
				}
			}
			return;
		}

		constexpr auto draw(
			ConstantView const & image
		) const -> Void requires
			(!constant.value) {
			assert_test(thiz.size() == image.size());
			auto backward = as_constant_pointer(thiz.data().data()) == image.data().data() && (thiz.position().y * thiz.area().width + thiz.position().x) > (image.position().y * image.area().width + image.position().x);
			if (!backward) {
				for (auto & y : SizeRange{thiz.m_size.height}) {
					for (auto & x : SizeRange{thiz.m_size.width}) {
						thiz[x, y] = image[x, y];
					}
				}
			}
			else {
				for (auto & y : SizeRange{thiz.m_size.height}) {
					for (auto & x : SizeRange{thiz.m_size.width}) {
						thiz[thiz.m_size.width - x - 1_sz, thiz.m_size.height - y - 1_sz] = image[thiz.m_size.width - x - 1_sz, thiz.m_size.height - y - 1_sz];
					}
				}
			}
			return;
		}

		#pragma endregion

		#pragma region sub

		constexpr auto sub(
			ImagePosition const & position,
			ImageSize const &     size
		) const -> BasicImageView {
			assert_test(position.y + size.height <= thiz.m_size.height);
			assert_test(position.x + size.width <= thiz.m_size.width);
			auto sub_view = BasicImageView{};
			sub_view.m_data = thiz.m_data;
			sub_view.m_position = ImagePosition{thiz.m_position.x + position.x, thiz.m_position.y + position.y};
			sub_view.m_size = size;
			sub_view.m_area = thiz.m_area;
			return sub_view;
		}

		#pragma endregion

	};

	#pragma endregion

	#pragma region alias

	template <typename TPixel> requires
		AutomaticConstraint
	using VariableBasicImageView = BasicImageView<TPixel, k_false>;

	template <typename TPixel> requires
		AutomaticConstraint
	using ConstantBasicImageView = BasicImageView<TPixel, k_true>;

	#pragma endregion

}
