module;

#include "kernel/common.hpp"

export module twinning.kernel.utility.image.image;
import twinning.kernel.utility.builtin;
import twinning.kernel.utility.trait;
import twinning.kernel.utility.box;
import twinning.kernel.utility.exception.utility;
import twinning.kernel.utility.image.size;
import twinning.kernel.utility.image.color;
import twinning.kernel.utility.image.pixel;
import twinning.kernel.utility.image.image_view;
import twinning.kernel.utility.container.list.list_view;
import twinning.kernel.utility.container.array.array;

export namespace Twinning::Kernel::Image {

	#pragma region type

	template <typename TPixel> requires
		CategoryConstraint<IsPureInstance<TPixel>>
	class BasicImage {

	public:

		using Pixel = TPixel;

		using VariablePixel = Pixel;

		using VariablePixelRow = ListView<Pixel, k_false>;

		using VariableView = BasicImageView<Pixel, k_false>;

		using ConstantPixel = Pixel const;

		using ConstantPixelRow = ListView<Pixel, k_true>;

		using ConstantView = BasicImageView<Pixel, k_true>;

	protected:

		Array<Array<Pixel>> m_data;

	public:

		#pragma region constructor

		~BasicImage (
		) {
			thiz.reset();
			return;
		}

		// ----------------

		BasicImage (
		) :
			m_data{} {
			return;
		}

		BasicImage (
			BasicImage const & that
		) = default;

		BasicImage (
			BasicImage && that
		) = default;

		// ----------------

		explicit BasicImage (
			ImageSize const & size
		) :
			BasicImage{} {
			thiz.allocate(size);
			return;
		}

		explicit BasicImage (
			ConstantView const & view
		) :
			BasicImage{} {
			thiz = view;
			return;
		}

		#pragma endregion

		#pragma region operator

		auto operator = (
			BasicImage const & that
		) -> BasicImage & = default;

		auto operator = (
			BasicImage && that
		) -> BasicImage & = default;

		// ----------------

		auto operator = (
			ConstantView const & that
		) -> BasicImage & {
			thiz.draw(that);
			return thiz;
		}

		// ----------------

		auto operator [] (
			Size const & y
		) -> VariablePixelRow const & {
			return thiz.as_view()[y];
		}

		auto operator [] (
			Size const & y
		) const -> ConstantPixelRow const & {
			return thiz.as_view()[y];
		}

		auto operator [] (
			ImagePosition const & position
		) -> Pixel & {
			return thiz.as_view()[position];
		}

		auto operator [] (
			ImagePosition const & position
		) const -> Pixel const & {
			return thiz.as_view()[position];
		}

		// ----------------

		implicit operator VariableView const & () {
			return thiz.as_view();
		}

		implicit operator ConstantView const & () const {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region view

		auto as_view (
		) -> VariableView const & {
			return self_cast<VariableView>(thiz);
		}

		auto as_view (
		) const -> ConstantView const & {
			return self_cast<ConstantView>(thiz);
		}

		// ----------------

		auto view (
		) -> VariableView {
			return thiz.as_view();
		}

		auto view (
		) const -> ConstantView {
			return thiz.as_view();
		}

		#pragma endregion

		#pragma region data & size

		auto data (
		) -> ConstantListView<VariablePixelRow> const & {
			return thiz.as_view().data();
		}

		auto data (
		) const -> ConstantListView<ConstantPixelRow> const & {
			return thiz.as_view().data();
		}

		auto size (
		) const -> ImageSize {
			return thiz.as_view().size();
		}

		#pragma endregion

		#pragma region draw

		auto fill (
			Pixel const & pixel
		) -> Void {
			return thiz.as_view().fill(pixel);
		}

		auto draw (
			ConstantView const & image
		) -> Void {
			return thiz.as_view().draw(image);
		}

		#pragma endregion

		#pragma region sub

		auto sub (
			ImagePosition const & position,
			ImageSize const &     size
		) -> VariableView {
			return thiz.as_view().sub(position, size);
		}

		auto sub (
			ImagePosition const & position,
			ImageSize const &     size
		) const -> ConstantView {
			return thiz.as_view().sub(position, size);
		}

		#pragma endregion

		#pragma region allocate

		auto reset (
		) -> Void {
			thiz.m_data.reset();
			return;
		}

		auto allocate (
			ImageSize const & size
		) -> Void {
			thiz.m_data.allocate(size.height);
			for (auto & row : thiz.m_data) {
				row.allocate(size.width);
			}
			return;
		}

		#pragma endregion

	};

	#pragma endregion

}
