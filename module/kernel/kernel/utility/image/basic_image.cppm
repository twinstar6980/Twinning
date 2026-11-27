module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.basic_image;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception;
import twinning.kernel.utility.image.size;
import twinning.kernel.utility.image.basic_color;
import twinning.kernel.utility.image.basic_pixel;
import twinning.kernel.utility.image.basic_image_view;
import twinning.kernel.utility.container.array.array_view;
import twinning.kernel.utility.container.array.array;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TPixel> requires
		CategoryConstraint<IsPureInstance<TPixel>>
	class BasicImage {

	public:

		using Pixel = TPixel;

		using VariablePixel = Pixel;

		using VariablePixelRow = ArrayView<Pixel, k_false>;

		using VariableView = BasicImageView<Pixel, k_false>;

		using ConstantPixel = Pixel const;

		using ConstantPixelRow = ArrayView<Pixel, k_true>;

		using ConstantView = BasicImageView<Pixel, k_true>;

	protected:

		Array<Pixel> m_data;

		ImagePosition m_position;

		ImageSize m_size;

		ImageSize m_area;

	public:

		#pragma region constructor

		~BasicImage(
		) = default;

		// ----------------

		BasicImage(
		) :
			m_data{},
			m_position{},
			m_size{},
			m_area{} {
			return;
		}

		BasicImage(
			BasicImage const & that
		) = default;

		BasicImage(
			BasicImage && that
		) = default;

		// ----------------

		explicit BasicImage(
			ImageSize const & size
		) :
			BasicImage{} {
			thiz.allocate(size);
			return;
		}

		explicit BasicImage(
			ConstantView const & view
		) :
			BasicImage{} {
			thiz = view;
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator =(
			BasicImage const & that
		) -> BasicImage & = default;

		auto operator =(
			BasicImage && that
		) -> BasicImage & = default;

		// ----------------

		auto operator =(
			ConstantView const & that
		) -> BasicImage & {
			auto that_copy = that;
			auto _ = as_moveable(thiz);
			thiz.allocate(that_copy.size());
			thiz.draw(that_copy);
			return thiz;
		}

		// ----------------

		auto operator [](
			Size const & x,
			Size const & y
		) -> Pixel & {
			return thiz.at(x, y);
		}

		auto operator [](
			Size const & x,
			Size const & y
		) const -> Pixel const & {
			return thiz.at(x, y);
		}

		// ----------------

		implicit operator VariableView const &() {
			return thiz.as_view();
		}

		implicit operator ConstantView const &() const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		auto as_view(
		) -> VariableView const & {
			return unsafe_cast<VariableView>(thiz);
		}

		auto as_view(
		) const -> ConstantView const & {
			return unsafe_cast<ConstantView>(thiz);
		}

		#pragma endregion

		#pragma region access

		auto data(
		) const -> Array<Pixel>::ConstantView const & {
			return thiz.as_view().data();
		}

		auto position(
		) const -> ImagePosition {
			return thiz.as_view().position();
		}

		auto size(
		) const -> ImageSize {
			return thiz.as_view().size();
		}

		auto area(
		) const -> ImageSize {
			return thiz.as_view().area();
		}

		#pragma endregion

		#pragma region query

		auto at(
			Size const & x,
			Size const & y
		) -> Pixel & {
			return thiz.as_view().at(x, y);
		}

		auto at(
			Size const & x,
			Size const & y
		) const -> Pixel const & {
			return thiz.as_view().at(x, y);
		}

		// ----------------

		auto row(
			Size const & y
		) -> VariablePixelRow {
			return thiz.as_view().row(y);
		}

		auto row(
			Size const & y
		) const -> ConstantPixelRow {
			return thiz.as_view().row(y);
		}

		#pragma endregion

		#pragma region modify

		auto fill(
			Pixel const & pixel
		) -> Void {
			return thiz.as_view().fill(pixel);
		}

		auto draw(
			ConstantView const & image
		) -> Void {
			return thiz.as_view().draw(image);
		}

		#pragma endregion

		#pragma region sub

		auto sub(
			ImagePosition const & position,
			ImageSize const &     size
		) -> VariableView {
			return thiz.as_view().sub(position, size);
		}

		auto sub(
			ImagePosition const & position,
			ImageSize const &     size
		) const -> ConstantView {
			return thiz.as_view().sub(position, size);
		}

		#pragma endregion

		#pragma region allocation

		auto reset(
		) -> Void {
			thiz.m_data.reset();
			thiz.m_position = ImagePosition{};
			thiz.m_size = ImageSize{};
			thiz.m_area = ImageSize{};
			return;
		}

		auto allocate(
			ImageSize const & size
		) -> Void {
			thiz.m_data.allocate_full(size.width * size.height);
			thiz.m_position = ImagePosition{};
			thiz.m_size = size;
			thiz.m_area = size;
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
